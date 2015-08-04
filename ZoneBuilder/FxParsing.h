#include "StdInc.h"
#include "Tool.h"

#pragma once

enum fxEntryType_e : int
{
	ENTRY_TYPE_CHAR,
	ENTRY_TYPE_INT,
	ENTRY_TYPE_FLOAT,
	ENTRY_TYPE_VEC3,
	ENTRY_TYPE_INT_RANGE,
	ENTRY_TYPE_FLOAT_RANGE,
	ENTRY_TYPE_VEC3X_RANGE,
	ENTRY_TYPE_VEC3Y_RANGE,
	ENTRY_TYPE_VEC3Z_RANGE,
	ENTRY_TYPE_STRING,
	ENTRY_TYPE_GRAPH,
	ENTRY_TYPE_FLAGS,
	ENTRY_TYPE_VISUALS,
};

enum fxEntryLoc_e : int
{
	LOC_EFFECT,
	LOC_ELEM,
	LOC_SPARK,
	LOC_TRAIL,
	LOC_ATLAS,
};

struct fxEntry_s
{
	const char* name;
	int offset;
	fxEntryType_e type;
	fxEntryLoc_e loc;
};

struct fxEffectFlag_s
{
	const char* name;
	int offset;
	int flagRemove;
	int flagSet;
};

fxEffectFlag_s* effectFlags = (fxEffectFlag_s*)0x73B518;

bool FX_ParseFlag(int* data, char* flag)
{
	for(int i=0; i<49; i++)
	{
		if(!strcmp(effectFlags[i].name, flag))
		{
			*data &= ~effectFlags[i].flagRemove;
			*data |= effectFlags[i].flagSet;
			return true;
		}
	}
	return false;
}

bool FX_ParseFlagList(int* data, char** session) 
	// remember we consume the semi-colon so we need to skip it laters
{
	char* v = Com_ParseExt(session);
	if(*v == ';') { return true;}

	if(v[strlen(v) - 1] == ';') {
			v[strlen(v) - 1] = 0;
			FX_ParseFlag(data, v);
			return true;
		}

	while(FX_ParseFlag(data, v))
	{
		v = Com_ParseExt(session);
		if(v[strlen(v) - 1] == ';') {
			v[strlen(v) - 1] = 0;
			FX_ParseFlag(data, v);
			return true;
		}
	}
	return false;
}


struct fxVisualType_s
{
	const char* name;
	FxElemType type;
};

fxVisualType_s visualTypes[] = {
	{ "billboardSprite",  FX_ELEM_TYPE_SPRITE_BILLBOARD },
	{ "orientedSprite",   FX_ELEM_TYPE_SPRITE_ORIENTED },
	{ "cloud",            FX_ELEM_TYPE_CLOUD },
	{ "sparkCloud",       FX_ELEM_TYPE_SPARKCLOUD },
	{ "sparkFountain",    FX_ELEM_TYPE_SPARKFOUNTAIN },
	{ "tail",             FX_ELEM_TYPE_TAIL },
	{ "trail",            FX_ELEM_TYPE_TRAIL },
	{ "decal",            FX_ELEM_TYPE_DECAL },
	{ "model",            FX_ELEM_TYPE_MODEL },
	{ "light",            FX_ELEM_TYPE_OMNI_LIGHT },
	{ "spotLight",        FX_ELEM_TYPE_SPOT_LIGHT },
	{ "runner",           FX_ELEM_TYPE_RUNNER },
	{ "sound",            FX_ELEM_TYPE_SOUND }
};

