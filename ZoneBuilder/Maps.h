#include "StdInc.h"
#pragma once

struct cBrush;
struct cPlane;
struct PhysMass;

struct Bounds
{
	vec3_t midPoint;
	vec3_t halfSize;
};

struct TriggerModel
{
	int contents;
	unsigned short hullCount;
	unsigned short firstHull;
};

struct TriggerHull
{
	Bounds bounds;
	int contents;
	unsigned short slabCount;
	unsigned short firstSlab;
};

struct TriggerSlab
{
	vec3_t dir;
	float midPoint;
	float halfSize;
};

struct MapTriggers
{
	int modelCount;
	TriggerModel* models; // sizeof 8
	int hullCount;
	TriggerHull* hulls; // sizeof 32
	int slabCount;
	TriggerSlab* slabs; // sizeof 20
};

struct Stage
{
	char * stageName;
	float offset[3];
	int flags;
};

#pragma pack(push, 1)
struct MapEnts
{
	const char* name;
	const char* entityString;
	int numEntityChars;
	MapTriggers trigger;
	Stage* stageNames;
	char stageCount;
	char pad[3];
};
#pragma pack(pop)

#pragma pack(push, 4)
struct BrushWrapper
{
	float mins[3];
	float maxs[3];
	cBrush brush;
	int totalEdgeCount;
	cPlane *planes;
};
#pragma pack(pop)

struct PhysGeomInfo
{
	BrushWrapper *brush;
	int type;
	float orientation[3][3];
	float offset[3];
	float halfLengths[3];
};

struct PhysGeomList
{
	const char * name;
	unsigned int count;
	PhysGeomInfo *geoms;
	char unknown[0x18];
	PhysMass mass;
};

struct ComPrimaryLight
{
	char type;
	char canUseShadowMap;
	char exponent;
	char unused;
	float color[3];
	float dir[3];
	float origin[3];
	float radius;
	float cosHalfFovOuter;
	float cosHalfFovInner;
	float cosHalfFovExpanded;
	float rotationLimit;
	float translationLimit;
	char* name;
};

struct ComWorld
{
	char *name;
	int isInUse;
	int lightCount;
	ComPrimaryLight* lights;
};

struct GameMap_Data
{
	void* unk1;
	int unkCount1;
	int unkCount2;
	void* unk2;
	char pad[112];
};

struct GameMap_SP
{
	const char* name;
	char pad[48];
	GameMap_Data* data;
};


struct GameMap_MP
{
	const char* name;
	GameMap_Data* data;
};