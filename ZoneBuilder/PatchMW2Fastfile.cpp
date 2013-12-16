#include "StdInc.h"
#include "Hooking.h"
#include <unordered_map>

void PatchMW2_FilePointers();
void SetUseFilePointers(bool use);

CallHook loadXFileVersionHook;
DWORD loadXFileVersionHookLoc = 0x4158E7;

void __declspec(naked) DB_LoadXFileInternal_VersionHookFunc()
{
	__asm
	{
		push 4
		push eax
		call loadXFileVersionHook.pOriginal
		mov ecx, [esp] // previous value of eax
		mov eax, [ecx] // dereference the pointer
		add esp, 8h

		cmp eax, 115h // IW4 'filepointers' version
		je filePointers

		// don't use filepointers
		push eax
		push 0
		call SetUseFilePointers
		add esp, 4h
		pop eax

		retn

filePointers:
		mov [ecx], 114h // patch version back

		// use filepointers
		push 1
		call SetUseFilePointers
		add esp, 4h

		retn
	}
}

void PatchMW2_Fastfile()
{
	PatchMW2_FilePointers();

	loadXFileVersionHook.initialize(loadXFileVersionHookLoc, DB_LoadXFileInternal_VersionHookFunc);
	loadXFileVersionHook.installHook();
}

static std::unordered_map<DWORD, void*> filePointers;
static std::unordered_map<void*, DWORD> filePointersReverse;

StompHook offsetToPointerHook;
DWORD offsetToPointerHookLoc = 0x4A82B0;
DWORD offsetToPointerHookRet = 0x4A82B9;

void* OffsetToPointerHookFunc(DWORD offset)
{
	offset &= 0xFFFFFFF;

	return filePointers[offset];
}

void __declspec(naked) OffsetToPointerHookStub()
{
	__asm
	{
		mov ecx, [esp + 4]
		mov eax, [ecx]
		sub eax, 1
		mov edx, eax
		shr edx, 1Ch

		cmp edx, 0Fh
		jne returnToSender

		push eax
		call OffsetToPointerHookFunc
		add esp, 4

		mov ecx, [esp + 4]
		mov [ecx], eax
		retn

returnToSender:
		jmp offsetToPointerHookRet
	}
}

StompHook offsetToAliasHook;
DWORD offsetToAliasHookLoc = 0x4FDFA0;
DWORD offsetToAliasHookRet = 0x4FDFA9;

void* OffsetToAliasHookFunc(DWORD offset)
{
	offset &= 0xFFFFFFF;

	return filePointers[offset];
}

void __declspec(naked) OffsetToAliasHookStub()
{
	__asm
	{
		mov ecx, [esp + 4]
		mov eax, [ecx]
		sub eax, 1
		mov edx, eax
		shr edx, 1Ch

		cmp edx, 0Fh
		jne returnToSender

		push eax
		call OffsetToAliasHookFunc
		add esp, 4

		mov ecx, [esp + 4]
		mov [ecx], eax
		retn

returnToSender:
		jmp offsetToAliasHookRet
	}
}

static bool g_useFilePointers = false;

CallHook loadStreamStoreHook;
DWORD loadStreamStoreHookLoc = 0x470E6C;

void Load_StreamStoreHookFunc(void* ptr)
{
	if (g_useFilePointers)
	{
		DWORD offset = *(DWORD*)0x112A6C8;
		filePointers[offset] = ptr;
		filePointersReverse[ptr] = offset;

		if (offset == 0x1E83)
		{
			//DebugBreak();
		}
	}
}

void __declspec(naked) Load_StreamStoreHookStub()
{
	__asm
	{
		push ecx
		call Load_StreamStoreHookFunc
		add esp, 4h

		jmp loadStreamStoreHook.pOriginal
	}
}

StompHook fpdbAddXAssetHook;
DWORD fpdbAddXAssetHookLoc = 0x5BB657;
DWORD fpdbAddXAssetHookRet = 0x5BB65F;

static void* g_assetPtr;

void MarkReplaceAsset(void** asset)
{
	g_assetPtr = *asset;
}

void DoMarkReplaceAsset(DWORD* arg)
{
	if (!g_useFilePointers)
	{
		return;
	}

	void* newPtr = (void*)arg[1];

	/*for (google::dense_hash_map<DWORD, void*>::iterator i = filePointers.begin(); i != filePointers.end(); i++)
	{
		if ((*i).second == g_assetPtr)
		{
			filePointers[(*i).first] = newPtr;
			return;
		}
	}*/

	filePointers[filePointersReverse[g_assetPtr]] = newPtr;
	filePointersReverse[newPtr] = filePointersReverse[g_assetPtr];
}

void __declspec(naked) FPDB_AddXAssetHookStub()
{
	__asm
	{
		push esi
		call MarkReplaceAsset
		add esp, 4

		mov eax, [esp + 14h + 8]
		mov ecx, [eax]
		push ebx
		push ebp
		mov ebp, [esp + 1Ch + 4]
		jmp fpdbAddXAssetHookRet
	}
}

// there should be a better way by stack return pointer injection, but I'm too lazy to figure out
StompHook addXAssetTailHook1;
DWORD addXAssetTailHook1Loc = 0x5BB704;

StompHook addXAssetTailHook2;
DWORD addXAssetTailHook2Loc = 0x5BB9A2;

StompHook addXAssetTailHook3;
DWORD addXAssetTailHook3Loc = 0x5BB978;

StompHook addXAssetTailHook4;
DWORD addXAssetTailHook4Loc = 0x5BB85D;

StompHook addXAssetTailHook5;
DWORD addXAssetTailHook5Loc = 0x5BB895;

void __declspec(naked) AddXAssetTailHook1Stub()
{
	__asm
	{
		push eax
		call DoMarkReplaceAsset
		pop eax

		pop ebx
		add esp, 14h
		retn
	}
}

void SetUseFilePointers(bool use)
{
	filePointers.clear();
	filePointersReverse.clear();

	g_useFilePointers = use;
}

void PatchMW2_FilePointers()
{
	//filePointers.set_empty_key(0);
	//filePointersReverse.set_empty_key(NULL);

	offsetToPointerHook.initialize(offsetToPointerHookLoc, OffsetToPointerHookStub);
	offsetToPointerHook.installHook();

	offsetToAliasHook.initialize(offsetToAliasHookLoc, OffsetToAliasHookStub);
	offsetToAliasHook.installHook();

	loadStreamStoreHook.initialize(loadStreamStoreHookLoc, Load_StreamStoreHookStub);
	loadStreamStoreHook.installHook();

	fpdbAddXAssetHook.initialize(fpdbAddXAssetHookLoc, FPDB_AddXAssetHookStub);
	fpdbAddXAssetHook.installHook();

	addXAssetTailHook1.initialize(addXAssetTailHook1Loc, AddXAssetTailHook1Stub);
	addXAssetTailHook1.installHook();

	addXAssetTailHook2.initialize(addXAssetTailHook2Loc, AddXAssetTailHook1Stub);
	addXAssetTailHook2.installHook();

	addXAssetTailHook3.initialize(addXAssetTailHook3Loc, AddXAssetTailHook1Stub);
	addXAssetTailHook3.installHook();

	addXAssetTailHook4.initialize(addXAssetTailHook4Loc, AddXAssetTailHook1Stub);
	addXAssetTailHook4.installHook();

	addXAssetTailHook5.initialize(addXAssetTailHook5Loc, AddXAssetTailHook1Stub);
	addXAssetTailHook5.installHook();
}