void * getVisualByName(zoneInfo_t* info, const char* name, char type)
{
	switch(type)
	{
	case FX_ELEM_TYPE_MODEL:
		{
			void* model = addXModel(info, name, (char*)findAssetEverywhere(info, ASSET_TYPE_XMODEL, name), 0);
			addAsset(info, ASSET_TYPE_XMODEL, name, model);
			return model;
		}

	case FX_ELEM_TYPE_SOUND:
		{
			char* ret = strdup(name);
			return (char*)&ret;
		}
	case FX_ELEM_TYPE_RUNNER:
		{
			void* effect = addFxEffectDef(info, name, (char*)findAssetEverywhere(info, ASSET_TYPE_FX, name), 0);
			addAsset(info, ASSET_TYPE_FX, name, effect);
			char* ret = strdup(name);
			return (char*)&ret;
		}

	case FX_ELEM_TYPE_OMNI_LIGHT:
	case FX_ELEM_TYPE_SPOT_LIGHT:
			return NULL;

	default:
		{
			void* material = addMaterial(info, name, (char*)findAssetEverywhere(info, ASSET_TYPE_MATERIAL, name), 0);
			addAsset(info, ASSET_TYPE_MATERIAL, name, material);
			return material;
		}
	}
}

bool FX_ParseVisuals(zoneInfo_t* info, const char* vis, FxElemDef* data, char**session)
{
	if(data->elemType != -1)  { Com_Debug("Error: can't have more than one vis type per segment!\n"); return false;}

	for(int i=0; i<sizeof(visualTypes); i++)
	{
		if(!strcmp(visualTypes[i].name, vis))
		{
			data->elemType = visualTypes[i].type;
			break;
		}
	}

	if(data->elemType == 11) // decal
		Com_Error(true, "Error can't do decal FX assets!");

	char * elements[16];
	memset(elements, 0, sizeof(elements));
	int i = 0;

	Com_Parse_MatchToken(session, "{", false);

	char* token = Com_ParseExt(session);
	while(*token != '}')
	{
		elements[i++] = strdup(token);
		token = Com_ParseExt(session);
	}

	data->visualCount = i;

	if(data->elemType == 11)
	{
		/*data->visuals.markArray = (FxElemMarkVisuals*)calloc(i, sizeof(FxElemMarkVisuals));
		for(int j=0; j<i; j+=2)
		{
			data->visuals.markArray[j/2].data[0] = (Material*)getVisualByName(info, elements[j], data->elemType);
			data->visuals.markArray[j/2].data[1] = (Material*)getVisualByName(info, elements[j + 1], data->elemType);
		}*/
	}
	else if(i > 1)
	{
		data->visuals.array = (FxElemVisuals*)calloc(i, sizeof(FxElemVisuals));
		for(int j=0; j<i; j++)
		{
			data->visuals.array[j].anonymous = getVisualByName(info, elements[j], data->elemType);
		}
	}
	else
	{
		data->visuals.instance.anonymous = getVisualByName(info, elements[0], data->elemType);
	}

	return true;
}

#define doElemEntry(s, o, t) { s, offsetof(FxElemDef, o), t, LOC_ELEM }, 
#define doEffectEntry(s, o, t) { s, offsetof(FxEffectDef, o), t , LOC_EFFECT },
#define doSparkEntry(s, o, t) { s, offsetof(FxSparkFountain, o), t, LOC_SPARK },
#define doAtlasEntry(s, o, t) { s, offsetof(FxElemAtlas, o), t, LOC_ATLAS },
#define doTrailEntry(s, o, t) { s, offsetof(FxTrailDef, o), t, LOC_TRAIL },
#define doGraphEntry(s) { s, NULL, ENTRY_TYPE_GRAPH, LOC_ELEM },

