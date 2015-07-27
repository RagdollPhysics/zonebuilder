#include "StdInc.h"
#include "Tool.h"

#define LTM_DESC
#include <tomcrypt.h>

#include <zlib/zlib.h>

#include "GfxWorld.h"
#include "s10e5.h"

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

void UnpackUnitVec(PackedUnitVec in, float* out)
{
	float decodeScale = (float)((float)(unsigned __int8)in.array[3] - -192.0) / 32385.0;
	out[0] = (float)((float)(unsigned __int8)in.array[0] - 127.0) * decodeScale;
	out[1] = (float)((float)(unsigned __int8)in.array[1] - 127.0) * decodeScale;
	out[2] = (float)((float)(unsigned __int8)in.array[2] - 127.0) * decodeScale;
}

void dumpStuff(const char* param)
{
	XZoneInfo info;
	info.name = param;
	info.type1 = 2;
	info.type2 = 0;
	Com_LoadZones(&info, 1);
	Com_Printf("Loaded!\n");

	GfxWorld * world = (GfxWorld*)0x69F9060;

	Com_Printf("Map is %s (%s)\n", world->name, world->baseName);

	FILE* out = fopen(va("%s.obj", world->baseName), "w");

	GfxWorldVertex* verts = world->worldDraw.vd.vertices;
	Com_Printf("Dumping %d verts\n", world->worldDraw.vertexCount);

	fprintf(out, "# location\n");
	for (int i = 0; i < world->worldDraw.vertexCount; i++)
	{
		fprintf(out, "v %g %g %g\n", verts[i].xyz[0], verts[i].xyz[2], verts[i].xyz[1]);
	}

	fprintf(out, "\n\n# texcoords\n");
	for (int i = 0; i < world->worldDraw.vertexCount; i++)
	{
		fprintf(out, "vt %g %g\n", verts[i].texCoord[0], verts[i].texCoord[1]);
	}

	fprintf(out, "\n\n# normals\n");
	for (int i = 0; i < world->worldDraw.vertexCount; i++)
	{
		float normal[3];
		UnpackUnitVec(verts[i].normal, normal);
		fprintf(out, "vn %g %g %g\n", normal[0], normal[1], normal[2]);
	}

	Com_Printf("Dumping %d faces\n", world->worldDraw.indexCount);
	fprintf(out, "\n\n# faces\n");
	for (int i = 0; i < world->worldDraw.indexCount / 3; i++)
	{
		int f1 = world->worldDraw.indices[(i * 3) + 0] + 1;
		int f2 = world->worldDraw.indices[(i * 3) + 1] + 1;
		int f3 = world->worldDraw.indices[(i * 3) + 2] + 1;
		fprintf(out, "f %d/%d/%d %d/%d/%d %d/%d/%d\n", f1, f1, f1, f2, f2, f2, f3, f3, f3);

	}

	fclose(out);

	__asm int 3
}

#if ZB_DEBUG
const char** defaultAssetNames = (const char**)0x799958;
extern char header[];
void buildDefaults()
{
	zoneInfo_t* info = getZoneInfo("defaults");
	loadAsset(info, ASSET_TYPE_PHYSPRESET, "THIS_SHOULDNT_EXIST", "default");
	loadAsset(info, ASSET_TYPE_PHYS_COLLMAP, "THIS_SHOULDNT_EXIST", "void");
	loadAsset(info, ASSET_TYPE_XANIM, "THIS_SHOULDNT_EXIST", "void");
	loadAsset(info, ASSET_TYPE_PIXELSHADER, "THIS_SHOULDNT_EXIST", "null.hlsl");
	loadAsset(info, ASSET_TYPE_VERTEXSHADER, "THIS_SHOULDNT_EXIST", "transform_only.hlsl");
	loadAsset(info, ASSET_TYPE_VERTEXDECL, "THIS_SHOULDNT_EXIST", "pp");
	loadAsset(info, ASSET_TYPE_TECHSET, "THIS_SHOULDNT_EXIST", "default");
	loadAsset(info, ASSET_TYPE_IMAGE, "THIS_SHOULDNT_EXIST", "$white");
	loadAsset(info, ASSET_TYPE_MATERIAL, "THIS_SHOULDNT_EXIST", "$default");
	loadAsset(info, ASSET_TYPE_XMODEL, "THIS_SHOULDNT_EXIST", "void");
	//loadAsset(info, ASSET_TYPE_SOUND, "THIS_SHOULDNT_EXIST", "");
	//loadAsset(info, ASSET_TYPE_SNDCURVE, "THIS_SHOULDNT_EXIST", "");
	//loadAsset(info, ASSET_TYPE_LOADED_SOUND, "THIS_SHOULDNT_EXIST", "");
	//loadAsset(info, ASSET_TYPE_LIGHTDEF, "THIS_SHOULDNT_EXIST", "");
	loadAsset(info, ASSET_TYPE_FONT, "THIS_SHOULDNT_EXIST", "fonts/consolefont");
	//loadAsset(info, ASSET_TYPE_MENUFILE, "THIS_SHOULDNT_EXIST", "ui/default.menu");
	//loadAsset(info, ASSET_TYPE_MENU, "THIS_SHOULDNT_EXIST", "default_menu");
	Localize* loc = new Localize;
	loc->name = "CGAME_UNKNOWN";
	loc->localizedString = NULL;
	addAsset(info, ASSET_TYPE_LOCALIZE, "CGAME_UNKNOWN", loc);
	loadAsset(info, ASSET_TYPE_FX, "THIS_SHOULDNT_EXIST", "misc/missing_fx");
	loadAsset(info, ASSET_TYPE_RAWFILE, "THIS_SHOULDNT_EXIST", "codescripts/$default");
	loadAsset(info, ASSET_TYPE_STRINGTABLE, "THIS_SHOULDNT_EXIST", "mp/defaultStringTable.csv");
	//loadAsset(info, ASSET_TYPE_LEADERBOARDDEF, "THIS_SHOULDNT_EXIST", "DEFAULT_LB");
	//loadAsset(info, ASSET_TYPE_STRUCTUREDDATADEF, "THIS_SHOULDNT_EXIST", "mp/defaultStructuredData.def");
	loadAsset(info, ASSET_TYPE_TRACER, "THIS_SHOULDNT_EXIST", "defaulttracer");
	//loadAsset(info, ASSET_TYPE_VEHICLE, "THIS_SHOULDNT_EXIST", "defaultvehicle");

	loadAsset(info, ASSET_TYPE_WEAPON, "defaultweapon_mp", "defaultweapon_mp");

	Sleep(100);

	ZStream* buf = writeZone(info);

	BUFFER* compressed = buf->compressZlib();
	delete buf;

	Com_Printf("Writing to Disk...");
	CreateDirectoryA("zone", NULL);

	//const char* outputdir = ((char*(*)())0x45CBA0)();
	const char* outputdir = "alter";

	FILE* out = fopen("zone\\alter\\defaults.ff", "wb");
	_setmode(_fileno(out), _O_BINARY); // it was fucking up zlib output

	FILETIME time;
	GetSystemTimeAsFileTime(&time);

	*((int*)&header[13]) = time.dwHighDateTime;
	*((int*)&header[17]) = time.dwLowDateTime;

	fwrite(header, 21, 1, out);
	compressed->writetofile(out);
	delete compressed;

	fclose(out);

	Com_Printf("Done!\n");

}
#endif