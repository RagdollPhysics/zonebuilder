#include "StdInc.h"
#include "Tool.h"

#define LTM_DESC
#include <tomcrypt.h>

#include <zlib/zlib.h>

void printHelp_f()
{
	Com_Printf("Commands: \n");
	Com_Printf("\tloadzone <zone> - load a zone as source\n");
	Com_Printf("\tbuildzone <zone> - build a zone\n");
	Com_Printf("\tverify <zone> - verify a zone\n");
	Com_Printf("\tunloadzones - unload all loaded zones\n");
	Com_Printf("\tlistassets <type> - list all loaded assets of a type\n");
	Com_Printf("\tdecryptzone <zone> - decrypts an iw4c zone\n");
	Com_Printf("\texec <script> - runs a batch script\n");
	Com_Printf("\thelp - prints this message\n");
	Com_Printf("\tquit - quits the program\n");
#if ZB_DEBUG
	Com_Printf("\tdump - don't even ask, this is basically a throwaway one\n");
	Com_Printf("\tdefaults - builds defaults.ff\n");
#endif
}

void loadzone_f()
{
	if (Cmd_Argc() < 2) { Com_Printf("loadzone requires at least 1 argument"); return; }

	int num = Cmd_Argc() - 1;
	if (num > 9) { Com_Printf("loadzone can't have more than 9 zones at once... ignoring rest\n"); num = 9; }
	XZoneInfo *info = new XZoneInfo[num];
	for (int i = 0; i < num; i++)
	{
		info[i].name = Cmd_Argv(i + 1);
		info[i].type1 = SOURCE_ZONE_GROUP;
		info[i].type2 = 0;
	}

	Com_LoadZones(info, num);
	Com_Printf("Loaded!\n");
}

extern bool useEntryNames;
void verify_f()
{
	if (Cmd_Argc() != 2) { Com_Printf("verify requires 1 argument, zone"); return; }

	useEntryNames = true;
	XZoneInfo info;
	info.name = Cmd_Argv(1);
	info.type1 = VERIFY_ZONE_GROUP;
	info.type2 = 0;
	Com_LoadZones(&info, 1);
	useEntryNames = false;
	Com_UnloadZones(VERIFY_ZONE_GROUP);
	Com_Printf("Verified!\n");
}

void unloadZones_f()
{
	Com_UnloadZones(SOURCE_ZONE_GROUP);
}

void buildzone_f()
{
	if (Cmd_Argc() != 2) { Com_Printf("buildzone requires 1 argument, zone"); return; }

	ZoneBuild(Cmd_Argv(1));
}

void quit_f()
{
	Com_Quit();
}

void PrintNameOfAsset(void* data, int userdata)
{
	FILE* output = (FILE*)userdata;
	fprintf(output, "%s\n", *((char**)data));
}

void listAssets_f()
{
	if (Cmd_Argc() < 2) { Com_Printf("listAssets requires 1 argument, type\n"); return; }

	Com_Printf("Listing all loaded %s assets to 'assets.txt'\n", Cmd_Argv(1));
	assetType_t type = (assetType_t)getAssetTypeForString(Cmd_Argv(1));
	if (type >= 0)
	{
		FILE* output = fopen("assets.txt", "w");
		DB_EnumXAssets(type, PrintNameOfAsset, (int)output);
		fclose(output);
	}
}

void exec_f()
{
	if (Cmd_Argc() < 2) { Com_Printf("exec requires 1 argument, script\n"); return; }
	char* buf;
	if (FS_ReadFile(Cmd_Argv(1), (void**)&buf) > 0)
	{
		Cbuf_AddText(0, buf);
		FS_FreeFile(buf);
		return;
	}
	Com_Printf("Unable to find script file \"%s\"\n", Cmd_Argv(1));
}


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

