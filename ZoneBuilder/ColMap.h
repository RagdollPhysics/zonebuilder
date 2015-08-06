#include "StdInc.h"
#pragma once

struct PhysMass
{
	float centerOfMass[3];
	float momentsOfInertia[3];
	float productsOfInertia[3];
};


struct cPlane
{
	vec3_t a;
	float dist;
	int type;
};

struct cStaticModel
{
	XModel *xmodel;
	float origin[3];
	float invScaledAxis[3][3];
	float absmin[3];
	float absmax[3];
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

struct cBrush
{
	int count;
	cBrushSide * brushSide;
	char * brushEdge;
	char pad[24];
};

struct cLeaf
{
	unsigned __int16 firstCollAabbIndex;
	unsigned __int16 collAabbCount;
	int brushContents;
	int terrainContents;
	float mins[3];
	float maxs[3];
	int leafBrushNode;
};

struct cLeafBrushNodeLeaf
{
	unsigned __int16 *brushes;
};

struct cLeafBrushNodeChildren
{
	float dist;
	float range;
	unsigned __int16 childOffset[6];
};

union cLeafBrushNodeData
{
	cLeafBrushNodeLeaf leaf;
	cLeafBrushNodeChildren children;
};

struct cLeafBrushNode
{
	char axis;
	__int16 leafBrushCount;
	int contents;
	cLeafBrushNodeData data;
};

struct cModel
{
	float mins[3];
	float maxs[3];
	float radius;
	cLeaf leaf;
};

enum DynEntityType
{
	DYNENT_TYPE_INVALID = 0x0,
	DYNENT_TYPE_CLUTTER = 0x1,
	DYNENT_TYPE_DESTRUCT = 0x2,
	DYNENT_TYPE_COUNT = 0x3,
};

struct GfxPlacement
{
	float quat[4];
	float origin[3];
};

struct DynEntityDef
{
	DynEntityType type;
	GfxPlacement pose;
	XModel *xModel;
	unsigned __int16 brushModel;
	unsigned __int16 physicsBrushModel;
	FxEffectDef *destroyFx;
	PhysPreset *physPreset;
	int health;
	PhysMass mass;
	int contents;
};

struct DynEntityPose
{
	GfxPlacement pose;
	float radius;
};

struct DynEntityClient
{
	int physObjId;
	unsigned __int16 flags;
	unsigned __int16 lightingHandle;
	int health;
};

struct DynEntityColl
{
	unsigned __int16 sector;
	unsigned __int16 nextEntInSector;
	float linkMins[2];
	float linkMaxs[2];
};

struct CollisionBorder
{
	float distEq[3];
	float zBase;
	float zSlope;
	float start;
	float length;
};

struct CollisionPartition
{
	char triCount;
	char borderCount;
	int firstTri;
	CollisionBorder *borders;
};

union CollisionAabbTreeIndex
{
	int firstChildIndex;
	int partitionIndex;
};

struct CollisionAabbTree
{
	float origin[3];
	float halfSize[3];
	unsigned __int16 materialIndex;
	unsigned __int16 childCount;
	CollisionAabbTreeIndex u;
};

struct clipMap_t
{
	const char* name;
	int unknown1; // +8
	int numCPlanes; // +8
	cPlane* cPlanes; // sizeof 20, +12
	int numStaticModels; // +16
	cStaticModel* staticModelList; // sizeof 76, +20
	int numMaterials; // +24
	dMaterial* materials; // sizeof 12 with a string (possibly name?), +28
	int numCBrushSides; // +32
	cBrushSide* cBrushSides; // sizeof 8, +36
	int numCBrushEdges; // +40
	char* cBrushEdges; // +44
	int numCNodes; // +48
	cNode * cNodes; // sizeof 8, +52
	int numCLeaf; // +56
	cLeaf* cLeaf; // +60
	int numCLeafBrushNodes; // +64
	cLeafBrushNode* cLeafBrushNodes; // +68
	int numLeafBrushes; // +72
	short* leafBrushes; // +76
	int numLeafSurfaces; // +80
	int* leafSurfaces; // +84
	int numVerts; // +88
	vec3_t* verts; // +92
	int numTriIndices; // +96
	short* triIndices; // +100
	bool* triEdgeIsWalkable; // +104
	int numCollisionBorders; // +108
	CollisionBorder* collisionBorders;// sizeof 28, +112
	int numCollisionPartitions; // +116
	CollisionPartition* collisionPartitions; // sizeof 12, +120
	int numCollisionAABBTrees; // +124
	CollisionAabbTree* collisionAABBTrees;// sizeof 32, +128
	int numCModels; // +132
	cModel* cModels; // sizeof 68, +136
	short numCBrushes; // +140
	short pad2; // +142
	cBrush * cBrushes; // sizeof 36, +144
	void* unknown2; // same count as cBrushes, +148
	int * unknown3; // same count as cBrushes, +152
	MapEnts * mapEnts; // +156
	int unkCount4; // +160
	void* unknown4; // +164
	unsigned __int16 dynEntCount[2];
	DynEntityDef *dynEntDefList[2];
	DynEntityPose *dynEntPoseList[2];
	DynEntityClient *dynEntClientList[2];
	DynEntityColl *dynEntCollList[2];
	unsigned int checksum;
	char unknown5[0x30];
}; // +256