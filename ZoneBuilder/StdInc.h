#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <stdio.h>
#include <string>
#include <map>
#include <io.h>
#include <fcntl.h>
#include <zlib\zlib.h>
using namespace std;

#include "Buffers.h"
#include "AssetStructs.h"

#define MAX_ASSET_COUNT 2048
#define MAX_SCRIPT_STRINGS 2048

#if _DEBUG
#define Com_Debug Com_Debug_
#endif

typedef enum assetType_e
{
	ASSET_TYPE_PHYSPRESET = 0,
	ASSET_TYPE_PHYS_COLLMAP = 1,
	ASSET_TYPE_XANIM = 2,
	ASSET_TYPE_XMODELSURFS = 3,
	ASSET_TYPE_XMODEL = 4,
	ASSET_TYPE_MATERIAL = 5,
	ASSET_TYPE_PIXELSHADER = 6,
	ASSET_TYPE_VERTEXSHADER = 7,
	ASSET_TYPE_VERTEXDECL = 8,
	ASSET_TYPE_TECHSET = 9,
	ASSET_TYPE_IMAGE = 10,
	ASSET_TYPE_SOUND = 11,
	ASSET_TYPE_SNDCURVE = 12,
	ASSET_TYPE_LOADED_SOUND = 13,
	ASSET_TYPE_COL_MAP_SP = 14,
	ASSET_TYPE_COL_MAP_MP = 15,
	ASSET_TYPE_COM_MAP = 16,
	ASSET_TYPE_GAME_MAP_SP = 17,
	ASSET_TYPE_GAME_MAP_MP = 18,
	ASSET_TYPE_MAP_ENTS = 19,
	ASSET_TYPE_FX_MAP = 20,
	ASSET_TYPE_GFX_MAP = 21,
	ASSET_TYPE_LIGHTDEF = 22,
	ASSET_TYPE_UI_MAP = 23,
	ASSET_TYPE_FONT = 24,
	ASSET_TYPE_MENUFILE = 25,
	ASSET_TYPE_MENU = 26,
	ASSET_TYPE_LOCALIZE = 27,
	ASSET_TYPE_WEAPON = 28,
	ASSET_TYPE_SNDDRIVERGLOBALS = 29,
	ASSET_TYPE_FX = 30,
	ASSET_TYPE_IMPACTFX = 31,
	ASSET_TYPE_AITYPE = 32,
	ASSET_TYPE_MPTYPE = 33,
	ASSET_TYPE_CHARACTER = 34,
	ASSET_TYPE_XMODELALIAS = 35,
	ASSET_TYPE_RAWFILE = 36,
	ASSET_TYPE_STRINGTABLE = 37,
	ASSET_TYPE_LEADERBOARDDEF = 38,
	ASSET_TYPE_STRUCTUREDDATADEF = 39,
	ASSET_TYPE_TRACER = 40,
	ASSET_TYPE_VEHICLE = 41,
	ASSET_TYPE_ADDON_MAP_ENTS = 42,
	ASSET_TYPE_MAX = 43
} assetType_t;

typedef struct
{
	int name;
	int type;	
	char* data;
	int length;
	int offset;
	map<int, int> * fixups; // offset, assetNumber
} asset_t;

typedef struct
{
	int scriptStringCount;
	string * scriptStrings;
	int assetCount;
	asset_t * assets;
} zoneInfo_t;

// sscanline ish
#define sscanlinef_init(ptr) int __sscanlinef_offset = 0; char* __sscanlinef_at = ptr;
#define _sscanlinef_inc() __sscanlinef_at += __sscanlinef_offset;

#define _sscanlinef(format, ...) sscanf(__sscanlinef_at, format "%n", __VA_ARGS__, &__sscanlinef_offset)
#define sscanlinef(format, ...) _sscanlinef(format, __VA_ARGS__); _sscanlinef_inc();

// Main
extern void loadAsset(zoneInfo_t* info, int type, const char* filename, const char* name);

// Util
extern void Com_Printf(const char* format, ...);
void Com_Error(bool exit, const char* format, ...);
void Com_Debug_(const char* format, ...);
int getAssetTypeForString(const char* str);
const char* getAssetStringForType(int type);

// ZoneWriter
extern BUFFER* writeZone(zoneInfo_t * info);

// ZoneData
extern zoneInfo_t * getZoneInfo();
extern void freeZoneInfo(zoneInfo_t* info);
extern unsigned int R_HashString(const char* string);
extern int containsAsset(zoneInfo_t* info, int type, const char* name);
extern int addAsset(zoneInfo_t* info, int type, const char* name, char* data, size_t dataLen);
extern void setAssetData(zoneInfo_t * info, int asset, char* data, size_t dataLen);
extern void addFixup(zoneInfo_t* info, int asset, int offset, int toAsset);
extern int addScriptString(zoneInfo_t* info, string str);
extern int addScriptString(zoneInfo_t* info, char* str);
extern void doLastAsset(zoneInfo_t* info, const char* name);
extern void addXAnim(zoneInfo_t* info, const char* name, char* data, size_t dataLen);
extern void addRawfile(zoneInfo_t* info, const char* name, char* data, size_t dataLen);

// MaterialData
extern void addMaterial(zoneInfo_t* info, const char* name, char* data, size_t dataLen);

// TechsetData
extern void addVertexShader(zoneInfo_t* info, const char* name, char* data, size_t dataLen);
extern void addPixelShader(zoneInfo_t* info, const char* name, char* data, size_t dataLen);
extern void addVertexDecl(zoneInfo_t* info, const char* name, char* data, size_t dataLen);
extern void addTechset(zoneInfo_t* info, const char* name, char* data, size_t dataLen);
