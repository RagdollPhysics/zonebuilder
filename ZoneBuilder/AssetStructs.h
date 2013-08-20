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
	short _1;
	short _2;
} Indicies;

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
	Indicies* indicies; // 76 - 0x4C
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
	//char * data; not needed because we are using IWDs
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
	void * /*Direct3DVertexShader9**/ shader;
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

struct XSurface
{
	short index; // +0
	short numVertices; // +2
	short numPrimitives; // +4
	unsigned char streamHandle; // something to do with buffers, +6
	char pad2; // +7
	int pad3; // +8
	void* indexBuffer; // +12
	char pad4[12]; // +16
	void* vertexBuffer; // +28
	char pad5[32]; // +32
}; // total size, 64

typedef struct
{
	const char* name;
	XSurface* surfaces;
	int numSurfaces;
	char pad[24];
} XModelSurface;

struct XSurfaceLod
{
	char pad[8];
	XModelSurface* surfaces;
	char pad2[32];
};

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
	char* boneUnknown5; // +56, element size 32
	Material** materials; // +60
	XSurfaceLod lods[4]; // +64
	int pad4; // +240
	char* stuffs; // +244
	int numStuffs; // +248
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
	char pad[20];
	StreamFile* stream;
	char pad2[76];
} snd_alias_t;

typedef struct  
{
	const char* name;
	snd_alias_t* aliases;
	int numAliases;
} snd_alias_list_t;

typedef struct
{
	const char* name;
	char pad[132];
} sndcurve_t;

typedef struct
{
	const char* name;
	char pad[40];
} loaded_sound_t;

typedef struct
{
	const char* name;
	char pad[252];
} col_map_sp_t;

typedef struct
{
	const char* name;
	char pad[252];
} col_map_mp_t;

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
	const char* entitystring;
} map_ents_t;

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
} weapon_t;

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

typedef struct
{
	const char* name;
	int sizeCompressed;
	int sizeUnCompressed;
	char * compressedData;
} rawfile_t;

typedef struct {
	const char* name;
	int columns;
	int rows;
	char** data;
} stringtable_t;

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