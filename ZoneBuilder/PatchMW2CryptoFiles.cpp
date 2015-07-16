// ==========================================================
// IW4M project
// 
// Component: clientdll
// Sub-component: steam_api
// Purpose: Handling of encrypted archive files.
//
// Initial author: NTAuthority
// Started: 2013-06-19
// ==========================================================

#include "StdInc.h"
#include "Hooking.h"

#define LTM_DESC
#include <tomcrypt.h>

#include "IWDKey.h"

static CRITICAL_SECTION iwdCryptCS;

static unsigned int secureKey[256];
static bool secureKeyInited = false;

static void InitSecureKey()
{
	for (int i = 0; i < sizeof(secureKey) / sizeof(int); i++)
	{
		secureKey[i] = *(unsigned int*)(&dynkey[i * 4]);
	}

	secureKeyInited = true;
}

static void FS_DecryptAESKey(unsigned char* key)
{
	register_hash(&sha256_desc);

	rsa_key rkey;
	int hash = find_hash("sha256");
	rsa_import(iwdPKey, sizeof(iwdPKey), &rkey);

	unsigned long outLen = 24;
	int stat;
	rsa_decrypt_key_ex(iwdKey, sizeof(iwdKey), key, &outLen, 0, 0, hash, 2, &stat, &rkey);
	rsa_free(&rkey);
}

symmetric_CTR iwdCTR;

void FS_InitCrypto()
{
	register_cipher(&aes_desc);

	unsigned char key[24];
	FS_DecryptAESKey(key);

	int iv[4];
	iv[0] = 0x1010101;
	iv[1] = 0x1010101;
	iv[2] = 0x1010101;
	iv[3] = 0x1010101;

	int cipher = find_cipher("aes");
	ctr_start(cipher, (unsigned char*)iv, key, sizeof(key), 0, CTR_COUNTER_BIG_ENDIAN, &iwdCTR);
}

static DWORD tlsIWDKey;

// TODO: replace with crc32 as it's nothing 'secure' whatsoever
int FS_GetIWDKey(const char* filename, int length_, int some0)
{
	if (!secureKeyInited)
	{
		InitSecureKey();
	}

	int key = ~some0;

	/*for (int i = 0; i < length; i++)
	{
		key = secureKey[(BYTE)(key ^ filename[i])] ^ ((DWORD)(key >> 8));
	}*/

	__asm
	{
		xor eax, eax
		push esi
		mov esi, filename

		mov ecx, length_

		push edi
		push edx
		mov edx, key

theLoop:
		movzx edi, byte ptr [eax + esi]
		xor edi, edx
		and edi, 0FFh
		shr edx, 8
		xor edx, secureKey[edi * 4]
		inc eax
		cmp eax, ecx
		jl theLoop

		mov key, edx

		pop edx
		pop edi
		pop esi
	}

	return ~key;
}

struct unz_s
{
	FILE* file;
	char pad[124];
	int encrypted;
	int fileKey;
};

static unz_s* unzunz;

int FS_IsFileEncrypted(const char* filename)
{
	return (strstr(filename, "iw_image") || strstr(filename, "iw_model") || strstr(filename, "iw_config") || strstr(filename, "iw_sound") || strstr(filename, "iw_se5") || strstr(filename, "iw_sh"));
}

void FS_LoadZipFile_getFileKey(unz_s* uf, const char* filename)
{
	unzunz = uf;

	uf->encrypted = FS_IsFileEncrypted(filename);
	uf->fileKey = FS_GetIWDKey(filename, strlen(filename), 0);

	TlsSetValue(tlsIWDKey, (LPVOID)uf->fileKey);
}

void __declspec(naked) FS_LoadZipFile_getFileKeyHook()
{
	__asm
	{
		mov eax, [esp + 184h + 8]
		push eax
		mov eax, [esp + 8]
		push eax
		call FS_LoadZipFile_getFileKey
		add esp, 8h

		// unzGoToFirstFile
		push 4D71D0h
		retn
	}
}

void FS_LoadZipFile_setIV(int index)
{
	int iv[4];
	iv[0] = 0x1010101;
	iv[1] = (int)TlsGetValue(tlsIWDKey);
	iv[2] = index;
	iv[3] = 0x1010101;

	ctr_setiv((unsigned char*)iv, sizeof(iv), &iwdCTR);
}

void __declspec(naked) FS_LoadZipFile_setIV1Hook()
{
	__asm
	{
		push edi // file index
		call FS_LoadZipFile_setIV
		add esp, 4h

		// unzGetCurrentFileInfo
		push 444A60h
		retn
	}
}

