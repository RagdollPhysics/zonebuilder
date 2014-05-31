// ==========================================================
// IW4M project
// 
// Component: clientdll
// Sub-component: steam_api
// Purpose: Handling of encrypted archive/fastfiles.
//
// Initial author: NTAuthority
// Started: 2013-06-19
// ==========================================================

#include "StdInc.h"
#include "Hooking.h"

#define LTM_DESC
#include <tomcrypt.h>

#include <zlib/zlib.h>

#include "FFKey.h"

typedef void (__cdecl * DB_ReadXFileRawData_t)(void* buffer, int length);
DB_ReadXFileRawData_t DB_ReadXFileRawData = (DB_ReadXFileRawData_t)0x4705E0;

struct ZoneKey
{
	unsigned char key[24];
	unsigned char iv[16];
};

void DB_AuthLoad_DecryptKey(unsigned char* encKey, ZoneKey* tKey)
{
	int hash = find_hash("sha256");

	rsa_key key;
	rsa_import(ffKey, sizeof(ffKey), &key);

	int stat;
	unsigned long outLen = 40;
	rsa_decrypt_key_ex(encKey, 256, (unsigned char*)tKey, &outLen, NULL, NULL, hash, 2, &stat, &key);

	rsa_free(&key);
}

static symmetric_CTR ffCTR;
static unsigned char ffIV[16];

extern int ffVersion;

void DB_AuthLoad_InitCrypto()
{
	if (ffVersion < 319)
	{
		return;
	}

	register_hash(&sha256_desc);
	register_cipher(&aes_desc);

	unsigned char encKey[256];
	DB_ReadXFileRawData(encKey, 256);

	ZoneKey key;
	DB_AuthLoad_DecryptKey(encKey, &key);

	int aes = find_cipher("aes");
	ctr_start(aes, key.iv, key.key, sizeof(key.key), 0, 0, &ffCTR);

	memcpy(ffIV, key.iv, sizeof(ffIV));
}

void DB_AuthLoad_InflateInit_decryptFunc(z_stream* strm)
{
	if (ffVersion < 319)
	{
		return;
	}

	ctr_decrypt(strm->next_in, strm->next_in, strm->avail_in, &ffCTR);
}

void __declspec(naked) DB_AuthLoad_InflateInit_decrypt()
{
	__asm
	{
		push ebp
		call DB_AuthLoad_InflateInit_decryptFunc
		add esp, 4h

		push 4D8090h // inflateInit
		retn
	}
}

void DB_AuthLoad_Inflate_compare(unsigned char* buffer, int length, unsigned char* ivValue)
{
	if (ffVersion < 319)
	{
		return;
	}

	// we don't do anything return-like here, as this is just hash comparing, and we don't care about the data after this at all
	ctr_setiv(ivValue, 16, &ffCTR);
	ctr_decrypt(buffer, buffer, length, &ffCTR);
}

void DB_AuthLoad_Inflate_decryptBaseFunc(unsigned char* buffer)
{
	if (ffVersion < 319)
	{
		return;
	}

	ctr_setiv(ffIV, sizeof(ffIV), &ffCTR);
	ctr_decrypt(buffer, buffer, 8192, &ffCTR);
}

void __declspec(naked) DB_AuthLoad_Inflate_decryptBase()
{
	__asm
	{
		push ebx
		call DB_AuthLoad_Inflate_decryptBaseFunc
		pop ebx

		push 5B96F0h
		retn
	}
}

void PatchMW2_CryptoZone()
{
	// crypto initialization (does not need to be returned to original function)
	call(0x4D02F0, DB_AuthLoad_InitCrypto, PATCH_CALL);

	// initial stage decryption
	call(0x4D0306, DB_AuthLoad_InflateInit_decrypt, PATCH_CALL);

	// hash bit decryption
	call(0x5B9958, DB_AuthLoad_Inflate_compare, PATCH_CALL);
	call(0x5B9912, DB_AuthLoad_Inflate_compare, PATCH_CALL);

	// general read?
	call(0x5B98E4, DB_AuthLoad_Inflate_decryptBase, PATCH_CALL);
}