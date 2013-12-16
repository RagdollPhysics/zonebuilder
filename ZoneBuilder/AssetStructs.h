#include "StdInc.h"
#pragma once

typedef struct 
{
	const char *name;
	int type;
	float mass;
	float bounce;
	float friction;
	float bulletForceScale;
	float explosiveForceScale;
	const char *sndAliasPrefix;
	float piecesSpreadFraction;
	float piecesUpwardVelocity;
	bool tempDefaultToCylinder;
} PhysPreset;

typedef struct
{
	const char* name;
	char pad[68];
} PhysCollmap;

typedef struct
{
	int name; // use DB_GetFFString
	float frame; // something like that
} Notetrack;

struct XAnimDeltaTransData
{
	char pad[24];
	char* unk;
}; // more data follows struct

struct XAnimDeltaTrans
{
	short deltaBase;
	short deltaBase2;
	union
	{
		float rawData[3];
		XAnimDeltaTransData data;
	};
};

struct XAnimDeltaQuatData
{
	char* data;
}; // more data follows end of struct

struct XAnimDeltaQuat
{
	short deltaBase;
	short deltaBase2;
	union
	{
		int rawData;
		XAnimDeltaQuatData data;
	};
};

struct XAnimDeltaUnkData
{
	char* data;
}; // more data follows end of struct, as usual

struct XAnimDeltaUnk
{
	short deltaBase;
	short deltaBase2;
	union
	{
		__int64 rawData;
		XAnimDeltaUnkData data;
	};
};

struct XAnimDeltaPart
{
	XAnimDeltaTrans* trans;
	XAnimDeltaQuat* quat;
	XAnimDeltaUnk* unk;
};


typedef struct 
{
	char * name; // 0
	short dataByteCount; // 4
	short dataShortCount; // 6
	short dataIntCount; // 8
	short randomDataByteCount; // 10 - 0xA
	short randomDataIntCount;// 12 - 0xC
	short framecount; // 14 - 0xE
	short flags; // 16 - 0x10
	char unknown1[8];
	char tagCount;
	char notetrackCount;
	short unknown2;
	short unknown3; // 30 - 0x1E
	int randomDataShortCount; // 32 - 0x20
	short indexcount; // 36 - 0x24
	short unknown4; // 38 - 0x26
	float framerate; // 40 - 0x28
	float frequency; // 44 - 0x2C
	short * tagnames; // 48 - 0x30
	char *dataByte;// 52 - 0x34
	short *dataShort; // 56 - 0x38
	int *dataInt; // 60 - 0x3C
	short *randomDataShort; // 64 - 0x40
	char *randomDataByte; // 68 - 0x44
	int *randomDataInt; // 72 - 0x48
	char* indicies; // 76 - 0x4C
	Notetrack* notetracks; // 80 - 0x50
	XAnimDeltaPart * delta; // 84 - 0x54
	// 88 - 0x58
} XAnim;

// material stuff
struct GfxImageLoadDef // actually a IDirect3DTexture* but this is easier
{
	char mipLevels;
	char pad[7];
	int format; // usually the compression Magic
	int dataSize; // set to zero to load from IWD
	//char * data; 
};
struct GfxImage
{
	GfxImageLoadDef * /*Direct3DTexture9**/ texture;
	char textureType; // 5 is cube, 4 is 3d, 3 is 2d
	char a3;
	char textureType2;
	char unknown3;
	char unknown4;
	char unknown5;
	char unknown6;
	char a4;
	int dataLen1;
	int dataLen2;
	short height;
	short width;
	short depth;
	char loaded;
	char pad;
	char* name;
};

struct MaterialTextureDef
{
	unsigned int typeHash; // asset hash of type
	char firstCharacter; // first character of image name
	char secondLastCharacter; // second-last character of image name (maybe only in CoD4?!)
	unsigned char textureType; // maybe 0xE2
	char unknown2; // likely 0x00
	GfxImage* image; // GfxImage* actually
};

struct VertexDecl
{
	const char* name;
	int unknown;
	char pad[28];
	/*IDirect3DVertexDeclaration9**/void* declarations[16];
};

struct PixelShader
{
	const char* name;
	/*IDirect3DPixelShader9*/void* shader;
	DWORD* bytecode;
	int codeLen;
};

struct VertexShader
{
	const char* name;
	void * /*IDirect3DVertexShader9**/ shader;
	DWORD* bytecode;
	int codeLen;
};

struct ShaderArgumentDef
{
	short type;
	short dest;
	short paramID;
	short more;
};

struct MaterialPass
{
	VertexDecl* vertexDecl;
	VertexShader* vertexShader;
	PixelShader* pixelShader;
	char argCount1;
	char argCount2;
	char argCount3;
	char unk;
	ShaderArgumentDef* argumentDef;
};

struct MaterialTechnique
{
	char* name;
	short pad2;
	short numPasses;
	MaterialPass passes[1];
};

struct MaterialTechniqueSet
{
	const char* name;
	char pad[4];
	MaterialTechniqueSet* remappedTechniques;
	MaterialTechnique* techniques[48];
};

