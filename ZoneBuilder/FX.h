#include "StdInc.h"
#pragma once

/* FxEffectDef::flags */
#define FX_ELEM_LOOPING				0x1
#define FX_ELEM_USE_RAND_COLOR			0x2
#define FX_ELEM_USE_RAND_ALPHA			0x4
#define FX_ELEM_USE_RAND_SIZE0			0x8
#define FX_ELEM_USE_RAND_SIZE1			0x10
#define FX_ELEM_USE_RAND_SCALE			0x20
#define FX_ELEM_USE_RAND_ROT_DELTA		0x40
#define FX_ELEM_MOD_COLOR_BY_ALPHA		0x80
#define FX_ELEM_USE_RAND_VEL0			0x100
#define FX_ELEM_USE_RAND_VEL1			0x200
#define FX_ELEM_USE_BACK_COMPAT_VEL		0x400
#define FX_ELEM_ABS_VEL0			0x800
#define FX_ELEM_ABS_VEL1			0x1000
#define FX_ELEM_PLAY_ON_TOUCH			0x2000
#define FX_ELEM_PLAY_ON_DEATH			0x4000
#define FX_ELEM_PLAY_ON_RUN			0x8000
#define FX_ELEM_BOUNDING_SPHERE			0x10000
#define FX_ELEM_USE_ITEM_CLIP			0x20000
#define FX_ELEM_DISABLED			0x80000000
#define FX_ELEM_DECAL_FADE_IN			0x40000

/* FxElemDef::flags */
#define FX_ELEM_SPAWN_RELATIVE_TO_EFFECT	0x2
#define FX_ELEM_SPAWN_FRUSTUM_CULL		0x4
#define FX_ELEM_RUNNER_USES_RAND_ROT		0x8
#define FX_ELEM_SPAWN_OFFSET_NONE		0x0
#define FX_ELEM_SPAWN_OFFSET_SPHERE		0x10
#define FX_ELEM_SPAWN_OFFSET_CYLINDER		0x20
#define FX_ELEM_SPAWN_OFFSET_MASK		0x30
#define FX_ELEM_RUN_RELATIVE_TO_WORLD		0x0
#define FX_ELEM_RUN_RELATIVE_TO_SPAWN		0x40
#define FX_ELEM_RUN_RELATIVE_TO_EFFECT		0x80
#define FX_ELEM_RUN_RELATIVE_TO_OFFSET		0xC0
#define FX_ELEM_RUN_MASK			0xC0
#define FX_ELEM_USE_COLLISION			0x100
#define FX_ELEM_DIE_ON_TOUCH			0x200
#define FX_ELEM_DRAW_PAST_FOG			0x400
#define FX_ELEM_DRAW_WITH_VIEWMODEL		0x800
#define FX_ELEM_BLOCK_SIGHT			0x1000
#define FX_ELEM_HAS_VELOCITY_GRAPH_LOCAL	0x1000000
#define FX_ELEM_HAS_VELOCITY_GRAPH_WORLD	0x2000000
#define FX_ELEM_HAS_GRAVITY			0x4000000
#define FX_ELEM_USE_MODEL_PHYSICS		0x8000000
#define FX_ELEM_NONUNIFORM_SCALE		0x10000000
#define FX_ELEM_CLOUD_SHAPE_CUBE		0x0
#define FX_ELEM_CLOUD_SHAPE_SPHERE_LARGE	0x20000000
#define FX_ELEM_CLOUD_SHAPE_SPHERE_MEDIUM	0x40000000
#define FX_ELEM_CLOUD_SHAPE_SPHERE_SMALL	0x60000000
#define FX_ELEM_CLOUD_MASK			0x60000000
#define FX_ELEM_DISABLE_FOUNTAIN_COLLISION	0x80000000
#define FX_ELEM_DRAW_IN_THERMAL_ONLY		0x2000
#define FX_ELEM_TRAIL_ORIENT_BY_VELOCITY	0x4000
#define FX_ELEM_EMIT_ORIENT_BY_ELEM		0x8000

/* FxElemAtlas::behavior */
#define FX_ATLAS_START_MASK		0x3
#define FX_ATLAS_START_FIXED		0x0
#define FX_ATLAS_START_RANDOM		0x1
#define FX_ATLAS_START_INDEXED		0x2
#define FX_ATLAS_PLAY_OVER_LIFE		0x4
#define FX_ATLAS_LOOP_ONLY_N_TIMES	0x8

enum FxElemType : char
{
	FX_ELEM_TYPE_SPRITE_BILLBOARD = 0x0,
	FX_ELEM_TYPE_SPRITE_ORIENTED = 0x1,
	FX_ELEM_TYPE_TAIL = 0x2,
	FX_ELEM_TYPE_TRAIL = 0x3,
	FX_ELEM_TYPE_CLOUD = 0x4,
	FX_ELEM_TYPE_SPARKCLOUD = 0x5,
	FX_ELEM_TYPE_SPARKFOUNTAIN = 0x6,
	FX_ELEM_TYPE_MODEL = 0x7,
	FX_ELEM_TYPE_OMNI_LIGHT = 0x8,
	FX_ELEM_TYPE_SPOT_LIGHT = 0x9,
	FX_ELEM_TYPE_SOUND = 0xA,
	FX_ELEM_TYPE_DECAL = 0xB,
	FX_ELEM_TYPE_RUNNER = 0xC,
	FX_ELEM_TYPE_COUNT = 0xD,
	FX_ELEM_TYPE_LAST_SPRITE = 0x3,
	FX_ELEM_TYPE_LAST_DRAWN = 0x9,
};

