#include "StdInc.h"
#include "AssetStructs.h"

struct XModelDrawInfo
{
	unsigned __int16 lod;
	unsigned __int16 surfId;
};

#pragma pack(push, 4)
struct GfxLightImage
{
	GfxImage *image;
	char samplerState;
};
#pragma pack(pop)
 
struct GfxLightDef
{
	const char *name;
	GfxLightImage attenuation;
	int lmapLookupStart;
};
 
struct GfxSceneDynModel
{
	XModelDrawInfo info;
	unsigned __int16 dynEntId;
};
 
struct BModelDrawInfo
{
	unsigned __int16 surfId;
};
 
struct GfxSceneDynBrush
{
	BModelDrawInfo info;
	unsigned __int16 dynEntId;
};
 
struct GfxStreamingAabbTree
{
	unsigned __int16 firstItem;
	unsigned __int16 itemCount;
	unsigned __int16 firstChild;
	unsigned __int16 childCount;
	float mins[3];
	float maxs[3];
};
 
struct GfxWorldStreamInfo
{
	int aabbTreeCount;
	GfxStreamingAabbTree *aabbTrees;
	int leafRefCount;
	int *leafRefs;
};
 
union GfxColor
{
	unsigned int packed;
	char asArray[4];
};
 
union PackedUnitVec
{
	unsigned int packed;
};
 
struct GfxWorldVertex
{
	float xyz[3];
	float binormalSign;
	GfxColor color;
	float texCoord[2];
	float lmapCoord[2];
	PackedUnitVec normal;
	PackedUnitVec tangent;
};
 
struct GfxWorldVertexData
{
	GfxWorldVertex *vertices;
	void* worldVb;
};
 
struct GfxLight
{
	char type;
	char canUseShadowMap;
	char unused[2];
	float color[3];
	float dir[3];
	float origin[3];
	float radius;
	float cosHalfFovOuter;
	float cosHalfFovInner;
	int exponent;
	unsigned int spotShadowIndex;
	GfxLightDef *def;
};
 
struct GfxReflectionProbe
{
	float offset[3];
};

struct cplane_s
{
	float normal[3];
	float dist;
	char type;
	char signbits;
};
 
struct GfxWorldDpvsPlanes
{
	int cellCount;
	cplane_s *planes;
	unsigned __int16 *nodes;
	unsigned int *sceneEntCellBits; //Size = cellCount << 11
};
 
struct GfxAabbTree
{
	float mins[3];
	float maxs[3];
	unsigned __int16 childCount;
	unsigned __int16 surfaceCount;
	unsigned __int16 startSurfIndex;
	unsigned __int16 smodelIndexCount;
	unsigned __int16 *smodelIndexes;
	int childrenOffset;
};
 
struct GfxLightGridEntry
{
	unsigned __int16 colorsIndex;
	char primaryLightIndex;
	char needsTrace;
};
 
struct GfxLightGridColors
{
	char rgb[56][3];
};
 
struct GfxStaticModelInst
{
	float mins[3];
	float maxs[3];
	GfxColor groundLighting;
};
 
struct srfTriangles_t
{
	int vertexLayerData;
	int firstVertex;
	unsigned __int16 vertexCount;
	unsigned __int16 triCount;
	int baseIndex;
};
 
struct GfxSurface
{
	srfTriangles_t tris;
	Material *material;
	char lightmapIndex;
	char reflectionProbeIndex;
	char primaryLightIndex;
	bool castsSunShadow;
};
 
struct GfxCullGroup
{
	float mins[3];
	float maxs[3];
	int surfaceCount;
	int startSurfIndex;
};
 
struct GfxDrawSurfFields
{
	__int64 _bf0;
};
 
union GfxDrawSurf
{
	GfxDrawSurfFields fields;
	unsigned __int64 packed;
};

#pragma pack(push, 4)
struct GfxPackedPlacement
{
	float origin[3];
	PackedUnitVec axis[3];
	float scale;
};
 
struct GfxStaticModelDrawInst
{
	GfxPackedPlacement placement;
	XModel *model;
	float cullDist;
	char reflectionProbeIndex;
	char primaryLightIndex;
	unsigned __int16 lightingHandle;
	char flags;
};
#pragma pack(pop)
 
struct GfxWorldDpvsStatic
{
	unsigned int smodelCount;
	unsigned int staticSurfaceCount;
	unsigned int litSurfsBegin;
	unsigned int litSurfsEnd;
	char unknown1[0x20];
	char *smodelVisData[3];
	char *surfaceVisData[3];
	unsigned __int16 *sortedSurfIndex;
	GfxStaticModelInst *smodelInsts;
	GfxSurface *surfaces;
	GfxCullGroup *cullGroups;
	GfxStaticModelDrawInst *smodelDrawInsts;
	GfxDrawSurf *surfaceMaterials;
	unsigned int *surfaceCastsSunShadow;
	volatile int usageCount;
};
 
#pragma pack(push, 4)
 
struct GfxPortalWritable
{
	bool isQueued;
	bool isAncestor;
	char recursionDepth;
	char hullPointCount;
	float (*hullPoints)[2];
};
 
struct DpvsPlane
{
	 float coeffs[4];
	 char side[3];
};
 
struct GfxPortal
{
	GfxPortalWritable writable;
	DpvsPlane plane;
	float (*vertices)[3];
	char unknown[2];
	char vertexCount;
	float hullAxis[2][3];
};
 