struct Material
{
	const char* name;
	unsigned short flags; // 0x2F00 for instance
	unsigned char animationX; // amount of animation frames in X
	unsigned char animationY; // amount of animation frames in Y
	char unknown1[4]; // 0x00
	unsigned int rendererIndex; // only for 3D models
	char unknown9[8];
	unsigned int unknown2; // 0xFFFFFFFF
	unsigned int unknown3; // 0xFFFFFF00
	char unknown4[40]; // 0xFF
	char numMaps; // 0x01, possibly 'map count' (zone code confirms)
	char unknown5; // 0x00
	char stateMapCount; // 0x01, maybe map count actually
	char unknown6; // 0x03
	unsigned int unknown7; // 0x04
	MaterialTechniqueSet* techniqueSet; // '2d' techset; +80
	MaterialTextureDef* maps; // map references
	unsigned int unknown8;
	void* stateMap; // might be NULL, need to test
};

struct XModelAngle
{
	short x;
	short y;
	short z;
	short base; // defines the 90-degree point for the shorts
};

struct XModelTagPos
{
	float x;
	float y;
	float z;
};

struct XSurfaceCTEntry
{
	char pad[24];
	int numNode;
	char* node; // el size 16
	int numLeaf;
	short* leaf;
};

struct XSurfaceCT
{
	int pad;
	int pad2;
	XSurfaceCTEntry* entry;
};

struct GfxPackedVertex
{
	float x;
	float y;
	float z;
	float unk; // either 1.0f or -1.0f
	DWORD color;
	DWORD texCoords;
	DWORD normal;
	DWORD unk2;
};

struct Face
{
	short v1;
	short v2;
	short v3;
};

struct XSurface
{
	short pad; // +0
	short numVertices; // +2
	short numPrimitives; // +4
	unsigned char streamHandle; // something to do with buffers, +6
	char pad2; // +7
	int pad3; // +8
	Face* indexBuffer; // +12
	short blendNum1; // +16
	short blendNum2; // +18
	short blendNum3; // +20
	short blendNum4; // +22
	char* blendInfo; // +24
	void* vertexBuffer; // +28
	int numCT; // +32
	XSurfaceCT* ct; // +36
	char pad5[24]; // +40
	// pad5 matches XModelSurfaces pad
	// total size, 64
};

typedef struct
{
	const char* name; // +0
	XSurface* surfaces; // +4
	int numSurfaces; // +8
	char pad[24]; // +12, matches XSurface pad5
} XModelSurfaces; // total size, 36

struct XSurfaceLod
{
	char pad[4];
	short numSurfs;
	short pad2;
	XModelSurfaces* surfaces;
	char pad3[32];
};

struct XColSurf
{
	void* tris; // +0, sizeof 48
	int count; // +4
	char pad[36]; // +8
}; // +44

typedef struct XModel
{
	char* name; // +0
	char numBones; // +4
	char numSubBones; // +5
	char numSurfaces; // +6
	char pad2; // +7
	char pad3[28]; // +8
	short* boneNames; // +36
	char* boneUnknown1; // +40
	XModelAngle* tagAngles; // +44, element size 8
	XModelTagPos* tagPositions; // +48, element size 12
	char* boneUnknown4; // +52
	char* animMatrix; // +56, element size 32
	Material** materials; // +60
	XSurfaceLod lods[4]; // +64
	int pad4; // +240
	XColSurf* colSurf; // +244
	int numColSurfs; // +248
	int pad6;
	void* unknowns; // bone count, +256, element size 28
	char pad5[36];
	void* physPreset;
	void* physCollmap;
} XModel; // total size 304

typedef struct  
{
	char type;
	char pad[3];
	const char* folder;
	const char* file;
} StreamFile;

typedef struct
{
	const char* name;
	char pad[132];
} sndcurve_t;

typedef struct
{
	char* name;
	char pad[4];
	int dataLength;
	char pad2[24];
	void* soundData;
} mss_sound_t;

typedef struct  
{
	char* name; // +0
	char* unkName1; // +4
	char* unkName2; // +8
	char* unkName3; // +12
	char* unkName4; // +16
	StreamFile* stream; // +20
	char pad2[72];// + 24
	void* speakerMap; // +96, sizeof 408
} snd_alias_t;

typedef struct  
{
	const char* name;
	snd_alias_t* aliases;
	int numAliases;
} snd_alias_list_t;

struct MapEnts_Brushes
{
	int planeCount;
	short* planes;
	int vertexCount;
	void* verticies;
	int unkCount;
	void* unk;
};

typedef struct MapEnts_s
{
	const char* name;
	const char* entitystring;
	MapEnts_Brushes brushes;
	char** stageNames;
	int stageCount;
} MapEnts;