struct FxElemVec3Range
{
	float base[3];
	float amplitude[3];
};

struct FxIntRange
{
	int base;
	int amplitude;
};

struct FxFloatRange
{
	float base;
	float amplitude;
};

struct FxSpawnDefLooping
{
	int intervalMsec;
	int count;
};

struct FxSpawnDefOneShot
{
	FxIntRange count;
};

union FxSpawnDef
{
	FxSpawnDefLooping looping;
	FxSpawnDefOneShot oneShot;
};

struct FxEffectDef;

union FxEffectDefRef
{
	FxEffectDef *handle;
	const char *name;
};

union FxElemVisuals
{
	const void *anonymous;
	Material *material;
	XModel *xmodel;
	FxEffectDefRef *effectDef;
	const char *soundName;
};

struct FxElemMarkVisuals
{
	Material* data[2];
};

union FxElemDefVisuals
{
	FxElemVisuals instance;
	//If parent FxElemDef::elemType == 0x7, use xmodel
	//If parent FxElemDef::elemType == 0xC, use effectDef
	//If parent FxElemDef::elemType == 0xA, use soundName
	//If parent FxElemDef::elemType != 0x9 || 0x8, use material

	FxElemVisuals *array;			//Total count = parent FxElemDef::visualCount
	FxElemMarkVisuals *markArray;		//Total count = parent FxElemDef::visualCount
};

struct FxTrailVertex
{
	float pos[2];
	float normal[2];
	float texCoord[2];
};

struct FxTrailDef
{
	int scrollTimeMsec;
	int repeatDist;
	float splitArcDist;
	int splitDist;
	int splitTime;
	int vertCount;
	FxTrailVertex *verts;
	int indCount;
	unsigned __int16 *inds;
};

struct FxSparkFountain
{
	float sparkFountainGravity;
	float sparkFountainBounceFrac;
	float sparkFountainBounceRand;
	float sparkFountainSparkSpacing;
	float sparkFountainSparkLength;
	int sparkFountainSparkCount;
	float sparkFountainLoopTime;
	float sparkFountainVelMin;
	float sparkFountainVelMax;
	float sparkFountainVelConeAngle;
	float sparkFountainRestSpeed;
	float sparkFountainBoostTime;
	float sparkFountainBoostFactor;
};

union unknownFxUnion
{
	char *unknownBytes;
	FxSparkFountain *sparkFountain;
	FxTrailDef *trailDef;
};

struct FxElemAtlas
{
	char behavior;
	char index;
	char fps;
	char loopCount;
	char colIndexBits;
	char rowIndexBits;
	__int16 entryCount;
};

struct FxElemVelStateInFrame
{
	FxElemVec3Range velocity;
	FxElemVec3Range totalDelta;
};

struct FxElemVelStateSample
{
	FxElemVelStateInFrame local;
	FxElemVelStateInFrame world;
};

struct FxElemVisualState
{
	char color[4];
	float rotationDelta;
	float rotationTotal;
	float size[2];
	float scale;
};

struct FxElemVisStateSample
{
	FxElemVisualState base;
	FxElemVisualState amplitude;
};

struct FxElemDef	// 0xFC
{
	int flags;
	FxSpawnDef spawn;
	FxFloatRange spawnRange;
	FxFloatRange fadeInRange;
	FxFloatRange fadeOutRange;
	float spawnFrustumCullRadius;
	FxIntRange spawnDelayMsec;
	FxIntRange lifeSpanMsec;
	FxFloatRange spawnOrigin[3];
	FxFloatRange spawnOffsetRadius;
	FxFloatRange spawnOffsetHeight;
	FxFloatRange spawnAngles[3];
	FxFloatRange angularVelocity[3];
	FxFloatRange initialRotation;
	FxFloatRange gravity;
	FxFloatRange reflectionFactor;
	FxElemAtlas atlas;
	char elemType;
	char visualCount;
	char velIntervalCount;
	char visStateIntervalCount;
	FxElemVelStateSample *velSamples;	// count = velIntervalCount
	FxElemVisStateSample *visSamples;	// count = visStateIntervalCount
	FxElemDefVisuals visuals;
	//If elemType is 0xB, then use markVisuals
	//If elemType is not 0xB and visualCount == 1, then use visual
	//If elemType is not 0xB and visualCount != 1, then use visualsArray
	Vector3 collMins;
	Vector3 collMaxs;
	FxEffectDefRef *effectOnImpact;
	FxEffectDefRef *effectOnDeath;
	FxEffectDefRef *effectEmitted;
	FxFloatRange emitDist;
	FxFloatRange emitDistVariance;
	unknownFxUnion *trailDef;
	//If elemType == 3, then use trailDef
	//If elemType == 6, then use sparkFountain
	//If elemType != 3 && elemType != 6 use unknownBytes (size = 1)
	char sortOrder;
	char lightingFrac;
	char unused[2];
};

struct FxEffectDef
{
	const char *name;
	int flags;
	int totalSize;
	int msecLoopingLife;
	int elemDefCountLooping;
	int elemDefCountOneShot;
	int elemDefCountEmission;
	FxElemDef *elemDefs;		//Count = elemDefCountOneShot + elemDefCountEmission + elemDefCountLooping
};