struct GfxCell
{
	float mins[3];
	float maxs[3];
	int portalCount;
	GfxPortal *portals;
	char reflectionProbeCount;
	char *reflectionProbes;
};
 
struct GfxLightmapArray
{
	GfxImage *primary;
	GfxImage *secondary;
};
 
struct GfxLightGrid
{
	bool hasLightRegions;
	unsigned int sunPrimaryLightIndex;
	unsigned __int16 mins[3];
	unsigned __int16 maxs[3];
	unsigned int rowAxis;
	unsigned int colAxis;
	unsigned __int16 *rowDataStart;
	unsigned int rawRowDataSize;
	char *rawRowData;
	unsigned int entryCount;
	GfxLightGridEntry *entries;
	unsigned int colorCount;
	GfxLightGridColors *colors;
};

struct GfxBrushModelWritable
{
	float mins[3];
	float maxs[3];
};
 
struct GfxBrushModel
{
	GfxBrushModelWritable writable;
	float bounds[2][3];
	unsigned int surfaceCount;
	unsigned int startSurfIndex;
};

struct MaterialMemory
{
	Material *material;
	int memory;
};

struct sunflare_t
{
	bool hasValidData;
	Material *spriteMaterial;
	Material *flareMaterial;
	float spriteSize;
	float flareMinSize;
	float flareMinDot;
	float flareMaxSize;
	float flareMaxDot;
	float flareMaxAlpha;
	int flareFadeInTime;
	int flareFadeOutTime;
	float blindMinDot;
	float blindMaxDot;
	float blindMaxDarken;
	int blindFadeInTime;
	int blindFadeOutTime;
	float glareMinDot;
	float glareMaxDot;
	float glareMaxLighten;
	int glareFadeInTime;
	int glareFadeOutTime;
	float sunFxPosition[3];
};

struct GfxShadowGeometry
{
	unsigned __int16 surfaceCount;
	unsigned __int16 smodelCount;
	unsigned __int16 *sortedSurfIndex;
	unsigned __int16 *smodelIndex;
};

struct GfxLightRegionAxis
{
	float dir[3];
	float midPoint;
	float halfSize;
};

struct GfxLightRegionHull
{
	float kdopMidPoint[9];
	float kdopHalfSize[9];
	unsigned int axisCount;
	GfxLightRegionAxis *axis;
};

struct GfxLightRegion
{
	unsigned int hullCount;
	GfxLightRegionHull *hulls;
};
 
struct GfxWorldDpvsDynamic
{
	unsigned int dynEntClientWordCount[2];
	unsigned int dynEntClientCount[2];
	unsigned int *dynEntCellBits[2];
	char *dynEntVisData[2][3];
};
 
struct SunLightParseParams
{
	char name[64];
	float ambientScale;
	float ambientColor[3];
	float diffuseFraction;
	float sunLight;
	float sunColor[3];
	float diffuseColor[3];
	bool diffuseColorHasBeenSet;
	float angles[3];
};
 
struct GfxWorldVertexLayerData
{
	char *data;
	void* layerVb;
};
 
typedef char GfxTexture[0x34];
 
struct GfxWorldDraw
{
	unsigned int reflectionProbeCount;
	GfxImage * * reflectionImages;
	GfxReflectionProbe *reflectionProbes;
	GfxTexture * reflectionProbeTextures; //Count = refelctionProbeCount
	int lightmapCount;
	GfxLightmapArray *lightmaps;
	GfxTexture * lightmapPrimaryTextures; //Count = lightmapCount
	GfxTexture * lightmapSecondaryTextures; //Count = lightmapCount
	GfxImage *skyImage;
	GfxImage *outdoorImage;
	unsigned int vertexCount;
	GfxWorldVertexData vd;
	unsigned int vertexLayerDataSize;
	GfxWorldVertexLayerData vld;
	int indexCount;
	unsigned __int16 *indices;
	void* indexBuffer;
};
 
struct unknownGfxWorldStruct2
{
	int unknownCount;
	int * unknownArray;
	GfxImage * unknownImage;
	int unknown;
};

struct GfxWorld
{
	const char *name;
	const char *baseName;
	int planeCount;
	int nodeCount;
	int unknownInt2;
	unsigned int unknownCount1;
	unknownGfxWorldStruct2 * unknownStructs1; //Count = unknownCount1;
	char unknown1[0x18];
	GfxWorldDpvsPlanes dpvsPlanes; //The following rely on the count in this
	char *unknown4;
	GfxAabbTree *aabbTree;
	GfxCell *cells;
	GfxWorldDraw worldDraw;
	GfxLightGrid lightGrid;
	int modelCount;
	GfxBrushModel *models;
	float mins[3];
	float maxs[3];
	unsigned int checksum;
	int materialMemoryCount;
	MaterialMemory *materialMemory;
	sunflare_t sun;
	unsigned int *cellCasterBits[2];
	GfxSceneDynModel *sceneDynModel;
	GfxSceneDynBrush *sceneDynBrush;
	unsigned int *primaryLightEntityShadowVis;
	unsigned int *primaryLightDynEntShadowVis[2];
	char *primaryLightForModelDynEnt;
	GfxShadowGeometry *shadowGeom;
	GfxLightRegion *lightRegion;
	GfxWorldDpvsStatic dpvs;
	GfxWorldDpvsDynamic dpvsDyn;
	unsigned int unknownCount2;
	char * unknown2; //Size = unknownCount2 * 0x38
	int unknown3;
};
#pragma pack(pop)