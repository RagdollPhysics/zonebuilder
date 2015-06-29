#include "StdInc.h"
#include "Tool.h"

#define LTM_DESC
#include <tomcrypt.h>

#include <zlib/zlib.h>

extern unsigned char ffKey[1191];

extern std::string _buffer;
char* GetZoneLocation(const char* name, bool searchMods);

struct ZoneKey
{
	unsigned char key[24];
	unsigned char iv[16];
};

static symmetric_CTR ffCTR;
static unsigned char ffIV[16];

void decryptFastfile(const char* param)
{
	FILE* input = fopen(param, "rb");
	char ffHeader[21];
	fread(ffHeader, 1, 21, input);

	_buffer.append(ffHeader, 21);

	register_hash(&sha256_desc);
	register_cipher(&aes_desc);

	unsigned char encKey[256];
	fread(encKey, 256, 1, input);

	ZoneKey zKey;

	int hash = find_hash("sha256");

	rsa_key rKey;
	rsa_import(ffKey, sizeof(ffKey), &rKey);

	int stat;
	unsigned long outLen = 40;
	rsa_decrypt_key_ex(encKey, 256, (unsigned char*)&zKey, &outLen, NULL, NULL, hash, 2, &stat, &rKey);

	rsa_free(&rKey);

	int aes = find_cipher("aes");
	ctr_start(aes, zKey.iv, zKey.key, sizeof(zKey.key), 0, 0, &ffCTR);

	memcpy(ffIV, zKey.iv, sizeof(ffIV));

	unsigned char dstBuf[0x10000];
	int count = fread(dstBuf, 1, 0x10000, input);
	while (count == 0x10000)
	{
		ctr_decrypt(dstBuf, dstBuf, 0x10000, &ffCTR);
		_buffer.append((char*)dstBuf, count);
		count = fread(dstBuf, 1, 0x10000, input);
	}
	fclose(input);

	FILE* output = fopen("decrypted.dat", "wb");
	fwrite(_buffer.c_str(), 1, _buffer.length(), output);
	fclose(output);
}

void dumpStuff(const char* param)
{
	XModel* model = (XModel*)DB_FindXAssetHeader(ASSET_TYPE_XMODEL, "mp_body_airborne_assault_a");
	FILE* out = fopen("mp_skeleton.txt", "w");
	fprintf(out, "NUMBONES %d\n", model->numBones);
	for (int i = 0; i < model->numRootBones; i++)
	{
		fprintf(out, "BONE %d -1 %s\n", i, SL_ConvertToString(model->boneNames[i]));
	}
	for (int i = model->numRootBones; i < model->numBones; i++)
	{
		fprintf(out, "BONE %d %d %s \n", i, model->parentList[i - model->numRootBones], SL_ConvertToString(model->boneNames[i]));
	}

	for (int i = 0; i < model->numBones; i++)
	{
		fprintf(out, "\nBONE %d\n", i);
		XModelTagPos* pos = &model->tagPositions[i];
		fprintf(out, "OFFSET %g, %g, %g\n", pos->x, pos->y, pos->z);
		fprintf(out, "SCALE 1.000000, 1.000000, 1.000000\n");
		fprintf(out, "ROT %g %g %g %g\n", model->animMatrix[i].quat[0],
			model->animMatrix[i].quat[1],
			model->animMatrix[i].quat[2],
			model->animMatrix[i].quat[3]);
	}

	fclose(out);
}