void decryptZone_f()
{
	if (Cmd_Argc() != 2) { Com_Printf("decryptzone requires 1 argument, zone"); return; }

	FILE* input = fopen(Cmd_Argv(1), "rb");
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

XZoneInfo dumpZones[] = { 
							{ "code_pre_gfx_mp", 2, 0 },
							{ "localized_code_pre_gfx_mp", 2, 0 },
							{ "code_post_gfx_mp", 2, 0 },
							{ "localized_code_post_gfx_mp", 2, 0 },
							{ "common_mp", 2, 0 },
						};

void dumpTechsetNames(void* data, int userdata)
{
	FILE* write = (FILE*)userdata;
	MaterialTechniqueSet* tech = (MaterialTechniqueSet*)data;
	fprintf(write, "techset,%s\n", tech->name);
}

#if ZB_DEBUG
void dump_f()
{

}

const char** defaultAssetNames = (const char**)0x799958;
extern char header[];
void buildDefaults_f()
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
	loadAsset(info, ASSET_TYPE_SOUND, "THIS_SHOULDNT_EXIST", "null");
	loadAsset(info, ASSET_TYPE_SNDCURVE, "THIS_SHOULDNT_EXIST", "default");
	loadAsset(info, ASSET_TYPE_LOADED_SOUND, "THIS_SHOULDNT_EXIST", "null.wav");
	loadAsset(info, ASSET_TYPE_LIGHTDEF, "THIS_SHOULDNT_EXIST", "light_dynamic");
	loadAsset(info, ASSET_TYPE_FONT, "THIS_SHOULDNT_EXIST", "fonts/consolefont");
	//loadAsset(info, ASSET_TYPE_MENUFILE, "THIS_SHOULDNT_EXIST", "ui/default.menu");
	//loadAsset(info, ASSET_TYPE_MENU, "THIS_SHOULDNT_EXIST", "default_menu");
	Localize* loc = new Localize;
	loc->name = "CGAME_UNKNOWN";
	loc->localizedString = NULL;
	addAsset(info, ASSET_TYPE_LOCALIZE, "CGAME_UNKNOWN", loc);
	loadAsset(info, ASSET_TYPE_FX, "THIS_SHOULDNT_EXIST", "misc/missing_fx");
	loadAsset(info, ASSET_TYPE_IMPACTFX, "THIS_SHOULDNT_EXIST", "default");
	loadAsset(info, ASSET_TYPE_RAWFILE, "THIS_SHOULDNT_EXIST", "codescripts/$default");
	loadAsset(info, ASSET_TYPE_STRINGTABLE, "THIS_SHOULDNT_EXIST", "mp/defaultStringTable.csv");
	loadAsset(info, ASSET_TYPE_LEADERBOARDDEF, "THIS_SHOULDNT_EXIST", "DEFAULT_LB");
	loadAsset(info, ASSET_TYPE_STRUCTUREDDATADEF, "THIS_SHOULDNT_EXIST", "mp/defaultStructuredData.def");
	loadAsset(info, ASSET_TYPE_TRACER, "THIS_SHOULDNT_EXIST", "defaulttracer");
	loadAsset(info, ASSET_TYPE_VEHICLE, "THIS_SHOULDNT_EXIST", "defaultvehicle");
	loadAsset(info, ASSET_TYPE_WEAPON, "defaultweapon_mp", "defaultweapon_mp");
	loadAsset(info, ASSET_TYPE_WEAPON, NULL, "none");
	loadAsset(info, ASSET_TYPE_RAWFILE, NULL, "mp/playeranimtypes.txt");

	Sleep(100);

	ZStream* buf = writeZone(info);

	BUFFER* compressed = buf->compressZlib();
	delete buf;

	Com_Printf("Writing to Disk...");
	CreateDirectoryA("zone", NULL);

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

void print_f()
{
	if (Cmd_Argc() != 2) { Com_Printf("print requires 1 argument, zone"); return; }
	Com_Printf("%s", Cmd_Argv(1));
}

cmd_function_t commands[32];

void ConsoleCommands_Init()
{
	printHelp_f();

	int i = 0;
	// direct console stuff
	Cmd_AddCommand("help", printHelp_f, &commands[i++], 0);
	Cmd_AddCommand("loadzone", loadzone_f, &commands[i++], 0);	
	Cmd_AddCommand("buildzone", buildzone_f, &commands[i++], 0);
	Cmd_AddCommand("unloadzones", unloadZones_f, &commands[i++], 0);
	Cmd_AddCommand("verify", verify_f, &commands[i++], 0);
	Cmd_AddCommand("decryptzone", decryptZone_f, &commands[i++], 0);
	Cmd_AddCommand("exec", exec_f, &commands[i++], 0);
	Cmd_AddCommand("listassets", listAssets_f, &commands[i++], 0);
	Cmd_AddCommand("quit", quit_f, &commands[i++], 0);

	// script stuff
	Cmd_AddCommand("print", print_f, &commands[i++], 0);
#if ZB_DEBUG
	Cmd_AddCommand("dump", dump_f, &commands[i++], 0);
	Cmd_AddCommand("defaults", buildDefaults_f, &commands[i++], 0);
#endif
}