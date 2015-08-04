#include "StdInc.h"
#pragma once

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
	const char* name;
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
	Stage* stages;
	char stageCount;
	char pad[3];
};
#pragma pack(pop)

struct cPlane
{
	vec3_t a;
	float dist;
	int type;
};

struct dMaterial
{
	char* name;
	int unk;
	int unk2;
};

struct cNode
{
	cPlane* plane;
	short children[2];
};

struct cBrushSide
{
	cPlane* side;
	short texInfo, dispInfo;
};

struct cModel
{
	vec3_t loc;
	float rotX, rotY, rotZ, rotW;
	char pad[40];
};

struct cBrush
{
	int count;
	cBrushSide * brushSide;
	char * brushEdge;
	char pad[24];
};

struct cLeafBrushNode
{
	short unk;
	short numBrushes;
	char pad[4];
	short* leaf;
	char pad2[8];
};

struct CollisionMap
{
	const char* name;
	int pad; // +8
	int numCPlanes; // +8
	cPlane* cPlanes; // sizeof 20, +12
	int numStaticModels; // +16
	char* staticModelList; // sizeof 76, +20
	int numMaterials; // +24
	dMaterial* materials; // sizeof 12 with a string (possibly name?), +28
	int numCBrushSides; // +32
	cBrushSide* cBrushSides; // sizeof 8, +36
	int numCBrushEdges; // +40
	char* cBrushEdges; // +44
	int numCNodes; // +48
	cNode * cNodes; // sizeof 8, +52
	int numCLeaf; // +56
	void* cLeaf; // +60
	int numCLeafBrushNodes; // +64
	cLeafBrushNode* cLeafBrushNodes; // +68
	int numCLeafBrushes; // +72
	void* cLeafBrushes; // +76
	int unkCount1; // +80
	int* unknown1; // +84
	int numVerts; // +88
	vec3_t* verts; // +92
	int numTriIndicies; // +96
	short* triIndicies; // +100
	bool* triEdgeIsWalkable; // +104
	int numCollisionBorders; // +108
	void* collisionBorders;// sizeof 28, +112
	int numCollisionPartitions; // +116
	void* collisionPartitions; // sizeof 12, +120
	int numCollisionAABBTrees; // +124
	void* collisionAABBTrees;// sizeof 32, +128
	int numCModels; // +132
	void* cModels; // sizeof 68, +136
	short numCBrushes; // +140
	short pad2; // +142
	cBrush * cBrushes; // sizeof 36, +144
	void* unknown5; // same count as cBrushes, +148
	int * unkInt; // same count as cBrushes, +152
	MapEnts * mapEnts; // +156
	int unkCount6; // +160
	void* unknown6; // +164
	short numDynEntities_Model; // +168
	short numDynEntities_Brush; // +170
	void* dynEntityDef_Model; // sizeof 92, +172
	void* dynEntityDef_Brush; // +176
	void* dynEntityPose_Model; // sizeof 32, +180
	void* dynEntityPose_Brush; // +184
	void* dynEntityClient_Model; // sizeof 12, +188
	void* dynEntityClient_Brush; // +192
	void* dynEntityColl_Model; // sizeof 20, +196
	void* dynEntityColl_Brush; // +200
	char pad3[52]; // +204
}; // +256


struct PhysMass  //NOTE: PhysMass is used in DynEntityDef's in the collision map.
{
	float centerOfMass[3];
	float momentsOfInertia[3];
	float productsOfInertia[3];
};

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