fxEntry_s fxDefEntries [] = { 
	doEffectEntry( "name",							 name,     						 ENTRY_TYPE_STRING )
	doEffectEntry( "editorFlags",					 flags,		                     ENTRY_TYPE_FLAGS )
	doElemEntry( "flags",                            flags,                          ENTRY_TYPE_FLAGS )
	doElemEntry( "spawnRange",                       spawnRange,                     ENTRY_TYPE_FLOAT_RANGE )
	doElemEntry( "fadeInRange",                      fadeInRange,                    ENTRY_TYPE_FLOAT_RANGE )
	doElemEntry( "fadeOutRange",                     fadeOutRange,                   ENTRY_TYPE_FLOAT_RANGE )
	doElemEntry( "spawnFrustumCullRadius",           spawnFrustumCullRadius,         ENTRY_TYPE_FLOAT )
	doElemEntry( "spawnLooping",                     spawn,							 ENTRY_TYPE_INT_RANGE )
	doElemEntry( "spawnOneShot",                     spawn,							 ENTRY_TYPE_INT_RANGE )
	doElemEntry( "spawnDelayMsec",                   spawnDelayMsec,                 ENTRY_TYPE_INT_RANGE )
	doElemEntry( "lifeSpanMsec",                     lifeSpanMsec,                   ENTRY_TYPE_INT_RANGE )
	doElemEntry( "spawnOrgX",                        spawnOrigin[0],                 ENTRY_TYPE_FLOAT_RANGE )
	doElemEntry( "spawnOrgY",                        spawnOrigin[1],                 ENTRY_TYPE_FLOAT_RANGE )
	doElemEntry( "spawnOrgZ",                        spawnOrigin[2],                 ENTRY_TYPE_FLOAT_RANGE )
	doElemEntry( "spawnOffsetRadius",                spawnOffsetRadius,              ENTRY_TYPE_FLOAT_RANGE )
	doElemEntry( "spawnOffsetHeight",                spawnOffsetHeight,              ENTRY_TYPE_FLOAT_RANGE )
	doElemEntry( "spawnAnglePitch",                  spawnAngles[0],				 ENTRY_TYPE_FLOAT_RANGE )
	doElemEntry( "spawnAngleYaw",                    spawnAngles[1],                 ENTRY_TYPE_FLOAT_RANGE )
	doElemEntry( "spawnAngleRoll",                   spawnAngles[2],                 ENTRY_TYPE_FLOAT_RANGE )
	doElemEntry( "angleVelPitch",                    angularVelocity[0],             ENTRY_TYPE_FLOAT_RANGE )
	doElemEntry( "angleVelYaw",                      angularVelocity[1],             ENTRY_TYPE_FLOAT_RANGE )
	doElemEntry( "angleVelRoll",                     angularVelocity[2],             ENTRY_TYPE_FLOAT_RANGE )
	doElemEntry( "initialRot",                       initialRotation,                ENTRY_TYPE_FLOAT_RANGE )
	doElemEntry( "gravity",                          gravity,                        ENTRY_TYPE_FLOAT_RANGE )
	//doElemEntry( "elasticity",                       elasticity,                     ENTRY_TYPE_FLOAT_RANGE )
	doAtlasEntry( "atlasBehavior",                   behavior,						 ENTRY_TYPE_FLAGS )
	doAtlasEntry( "atlasIndex",                      index,							 ENTRY_TYPE_CHAR )
	doAtlasEntry( "atlasFps",                        fps,							 ENTRY_TYPE_CHAR )
	doAtlasEntry( "atlasLoopCount",                  loopCount,						 ENTRY_TYPE_CHAR )
	doAtlasEntry( "atlasColIndexBits",               colIndexBits,					 ENTRY_TYPE_CHAR )
	doAtlasEntry( "atlasRowIndexBits",               rowIndexBits,					 ENTRY_TYPE_CHAR )
	doAtlasEntry( "atlasEntryCount",                 entryCount,					 ENTRY_TYPE_CHAR )

	doGraphEntry( "velGraph0X" )
	doGraphEntry( "velGraph0Y" )
	doGraphEntry( "velGraph0Z" )
	doGraphEntry( "velGraph1X" )
	doGraphEntry( "velGraph1Y" )
	doGraphEntry( "velGraph1Z" )
	doGraphEntry( "rotGraph"   )
	doGraphEntry( "sizeGraph0" )
	doGraphEntry( "sizeGraph1" )
	doGraphEntry( "scaleGraph" )
	doGraphEntry( "colorGraph" )
	doGraphEntry( "alphaGraph" )

	doElemEntry( "lightingFrac",                     lightingFrac,                   ENTRY_TYPE_CHAR )
	//doElemEntry( "decalFadeInTime",                  decalFadeInTime,                ENTRY_TYPE_FLOAT )
	doElemEntry( "collOffset",                       collMins,                       ENTRY_TYPE_VEC3 )
	doElemEntry( "collRadius",                       collMaxs,                       ENTRY_TYPE_VEC3 )
	doElemEntry( "fxOnImpact",                       effectOnImpact,                 ENTRY_TYPE_STRING )
	doElemEntry( "fxOnDeath",                        effectOnDeath,                  ENTRY_TYPE_STRING )
	doElemEntry( "sortOrder",                        sortOrder,                      ENTRY_TYPE_CHAR )
	doElemEntry( "emission",                         effectEmitted,                  ENTRY_TYPE_STRING )
	doElemEntry( "emitDist",                         emitDist,                       ENTRY_TYPE_FLOAT_RANGE )
	doElemEntry( "emitDistVariance",                 emitDistVariance,               ENTRY_TYPE_FLOAT_RANGE )
	/*
	doTrailEntry( "trailRepeatTime",                  trailRepeatTime,                ENTRY_TYPE_ )
	doTrailEntry( "trailSplitDist",                   trailSplitDist,                 ENTRY_TYPE_ )
	doTrailEntry( "trailSplitArcDist",                trailSplitArcDist,              ENTRY_TYPE_ )
	doTrailEntry( "trailSplitTime",                   trailSplitTime,                 ENTRY_TYPE_ )
	doTrailEntry( "trailScrollTime",                  trailScrollTime,                ENTRY_TYPE_ )
	doTrailEntry( "trailRepeatDist",                  trailRepeatDist,                ENTRY_TYPE_ )
	doTrailEntry( "trailDef",                         trailDef,                       ENTRY_TYPE_ )
	*/
	doSparkEntry( "sparkFountainGravity",             sparkFountainGravity,          ENTRY_TYPE_FLOAT )
	doSparkEntry( "sparkFountainBounceFrac",          sparkFountainBounceFrac,       ENTRY_TYPE_FLOAT )
	doSparkEntry( "sparkFountainBounceRand",          sparkFountainBounceRand,       ENTRY_TYPE_FLOAT )
	doSparkEntry( "sparkFountainSparkSpacing",        sparkFountainSparkSpacing,     ENTRY_TYPE_FLOAT )
	doSparkEntry( "sparkFountainSparkLength",         sparkFountainSparkLength,      ENTRY_TYPE_FLOAT )
	doSparkEntry( "sparkFountainSparkCount",          sparkFountainSparkCount,       ENTRY_TYPE_INT )
	doSparkEntry( "sparkFountainLoopTime",            sparkFountainLoopTime,         ENTRY_TYPE_FLOAT )
	doSparkEntry( "sparkFountainVelMin",              sparkFountainVelMin,           ENTRY_TYPE_FLOAT )
	doSparkEntry( "sparkFountainVelMax",              sparkFountainVelMax,           ENTRY_TYPE_FLOAT )
	doSparkEntry( "sparkFountainVelConeAngle",        sparkFountainVelConeAngle,     ENTRY_TYPE_FLOAT )
	doSparkEntry( "sparkFountainRestSpeed",           sparkFountainRestSpeed,        ENTRY_TYPE_FLOAT )
	doSparkEntry( "sparkFountainBoostTime",           sparkFountainBoostTime,        ENTRY_TYPE_FLOAT )
	doSparkEntry( "sparkFountainBoostFactor",         sparkFountainBoostFactor,      ENTRY_TYPE_FLOAT )
	//doSparkEntry( "sparkFountainCollisionXY",         sparkFountainCollisionXY,      ENTRY_TYPE_FLOAT )
	//doSparkEntry( "sparkFountainCollisionZMin",       sparkFountainCollisionZMin,    ENTRY_TYPE_FLOAT )
	//doSparkEntry( "sparkFountainCollisionZMax",       sparkFountainCollisionZMax,    ENTRY_TYPE_FLOAT )
	doElemEntry( "billboardSprite",                  visuals,						 ENTRY_TYPE_VISUALS )
	doElemEntry( "orientedSprite",                   visuals,						 ENTRY_TYPE_VISUALS )
	doElemEntry( "cloud",                            visuals,                        ENTRY_TYPE_VISUALS )
	doElemEntry( "sparkCloud",                       visuals,						 ENTRY_TYPE_VISUALS )
	doElemEntry( "sparkFountain",                    visuals,						 ENTRY_TYPE_VISUALS )
	doElemEntry( "tail",                             visuals,                        ENTRY_TYPE_VISUALS )
	doElemEntry( "trail",                            visuals,                        ENTRY_TYPE_VISUALS )
	doElemEntry( "decal",                            visuals,                        ENTRY_TYPE_VISUALS )
	doElemEntry( "model",                            visuals,                        ENTRY_TYPE_VISUALS )
	doElemEntry( "light",                            visuals,                        ENTRY_TYPE_VISUALS )
	doElemEntry( "spotLight",                        visuals,						 ENTRY_TYPE_VISUALS )
	doElemEntry( "runner",                           visuals,                        ENTRY_TYPE_VISUALS )
	doElemEntry( "sound",                            visuals,                        ENTRY_TYPE_VISUALS )
};