void __declspec(naked) FS_LoadZipFile_setIV2Hook()
{
	__asm
	{
		mov eax, [esp + 1A0h - 158h]
		push eax // file index
		call FS_LoadZipFile_setIV
		add esp, 4h

		// unzGetCurrentFileInfo
		push 444A60h
		retn
	}
}

struct pakfile_t
{
	char pad[804];
	int fileKey;
};

static std::unordered_map<DWORD, pakfile_t*> _filePtrToPakFile;

void FS_LoadZipFile_storeFileKeyDo(pakfile_t* pakFile)
{
	pakFile->fileKey = (int)TlsGetValue(tlsIWDKey);

	_filePtrToPakFile[(DWORD)unzunz->file] = pakFile;
}

void __declspec(naked) FS_LoadZipFile_storeFileKey()
{
	__asm
	{
		push esi
		call FS_LoadZipFile_storeFileKeyDo
		add esp, 4h

		// _crc32
		push 4B1330h
		retn
	}
}

void unzGetCurrentFileInfo_decrypt(unsigned char* buffer, size_t length, unz_s* unzFile)
{
	if (unzFile->encrypted)
	{
		EnterCriticalSection(&iwdCryptCS);
		ctr_decrypt(buffer, buffer, length, &iwdCTR);
		LeaveCriticalSection(&iwdCryptCS);
	}
}

static void* unzBuffer;

void __declspec(naked) unzGetCurrentFileInfo_decryptHook()
{
	__asm
	{
		mov unzBuffer, eax
		mov eax, 44E830h
		call eax

		add esp, 0Ch
		cmp eax, edi
		jz carryOn

		or ebp, 0FFFFFFFFh

carryOn:
		sub edx, edi
		test ebp, ebp

		jnz returnOtherFalse

		mov eax, unzBuffer
		push esi
		push edi
		push eax
		call unzGetCurrentFileInfo_decrypt
		add esp, 0Ch

		mov edi, [esp + 3Ch]

		push 65C443h
		retn

returnOtherFalse:
		mov edi, [esp + 3Ch]

		push 65C4AAh
		retn
	}
}

void unzReadCurrentFile_decrypt(unsigned char* buffer, int length, int positionInFile, DWORD file, unz_s* zFile)
{
	int iv[4];
	iv[0] = positionInFile;
	iv[1] = zFile->fileKey;//_filePtrToPakFile[file]->fileKey;
	iv[2] = 0x1010101;
	iv[3] = 0x1010101;

	EnterCriticalSection(&iwdCryptCS);
	ctr_setiv((unsigned char*)iv, sizeof(iv), &iwdCTR);
	ctr_decrypt(buffer, buffer, length, &iwdCTR);
	LeaveCriticalSection(&iwdCryptCS);
}

void __declspec(naked) unzReadCurrentFile_decryptHook()
{
	__asm
	{
		test edi, edi
		jz continued

		mov eax, [esp + 14h]
		mov eax, [eax + 80h]
		test eax, eax

		jz continued

		mov eax, [esp + 14h]
		push eax

		mov eax, [esi + 54h]
		push eax

		mov eax, [esi + 38h]
		push eax
		push edi

		mov ecx, [esi]
		push ecx
		call unzReadCurrentFile_decrypt
		add esp, 14h

continued:
		mov edx, [esi]
		add [esi + 38h], edi
		sub [esi + 4Ch], edi
		mov [esi + 4], edx
		mov [esi + 8], edi

		push 4302D3h
		retn
	}
}

void PatchMW2_CryptoZone();
void PatchMW2_CryptoFilesStreamDebug();

void PatchMW2_CryptoFiles()
{
	tlsIWDKey = TlsAlloc();

	ltc_mp = ltm_desc;

	PatchMW2_CryptoZone();

	call(0x486212, FS_InitCrypto, PATCH_JUMP);

	call(0x64248A, FS_LoadZipFile_getFileKeyHook, PATCH_CALL);
	call(0x6424BB, FS_LoadZipFile_setIV1Hook, PATCH_CALL);
	call(0x642660, FS_LoadZipFile_setIV2Hook, PATCH_CALL);

	call(0x642737, FS_LoadZipFile_storeFileKey, PATCH_CALL);

	call(0x65C42A, unzGetCurrentFileInfo_decryptHook, PATCH_JUMP);

	call(0x4302C5, unzReadCurrentFile_decryptHook, PATCH_JUMP);

	// unz_s size
	*(DWORD*)0x431EE6 += 8;
	*(DWORD*)0x4B5348 += 8; // unz_s clone function
	*(DWORD*)0x4B5356 += 8; // needed for multiple files to be loaded simultaneously?

	// packfile_t size
	*(DWORD*)0x642549 += 4;
	*(DWORD*)0x64255A += 4;

	InitializeCriticalSection(&iwdCryptCS);

	//PatchMW2_CryptoFilesStreamDebug();
}

