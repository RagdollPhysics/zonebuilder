#pragma once

#include "StdInc.h"
#include "Buffers.h"
#include "CSVFile.h"
#include "WriteMacros.h"

#define MAX_ASSET_COUNT 4096
#define MAX_SCRIPT_STRINGS 2048

#define ALIGN_TO_1 0
#define ALIGN_TO_2 1
#define ALIGN_TO_4 3
#define ALIGN_TO_8 7
#define ALIGN_TO_16 15

#if ZB_DEBUG
#define Com_Debug(fmt, ...) Com_Debug_(false, fmt, __VA_ARGS__)
#define Com_Debug_logOnly(fmt, ...) Com_Debug_(true, fmt, __VA_ARGS__)
#else
#define Com_Debug(fmt, ...)
#define Com_Debug_logOnly(fmt, ...)
#endif

#define Com_Printf(fmt, ...) Com_Printf_(false, fmt, __VA_ARGS__)
#define Com_Printf_logOnly(fmt, ...) Com_Printf_(true, fmt, __VA_ARGS__)

typedef struct
{
	int name;
	int type;	
	void* data;
	int offset;
	bool written;
#if ZB_DEBUG
	std::string debugName;
	bool verified;
#endif
} asset_t;

typedef struct
{
	char* name;
	int scriptStringCount;
	string * scriptStrings;
	int assetCount;
	asset_t * assets;
	int index_start;
} zoneInfo_t;

// sscanline ish
#define sscanlinef_init(ptr) int __sscanlinef_offset = 0; char* __sscanlinef_at = ptr;
#define _sscanlinef_inc() __sscanlinef_at += __sscanlinef_offset;

#define _sscanlinef(format, ...) sscanf(__sscanlinef_at, format "%n", __VA_ARGS__, &__sscanlinef_offset)
#define sscanlinef(format, ...) _sscanlinef(format, __VA_ARGS__); _sscanlinef_inc();

// Main
extern void loadAsset(zoneInfo_t* info, int type, const char* filename, const char* name);
extern void ZoneBuild(char* toBuild);;

// Util
extern void Com_Printf_(bool logOnly, const char* format, ...);
extern void Com_Error(bool exit, const char* format, ...);
extern void Com_Debug_(bool logOnly, const char* format, ...);
extern void Com_Quit();
extern int getAssetTypeForString(const char* str);
extern const char* getAssetStringForType(int type);
extern const char* getAssetName(int type, void* data);
extern void setAssetName(int type, void* data, const char* name);
extern int getArgc();
extern LPSTR* getArgs();
extern long flength(FILE* fp);
extern void debugChecks();
extern void Com_LoadZones(XZoneInfo* zones, int count);
extern void Com_UnloadZones(int group);
extern char *Info_ValueForKey(const char *s, const char *key);

// console
extern void InitConsole();
extern void DestroyConsole();
extern void RunConsole();

// args
extern char* fs_basegame;
extern char* cfg_name;
extern string zoneToBuild;
extern bool build, verify, console;
extern list<string> sources;
extern void parseArgs();

// ZoneWriter
//extern void writeZone(zoneInfo_t * info);
extern ZStream* writeZone(zoneInfo_t * info);
extern int writeAsset(zoneInfo_t* info, asset_t* asset, ZStream* buf);
extern int requireAsset(zoneInfo_t* info, int type, const char* name, ZStream* buf);

// ZoneData
extern zoneInfo_t * getZoneInfo(const char * zoneName);
extern void freeZoneInfo(zoneInfo_t* info);
extern unsigned int R_HashString(const char* string);
extern int containsAsset(zoneInfo_t* info, int type, const char* name);
extern int addAsset(zoneInfo_t* info, int type, const char* name, void* data);
extern int addScriptString(zoneInfo_t* info, string str);
extern int addScriptString(zoneInfo_t* info, char* str);
extern void doLastAsset(zoneInfo_t* info, const char* name);
extern void* getAsset(zoneInfo_t* info, int type, const char* name);
extern void* findAssetEverywhere(zoneInfo_t* info, int type, const char* name);
#if ZB_DEBUG
void verifyAsset(zoneInfo_t* info, int type, const char* name);
asset_t* nextUnverifiedAsset(zoneInfo_t* info);
#endif

// XAnimData
extern void * addXAnim(zoneInfo_t* info, const char* name, char* data, int dataLen);
extern void writeXAnim(zoneInfo_t* info, ZStream* buf, XAnim* data);

// RawfileData
extern void * addRawfile(zoneInfo_t* info, const char* name, char* data, int dataLen);
extern void writeRawfile(zoneInfo_t* info, ZStream* buf, Rawfile* data);

// XModelData
extern void * addXModel(zoneInfo_t* info, const char* name, char* data, int dataLen);
extern void writeXModel(zoneInfo_t* info, ZStream* buf, XModel* data);

// MaterialData
extern void * addMaterial(zoneInfo_t* info, const char* name, char* data, int dataLen);
extern void* addGfxImage(zoneInfo_t* info, const char* name, char* data, int dataLen);
extern void writeMaterial(zoneInfo_t* info, ZStream* buf, Material* data);
extern void writeGfxImage(zoneInfo_t* info, ZStream* buf, GfxImage* data);


