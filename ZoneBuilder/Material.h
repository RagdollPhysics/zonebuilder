#include "StdInc.h"
#pragma once

// material stuff

enum IWI_COMPRESSION_e
{
	IWI_INVALID = 0x0,
	IWI_ARGB = 0x1,
	IWI_RGB8 = 0x2,
	IWI_DXT1 = 0xB,
	IWI_DXT3 = 0xC,
	IWI_DXT5 = 0xD,
} IWI_COMPRESSION;

enum MaterialMapHashes
{
	HASH_COLORMAP = 0xa0ab1041,
	HASH_DETAILMAP = 0xeb529b4d,
	HASH_SPECULARMAP = 0x34ecccb3,
	HASH_NORMALMAP = 0x59d30d0f
};

enum MaterialSemantic
{
	SEMANTIC_2D = 0x0,
	SEMANTIC_FUNCTION = 0x1,
	SEMANTIC_COLOR_MAP = 0x2,
	SEMANTIC_NORMAL_MAP = 0x5,
	SEMANTIC_SPECULAR_MAP = 0x8,
	SEMANTIC_WATER_MAP = 0xB
};

typedef struct
{
	char magic[3]; //IWi
	char version; // 8
	int flags;
	short format; // see above
	short xsize;
	short ysize;
	short depth;
	int mipAddr4;
	int mipAddr3;
	int mipAddr2;
	int mipAddr1;
} _IWI;

struct GfxImageLoadDef // actually a IDirect3DTexture* but this is easier
{
	char mipLevels;
	char flags;
	short dimensions[3];
	int format; // usually the compression Magic
	int dataSize; // set to zero to load from IWD
	//char * data; 
};

struct GfxImage
{
	GfxImageLoadDef * /*Direct3DTexture9**/ texture;
	char mapType; // 5 is cube, 4 is 3d, 3 is 2d
	char semantic;
	char category;
	char flags;
	int cardMemory;
	int dataLen1;
	int dataLen2;
	short height;
	short width;
	short depth;
	bool loaded;
	char pad;
	char* name;
};

struct water_t
{
	float floatTime;
	float *H0X;		// Count = M * N
	float *H0Y;		// Count = M * N
	float *wTerm;		// Count = M * N
	int M;
	int N;
	float Lx;
	float Lz;
	float gravity;
	float windvel;
	float winddir[2];
	float amplitude;
	float codeConstant[4];
	GfxImage *image;
};

union MaterialTextureDefInfo
{
	GfxImage *image;	// MaterialTextureDef->semantic != SEMANTIC_WATER_MAP
	water_t *water;		// MaterialTextureDef->semantic == SEMANTIC_WATER_MAP
};

struct MaterialTextureDef
{
	unsigned int nameHash;
	char nameStart;
	char nameEnd;
	char sampleState;
	char semantic;
	MaterialTextureDefInfo info;
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

struct Material_old
{
	const char* name;
	char gameFlags;
	char sortKey;
	unsigned char animationX; // amount of animation frames in X
	unsigned char animationY; // amount of animation frames in Y
	void* drawSurf;
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

struct MaterialConstantDef
{
	int nameHash;
	char name[12];
	vec4_t literal;
};

struct Material
{
	const char *name;
	char gameFlags;
	char sortKey;
	char textureAtlasRowCount;
	char textureAtlasColumnCount;
	char drawSurf[12];
	int surfaceTypeBits;
	char stateBitsEntry[48];
	char textureCount;
	char constantCount;
	char stateBitsCount;
	char stateFlags;
	char cameraRegion;
	MaterialTechniqueSet *techniqueSet;
	MaterialTextureDef *textureTable;
	MaterialConstantDef *constantTable;
	void *stateBitTable;
};