#if 0
FS_FOpenFileRead_t FS_FOpenFileReadForThread = (FS_FOpenFileRead_t)0x643270;

#define AILCALLBACK __stdcall
#define U32 unsigned __int32
#define S32 __int32
#define MSS_FILE char

typedef U32	(AILCALLBACK FAR*AIL_file_open_callback)	(MSS_FILE const FAR* Filename,
														 U32 FAR* FileHandle);

typedef void (AILCALLBACK FAR*AIL_file_close_callback) (U32 FileHandle);

#define AIL_FILE_SEEK_BEGIN	0
#define AIL_FILE_SEEK_CURRENT 1
#define AIL_FILE_SEEK_END	 2

typedef S32	(AILCALLBACK FAR*AIL_file_seek_callback)	(U32 FileHandle,
														 S32 Offset,
														 U32 Type);

typedef U32	(AILCALLBACK FAR*AIL_file_read_callback)	(U32 FileHandle,
														 void FAR* Buffer,
														 U32 Bytes);

static std::unordered_map<int, std::string> _mssFileMap;

U32 __stdcall OpenCB(char const* filename, U32* fh)
{
	U32 rv = FS_FOpenFileRead(filename, (int*)fh, 1) >= 0;

	_mssFileMap[*fh] = filename;

	OutputDebugString(va("MSS open %s\n", filename));

	return rv;
}

void __stdcall CloseCB(U32 fh)
{
	OutputDebugString(va("MSS close %s\n", _mssFileMap[fh].c_str()));

	FS_FCloseFile(fh);
}

S32 __stdcall SeekCB(U32 fh, S32 offset, U32 type)
{
	int qtype = 2;

	if (type == 1)
	{
		qtype = 0;
	}
	else if (type == 2)
	{
		qtype = 1;
	}
	else if (type != 0)
	{
		return 0;
	}

	FS_Seek(fh, offset, qtype);

	OutputDebugString(va("MSS seek %s (off %i type %i) %i\n", _mssFileMap[fh].c_str(), offset, type, FS_FTell(fh)));

	return FS_FTell(fh);
}

U32 __stdcall ReadCB(U32 fh, void* buffer, U32 bytes)
{
	OutputDebugString(va("MSS read %s %i\n", _mssFileMap[fh].c_str(), bytes));

	return FS_Read(buffer, bytes, fh);
}

typedef void	(__stdcall * AIL_set_file_callbacks_t)	(AIL_file_open_callback opencb,
														 AIL_file_close_callback closecb,
														 AIL_file_seek_callback seekcb,
														 AIL_file_read_callback readcb);

void __stdcall _AIL_set_file_callbacks(AIL_file_open_callback opencb,
									   AIL_file_close_callback closecb,
									   AIL_file_seek_callback seekcb,
									   AIL_file_read_callback readcb)
{
	int fh1;
	int fh2;

	int l1 = FS_FOpenFileReadForThread("sound/music/ui_lobby_answering_the_call.mp3", &fh1, 1);
	int l2 = FS_FOpenFileReadForThread("sound/music/hz_special_ops_2mx.mp3", &fh2, 1);

	OutputDebugString(va("fh1 %i\nfh2 %i\n", l1, l2));

	//FS_Seek(fh1, 0, 1);
	OutputDebugString(va("fh1.1 %i\n", FS_FTell(fh1)));

	//FS_Seek(fh1, 2048, 1);
	OutputDebugString(va("fh1.2 %i\n", FS_FTell(fh1)));

	char buf[2048];
	int l;
	//l = FS_Read(buf, 1024, fh1);

	OutputDebugString(va("fh1.3 %i %i\n", l, FS_FTell(fh1)));

	FS_Seek(fh2, 0, 1);
	OutputDebugString(va("fh2.1 %i\n", FS_FTell(fh2)));

	FS_Seek(fh2, 2048, 1);
	OutputDebugString(va("fh2.2 %i\n", FS_FTell(fh2)));

	l = FS_Read(buf, 1024, fh2);

	OutputDebugString(va("fh2.3 %i %i\n", l, FS_FTell(fh2)));

	ExitProcess(1);

	static AIL_set_file_callbacks_t func;

	if (!func)
	{
		func = (AIL_set_file_callbacks_t)GetProcAddress(GetModuleHandle("mss32.dll"), "_AIL_set_file_callbacks@16");
	}

	func(OpenCB, CloseCB, SeekCB, ReadCB);
}

void PatchMW2_CryptoFilesStreamDebug()
{
	*(DWORD*)0x6D7530 = (DWORD)_AIL_set_file_callbacks;
}
#endif