// TechsetData
extern void* addTechset(zoneInfo_t* info, const char* name, char* data, int dataLen);
extern void* addPixelShader(zoneInfo_t* info, const char* name, char* data, int dataLen);
extern void* addVertexShader(zoneInfo_t* info, const char* name, char* data, int dataLen);
extern void* addVertexDecl(zoneInfo_t* info, const char* name, char* data, int dataLen);
extern void writeTechset(zoneInfo_t* info, ZStream* buf, MaterialTechniqueSet* data);
extern void writePixelShader(zoneInfo_t* info, ZStream* buf, PixelShader* data);
extern void writeVertexShader(zoneInfo_t* info, ZStream* buf, VertexShader* data);
extern void writeVertexDecl(zoneInfo_t* info, ZStream* buf, VertexDecl* data);

// ColMapData
//extern void * addColMap(zoneInfo_t* info, const char* name, char* data, int dataLen);
//extern void writeColMap(zoneInfo_t* info, ZStream* buf, ColMap* data);

// MapEntsData
extern void * addMapEnts(zoneInfo_t* info, const char* name, char* data, int dataLen);
extern void writeMapEnts(zoneInfo_t* info, ZStream* buf, MapEnts* data);

// ComWorldData
//void writeComWorld(zoneInfo_t* info, ZStream* buf, ComWorld* data);
//void * addComWorld(zoneInfo_t* info, const char* name, char* data, int dataLen);

// GameMapData
void writeGameMap(zoneInfo_t* info, ZStream* buf, GameMap_MP* data);
void * addGameMap_MP(zoneInfo_t* info, const char* name, char* data, int dataLen);
void * addGameMap_SP(zoneInfo_t* info, const char* name, char* data, int dataLen);

// StringTableData
void writeStringTable(zoneInfo_t* info, ZStream* buf, StringTable* data);
void * addStringTable(zoneInfo_t* info, const char* name, char* data, int dataLen);

// SoundAliasData
void writeSoundAlias(zoneInfo_t* info, ZStream* buf, SoundAliasList* data);
void * addSoundAlias(zoneInfo_t* info, const char* name, char* data, int dataLen);
void writeSndCurve(zoneInfo_t* info, ZStream* buf, SndCurve* data);
void * addSndCurve(zoneInfo_t* info, const char* name, char* data, int dataLen);
void writeLoadedSound(zoneInfo_t* info, ZStream* buf, LoadedSound* data);
void * addLoadedSound(zoneInfo_t* info, const char* name, char* data, int dataLen);

// FxEffectDefData
void writeFxEffectDef(zoneInfo_t* info, ZStream* buf, FxEffectDef* data);
void * addFxEffectDef(zoneInfo_t* info, const char* name, char* data, int dataLen);

// PhysPresetData
void writePhysPreset(zoneInfo_t* info, ZStream* buf, PhysPreset* data);
void* addPhysPreset(zoneInfo_t* info, const char* name, char* data, int dataLen);

// PhysCollMapData
void writePhysCollmap(zoneInfo_t* info, ZStream* buf, PhysGeomList* data);
void * addPhysCollmap(zoneInfo_t* info, const char* name, char* data, int dataLen);

// WeaponData
void writeWeaponVariantDef(zoneInfo_t* info, ZStream* buf, WeaponVariantDef* data);
void * addWeaponVariantDef(zoneInfo_t* info, const char* name, char* data, int dataLen);

// TracerData
void writeTracer(zoneInfo_t* info, ZStream* buf, Tracer* data);
void * addTracer(zoneInfo_t* info, const char* name, char* data, int dataLen);

// LocalizeData
void writeLocalize(zoneInfo_t* info, ZStream* buf, Localize* data);
void * addLocalize(zoneInfo_t* info, const char* name, char* data, int dataLen);

// FontData
void writeFont(zoneInfo_t* info, ZStream* buf, Font* data);
void * addFont(zoneInfo_t* info, const char* name, char* data, int dataLen);

// GfxLightDefData
void writeGfxLightDef(zoneInfo_t* info, ZStream* buf, GfxLightDef* data);
void * addGfxLightDef(zoneInfo_t* info, const char* name, char* data, int dataLen);

// LeaderboardDef
void writeLeaderboardDef(zoneInfo_t* info, ZStream* buf, LeaderboardDef* data);
void * addLeaderboardDef(zoneInfo_t* info, const char* name, char* data, int dataLen);

// FxImpactTableData
void writeFxImpactTable(zoneInfo_t* info, ZStream* buf, FxImpactTable* data);
void * addFxImpactTable(zoneInfo_t* info, const char* name, char* data, int dataLen);

// MenuData
extern void writeMenuList(zoneInfo_t* info, ZStream* buf, MenuList* data);
extern void * addMenuList(zoneInfo_t* info, const char* name, char* data, int dataLen);

// StructuredDataDefData
void writeStructuredDataDefSet(zoneInfo_t* info, ZStream* buf, StructuredDataDefSet* data);
extern void * addStructuredDataDefSet(zoneInfo_t* info, const char* name, char* data, int dataLen);

// VehicleData
void writeVehicleDef(zoneInfo_t* info, ZStream* buf, VehicleDef* data);
void * addVehicleDef(zoneInfo_t* info, const char* name, char* data, int dataLen);