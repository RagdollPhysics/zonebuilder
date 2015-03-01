#include "StdInc.h"
#pragma once

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