typedef struct
{
	const char* name;
	int pad; // +8
	int numCPlanes; // +8
	void* cPlanes; // sizeof 20, +12
	int numStaticModels; // +16
	void* staticModelList; // sizeof 76, +20
	int numMaterials; // +24
	void* materials; // sizeof 12 with a string (possibly name?), +28
	int numCBrushSides; // +32
	void* cBrushSides; // sizeof 8, +36
	int numCBrushEdges; // +40
	char* cBrushEdges; // +44
	int numCNodes; // +48
	void * cNodes; // sizeof 8, +52
	int numCLeaf; // +56
	void* cLeaf; // +60
	int numCLeafBrushNodes; // +64
	void* cLeafBrushNodes; // +68
	int numCLeafBrushes; // +72
	void* cLeafBrushes; // +76
	int unkCount1; // +80
	int* unknown1; // +84
	int numVerts; // +88
	Vector3* verts; // +92
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
	void * cBrushes; // sizeof 36, +144
	void* unknown5; // same count as cBrushes, +148
	int * unkInt; // same count as cBrushes, +152
	//void* mapEnts;
	MapEnts * mapEns; // +156
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
} Col_Map; // +256

struct com_map_entry
{
    char data[0x44];
};

struct ComMap
{
    char *name;
    int version;
    int entryCount;
    com_map_entry* entries;
};

typedef struct
{
	const char* name;
	char pad[52];
} game_map_sp_t;

typedef struct
{
	const char* name;
	char pad[4];
} game_map_mp_t;

typedef struct
{
	const char* name;
	char pad[114];
} fx_map_t;

typedef struct
{
	const char* name;
	char pad[624];
} gfx_map_t;

typedef struct
{
	const char* name;
	char pad[14];
} lightdef_t;

typedef struct
{
	unsigned short character;
	unsigned char padLeft;
	unsigned char padTop;
	unsigned char padRight;
	unsigned char width;
	unsigned char height;
	unsigned char const0;
	float uvLeft;
	float uvTop;
	float uvRight;
	float uvBottom;
} fontEntry_t;

typedef struct 
{
	char* name;
	int size;
	int entries;
	Material* image;
	Material* glowImage;
	fontEntry_t* characters;
} font_t;

// we will leave menus out of this as they are compicated as hell

typedef struct 
{
	char * localizedString;
	char * name;
	char * unk; // dont even know if this exists
	// its supposedly 0xC long but the rest is never referenced
} localize_t;

typedef struct
{
	const char* name;
	char pad[1664];
} WeaponDef;

enum animNames_e
{
	ANIM_NONE,
	ANIM_IDLE,
	ANIM_EMPTYIDLE,
	ANIM_FIRE,
	ANIM_4,
	ANIM_LASTSHOT,
	ANIM_6,
	ANIM_MELEE,
	ANIM_MELEE_CHARGE,
	ANIM_RELOAD,
	ANIM_RELOAD_EMPTY,
	ANIM_11,
	ANIM_12,
	ANIM_RAISE,
	ANIM_FIRST_RAISE,
	ANIM_15,
	ANIM_DROP,
	ANIM_17,
	ANIM_18,
	ANIM_QUICK_RAISE,
	ANIM_QUICK_DROP,
	ANIM_EMPTY_RAISE,
	ANIM_EMPTY_DROP,
	ANIM_SPRINT_IN,
	ANIM_SPRINT_LOOP,
	ANIM_SPRINT_OUT,
	ANIM_26,
	ANIM_27,
	ANIM_28,
	ANIM_29,
	ANIM_30,
	ANIM_31,
	ANIM_FIRE_ADS,
	ANIM_LASTSHOT_ADS,
	ANIM_34,
	ANIM_ADS_UP,
	ANIM_ADS_DOWN,
	ANIM_MAX
};

typedef struct
{
	const char* name;
	WeaponDef * weaponDef;
	char* localizedName;
	short* hideTags; // I think?
	char** animNames; // need to document which one is which
	char pad[40];
	char* unkString;
	char pad2[8];
	Material* hudIconMat;
	Material* dpadSelectMat;
	char pad3[20];
	short unkCount1;
	short unkCount2;
	void *unkStuff1; // sizeof 8
	void *unkStuff2; // sizeof 8
	int unknown;
} WeaponHeader; // sizeof 116

typedef struct 
{
	const char* name;
	int unk1; // 0x0
	int count1;
	int unk2[4];
	char * unkptr;
} fx_t;

typedef struct
{
	const char* name;
	char pad[4];
} impactfx_t;

struct Rawfile
{
	const char* name;
	int sizeCompressed;
	int sizeUnCompressed;
	char * compressedData;
};

typedef struct {
	const char* name;
	int columns;
	int rows;
	char** data;
} StringTable;

typedef struct
{
	const char* name;
	char pad[32];
} leaderboarddef_t;

typedef struct
{
	const char* name;
	char pad[8];
} structureddatadef_t;

typedef struct // only 1?
{
	const char* name;
	char * ptr1;
	int unkcount1;
	float range;
	float length;
	float width;
	float depth;
	float color[3];
	float unkval[18];
} tracer_t;

typedef struct
{
	const char* name;
	char pad[716];
} vehicle_t;

typedef struct // not used anywhere
{
	const char* name;
	char pad [32];
} addon_map_ents_t;