fxEntry_s * getFxEntryByName(const char* name)
{
	for(int i=0; i<78; i++)
		if(!strcmp(fxDefEntries[i].name, name))
			return &fxDefEntries[i];
	return NULL;
}

void doParseFxEntry(zoneInfo_t* info, char** session, const char* name, FxEffectDef* def, int elem)
{
	fxEntry_s* entry = getFxEntryByName(name);
	if(entry == NULL)  { Com_Debug("No such fx entry %s. Continuing...\n", name); return; }

	char* base = (char*)def;

	switch(entry->loc)
	{
	case LOC_ELEM:
		base = (char*)&def->elemDefs[elem];
		break;
	case LOC_SPARK:
		base = (char*)&def->elemDefs[elem].trailDef->sparkFountain;
		break;
	case LOC_TRAIL:
		base = (char*)&def->elemDefs[elem].trailDef->trailDef;
		break;
	case LOC_ATLAS:
		base = (char*)&def->elemDefs[elem].atlas;
		break;
	}

	void * place = base + entry->offset;

	switch(entry->type)
	{
	case ENTRY_TYPE_CHAR:
		{
			char* c = (char*)place;
			*c = Com_ParseInt(session);
			break;
		}
	case ENTRY_TYPE_INT:
		{
			int* i = (int*)place;
			*i = Com_ParseInt(session);
			break;
		}
	case ENTRY_TYPE_FLOAT:
		{
			float* f = (float*)place;
			*f = Com_ParseFloat(session);
			break;
		}
	case ENTRY_TYPE_VEC3:
		{
			vec3_t* vec = (vec3_t*)place; 
			vec->x = Com_ParseFloat(session);
			vec->y = Com_ParseFloat(session);
			vec->z = Com_ParseFloat(session);
			break;
		}
	case ENTRY_TYPE_INT_RANGE:
		{
			FxIntRange* intr = (FxIntRange*)place;
			intr->base = Com_ParseInt(session);
			intr->amplitude = Com_ParseInt(session);
			break;
		}
	case ENTRY_TYPE_FLOAT_RANGE:
		{
			FxFloatRange* flr = (FxFloatRange*)place;
			flr->base = Com_ParseFloat(session);
			flr->amplitude = Com_ParseFloat(session);
			break;
		}
	case ENTRY_TYPE_STRING:
		{
			char** str = (char**)place;
			*str = strdup(Com_ParseExt(session));
			if(!**str) *str = NULL;
			break;
		}
	case ENTRY_TYPE_FLAGS:
		{
			FX_ParseFlagList((int*)place, session);
			break;
		}
	case ENTRY_TYPE_VISUALS:
		{
			FX_ParseVisuals(info, name, &def->elemDefs[elem], session);
			break;
		}
		break;
	case ENTRY_TYPE_GRAPH:
		{
			int unk = Com_ParseInt(session);
			char* token = Com_ParseExt(session);
			int level = 1;
			while(*token != '}' || level > 0)
			{
				token = Com_ParseExt(session);
				if(*token == '{') level++;
				if(*token == '}') level--;
			}
		}
		break;
	}

	if(*Com_ParseExt(session) != ';') Com_Parse_UngetToken(); // make sure we consume the semi-colon
}