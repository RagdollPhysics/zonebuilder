#pragma once

#include "StdInc.h"
#include "Buffers.h"

#define MAX_ASSET_COUNT 2048
#define MAX_SCRIPT_STRINGS 2048

#define ZONE_STREAM_VERTEX 8
#define ZONE_STREAM_FACE 9

#if _DEBUG
#define Com_Debug Com_Debug_
#else
#define Com_Debug
#endif

typedef struct
{
	int name;
	int type;	
	void* data;
	int offset;
	bool written;
} asset_t;

typedef struct
{
	char* name;
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
int getArgc();
LPSTR* getArgs();

// ZoneWriter
//extern void writeZone(zoneInfo_t * info);
extern BUFFER* writeZone(zoneInfo_t * info);
extern int writeAsset(zoneInfo_t* info, asset_t* asset, BUFFER* buf);
extern int requireAsset(zoneInfo_t* info, int type, char* name, BUFFER* buf);
extern void addXZoneMemory(int index, int num);

// ZoneData
extern zoneInfo_t * getZoneInfo(const char * zoneName);
extern void freeZoneInfo(zoneInfo_t* info);
extern unsigned int R_HashString(const char* string);
extern int containsAsset(zoneInfo_t* info, int type, const char* name);
extern int addAsset(zoneInfo_t* info, int type, const char* name, void* data);
extern int addScriptString(zoneInfo_t* info, string str);
extern int addScriptString(zoneInfo_t* info, char* str);
extern void doLastAsset(zoneInfo_t* info, const char* name);

// XAnimData
extern void * addXAnim(zoneInfo_t* info, const char* name, char* data, size_t dataLen);
extern void writeXAnim(zoneInfo_t* info, BUFFER* buf, XAnim* data);

// RawfileData
extern void * addRawfile(zoneInfo_t* info, const char* name, char* data, size_t dataLen);
extern void writeRawfile(zoneInfo_t* info, BUFFER* buf, Rawfile* data);

// XModelData
extern void * addXModel(zoneInfo_t* info, const char* name, char* data, size_t dataLen);
extern void writeXModel(zoneInfo_t* info, BUFFER* buf, XModel* data);

// MaterialData
extern void * addMaterial(zoneInfo_t* info, const char* name, char* data, size_t dataLen);
extern void writeMaterial(zoneInfo_t* info, BUFFER* buf, Material* data);

// TechsetData
extern void * addTechset(zoneInfo_t* info, const char* name, char* data, size_t dataLen);
extern void writeTechset(zoneInfo_t* info, BUFFER* buf, MaterialTechniqueSet* data);
extern void writePixelShader(zoneInfo_t* info, BUFFER* buf, PixelShader* data);
extern void writeVertexShader(zoneInfo_t* info, BUFFER* buf, VertexShader* data);
extern void writeVertexDecl(zoneInfo_t* info, BUFFER* buf, VertexDecl* data);

// ColMapData
extern void * addColMap(zoneInfo_t* info, const char* name, char* data, size_t dataLen);
extern void writeColMap(zoneInfo_t* info, BUFFER* buf, Col_Map* data);

// MapEntsData
extern void * addMapEnts(zoneInfo_t* info, const char* name, char* data, size_t dataLen);
extern void writeMapEnts(zoneInfo_t* info, BUFFER* buf, MapEnts* data);

// ComWorldData
void writeComWorld(zoneInfo_t* info, BUFFER* buf, ComWorld* data);
void * addComWorld(zoneInfo_t* info, const char* name, char* data, size_t dataLen);

// GameMapData
void writeGameMap(zoneInfo_t* info, BUFFER* buf, GameMap_MP* data);
void * addGameMap_MP(zoneInfo_t* info, const char* name, char* data, size_t dataLen);
void * addGameMap_SP(zoneInfo_t* info, const char* name, char* data, size_t dataLen);