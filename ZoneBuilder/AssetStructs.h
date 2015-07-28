#include "StdInc.h"
#pragma once

struct PhysPreset;
struct PhysGeomList;
struct XAnim;
struct XModelSurfaces;
struct XModel;
struct Material;
struct PixelShader;
struct VertexShader;
struct VertexDecl;
struct MaterialTechniqueSet;
struct GfxImage;
struct SoundAliasList;
struct SndCurve;
struct LoadedSound;
//struct CollisionMap;
//struct ComMap;
//struct FxMap;
//struct GfxMap;
struct GfxLightDef;
struct Font;
struct MenuList;
struct menuDef_t;
struct Localize;
struct FXEffectDef;
struct FxImpactTable;
struct Rawfile;
struct StringTable;
struct LeaderboardDef;
struct StructuredDataDefSet;
struct Tracer;
//struct Vehicle;

#include "XAnim.h"
#include "Material.h"
#include "XModel.h"
#include "SoundAlias.h"
#include "FX.h"
#include "Maps.h"
#include "Weapon.h"
#include "StructuredDataDef.h"
#include "Menu.h"

struct PhysPreset
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

struct FxMap
{
	const char* name;
	char pad[114];
};

struct GfxMap
{
	const char* name;
	char pad[624];
};

struct GfxLightImage
{
	GfxImage *image;
	char samplerState;
};

struct GfxLightDef
{
	const char *name;
	GfxLightImage attenuation;
	int lmapLookupStart;
};

struct FontEntry
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
};

struct Font
{
	char* name;
	int size;
	int entries;
	Material* image;
	Material* glowImage;
	FontEntry* characters;
};

// we will leave menus out of this as they are compicated as hell

struct Localize
{
	const char * localizedString;
	const char * name;
};

struct FxImpactEntry
{
	FxEffectDef *nonflesh[31];
	FxEffectDef *flesh[4];
};

struct FxImpactTable
{
	const char *name;
	FxImpactEntry *table;
};

struct LbColumnDef
{
	const char* title;
	int id;
	int propertyId;
	int unk1;
	const char* statName;
	int unk2;
	int unk3;
	int unk4;
};

struct LeaderboardDef
{
	const char* name;
	int id;
	int columnCount;
	int xpColId;
	int prestigeColId;
	LbColumnDef* columns;
};

struct Tracer
{
	const char * name;
	Material * material;
	unsigned int drawInterval;
	float speed;
	float beamLength;
	float beamWidth;
	float screwRadius;
	float screwDist;
	float colors[5][4];
};

struct Vehicle
{
	const char* name;
	char pad[716];
};

struct AddonMapEnts
{
	const char* name;
	char pad[32];
};

struct Rawfile
{
	const char* name;
	int sizeCompressed;
	int sizeUnCompressed;
	char * compressedData;
};

struct StringTable {
	const char* name;
	int columns;
	int rows;
	char** data;
};