#include "StdInc.h"
#include "Tool.h"

// do this one like NTA does cause it seems to be neater

void writeFxElemVisuals(zoneInfo_t* info, BUFFER* buf, FxElemVisuals* data, int type)
{
	switch(type)
	{
	case 12:
		buf->write(data->effectDef->name, strlen(data->effectDef->name) + 1, 1);
		data->effectDef->name = (const char*)-1;
		break;
	case 10:
		buf->write(data->soundName, strlen(data->soundName) + 1, 1);
		data->soundName = (const char*)-1;
		break;
	case 7: // all good for these, I think
	default:
		break;
	}
}

void writeFxElemDefVisuals(zoneInfo_t* info, BUFFER* buf, FxElemDefVisuals* data, int type, int count)
{
	if(type == 11)
	{
		if(data->markArray)
		{
			buf->write(data->markArray, sizeof(FxElemMarkVisuals), count); // erm this should just work
		}
	}
	else if(count > 1)
	{
		FxElemVisuals* vis = (FxElemVisuals*)buf->at();
		buf->write(data->array, sizeof(FxElemVisuals), count);
		for(int i=0; i<count; i++)
			writeFxElemVisuals(info, buf, &vis[i], type);
	}
	else
	{
		writeFxElemVisuals(info, buf, &data->instance, type);
	}
}

void writeFxElemDef(zoneInfo_t* info, BUFFER* buf, FxElemDef* data)
{
	FxElemDef* def = (FxElemDef*)buf->at();
	buf->write(data, sizeof(FxElemDef), 1);

	if(def->velSamples)
	{
		buf->write(def->velSamples, sizeof(FxElemVelStateSample), def->velIntervalCount + 1);
		def->velSamples = (FxElemVelStateSample*)-1;
	}

	if(def->visSamples)
	{
		buf->write(def->visSamples, sizeof(FxElemVisStateSample), def->visStateIntervalCount + 1);
		def->visSamples = (FxElemVisStateSample*)-1;
	}

	writeFxElemDefVisuals(info, buf, &def->visuals, def->elemType, def->visualCount);

	if(def->effectOnImpact)
	{
		buf->write(def->effectOnImpact->name, strlen(def->effectOnImpact->name) + 1, 1);
		def->effectOnImpact->name = (char*)-1;
	}

	if(def->effectOnDeath)
	{
		buf->write(def->effectOnDeath->name, strlen(def->effectOnDeath->name) + 1, 1);
		def->effectOnDeath->name = (char*)-1;
	}

	if(def->effectEmitted)
	{
		buf->write(def->effectEmitted->name, strlen(def->effectEmitted->name) + 1, 1);
		def->effectEmitted->name = (char*)-1;
	}

	if(def->trailDef)
	{
		def->trailDef = NULL;
		/*
		if(def->elemType == 3) // trailDef
		{
			if(def->trailDef->trailDef)
			{
				buf->write(def->trailDef->trailDef, sizeof(FxTrailDef), 1);
				if(def->trailDef->trailDef->verts)
				{
					buf->write(def->trailDef->trailDef->verts, sizeof(FxTrailVertex), def->trailDef->trailDef->vertCount);
					def->trailDef->trailDef->verts = (FxTrailVertex*)-1;
				}
				if(def->trailDef->trailDef->inds)
				{
					buf->write(def->trailDef->trailDef->inds, sizeof(unsigned short), def->trailDef->trailDef->indCount);
					def->trailDef->trailDef->inds = (unsigned short*)-1;
				}
				def->trailDef->trailDef = (FxTrailDef*)-1;
			}
		} 
		else if(def->elemType == 6) // sparkFountain
		{
			if(def->trailDef->sparkFountain)
			{
				buf->write(def->trailDef->sparkFountain, sizeof(FxSparkFountain), 1);
				def->trailDef->sparkFountain = (FxSparkFountain*)-1;
			}
		}
		else // unkBytes
		{
			if(def->trailDef->unknownBytes)
			{
				buf->write(def->trailDef->unknownBytes, sizeof(byte), 1);
				def->trailDef->unknownBytes = (char*)-1;
			}
		}*/
	}
}

void doRequireForFxDef(zoneInfo_t* info, BUFFER* buf, FxEffectDef* def)
{
	for(int i=0; i<def->elemDefCountLooping + def->elemDefCountOneShot + def->elemDefCountEmission; i++)
	{
		if(def->elemDefs[i].effectEmitted) {
			requireAsset(info, ASSET_TYPE_FX, (char*)def->elemDefs[i].effectEmitted->name, buf);
		}
		if(def->elemDefs[i].effectOnImpact)
		{
			requireAsset(info, ASSET_TYPE_FX, (char*)def->elemDefs[i].effectOnImpact->name, buf);
		}
		if(def->elemDefs[i].effectOnDeath)
		{
			requireAsset(info, ASSET_TYPE_FX, (char*)def->elemDefs[i].effectOnDeath->name, buf);
		}
		for(int j=0; j<def->elemDefs[i].visualCount; j++)
		{
			if(def->elemDefs[i].elemType == 11)
			{
				if(def->elemDefs[i].visuals.markArray)
				{
					Material* m1 = def->elemDefs[i].visuals.markArray[j].data[0];
					Material* m2 = def->elemDefs[i].visuals.markArray[j].data[1];
					def->elemDefs[i].visuals.markArray[j].data[0] = (Material*)(requireAsset(info, ASSET_TYPE_MATERIAL, (char*)m1->name, buf) | 0xF0000000);
					def->elemDefs[i].visuals.markArray[j].data[1] = (Material*)(requireAsset(info, ASSET_TYPE_MATERIAL, (char*)m2->name, buf) | 0xF0000000);
				}
			}
			else if(def->elemDefs[i].visualCount > 1)
			{
				switch(def->elemDefs[i].elemType)
				{
				case 7:
					{
						XModel* m = def->elemDefs[i].visuals.array[j].xmodel;
						def->elemDefs[i].visuals.array[j].xmodel = (XModel*)(requireAsset(info, ASSET_TYPE_XMODEL, m->name, buf) | 0xF0000000);
						break;
					}
				case 12:
					{
						FxEffectDefRef* fx = def->elemDefs[i].visuals.array[j].effectDef;
						requireAsset(info, ASSET_TYPE_FX, (char*)fx->name, buf);
						break;
					}
				case 10:
					{
						const char* str = def->elemDefs[i].visuals.array[j].soundName;
						// do something here later and hope it just works for now.
					}
				default:
					{
						if(def->elemDefs[i].elemType != 8 && def->elemDefs[i].elemType != 9)
						{
							Material* m = def->elemDefs[i].visuals.array[j].material;
							def->elemDefs[i].visuals.array[j].material = (Material*)(requireAsset(info, ASSET_TYPE_MATERIAL, (char*)m->name, buf) | 0xF0000000);
							break;
						}
					}
				}
			}
		}
	}
}

void writeFxEffectDef(zoneInfo_t* info, BUFFER* buf, FxEffectDef* data)
{
	doRequireForFxDef(info, buf, data);

	FxEffectDef* def = (FxEffectDef*)buf->at();
	buf->write(data, sizeof(FxEffectDef), 1);

	buf->write(def->name, strlen(def->name) + 1, 1);
	def->name = (char*)-1;

	if(def->elemDefs)
	{
		for(int i =0; i<def->elemDefCountLooping + def->elemDefCountOneShot + def->elemDefCountEmission; i++)
		{
			writeFxElemDef(info, buf, &def->elemDefs[i]);
		}
		def->elemDefs = (FxElemDef*)-1;
	}
}

void * addFxEffectDef(zoneInfo_t* info, const char* name, char* data, size_t dataLen)
{
	if(dataLen != 0) Com_Error(true, "Can't load effects yet");

	if(dataLen == 0)
	{
		FxEffectDef* def = (FxEffectDef*)data;
		// add nested effects
		for(int i =0; i<def->elemDefCountLooping + def->elemDefCountOneShot + def->elemDefCountEmission; i++)
		{
			if(def->elemDefs[i].effectEmitted) {
				addFxEffectDef(info, def->elemDefs[i].effectEmitted->handle->name, (char*)def->elemDefs[i].effectEmitted->handle, 0);
				def->elemDefs[i].effectEmitted->name = def->elemDefs[i].effectEmitted->handle->name;
			}
			if(def->elemDefs[i].effectOnImpact)
			{
				addFxEffectDef(info, def->elemDefs[i].effectOnImpact->handle->name, (char*)def->elemDefs[i].effectOnImpact->handle, 0);
				def->elemDefs[i].effectOnImpact->name = def->elemDefs[i].effectOnImpact->handle->name;
			}
			if(def->elemDefs[i].effectOnDeath)
			{
				addFxEffectDef(info, def->elemDefs[i].effectOnDeath->handle->name, (char*)def->elemDefs[i].effectOnDeath->handle, 0);
				def->elemDefs[i].effectOnDeath->name = def->elemDefs[i].effectOnDeath->handle->name;
			}
			for(int j=0; j<def->elemDefs[i].visualCount; j++)
			{
				if(def->elemDefs[i].elemType == 11)
				{
					if(def->elemDefs[i].visuals.markArray)
					{
						Material* m1 = def->elemDefs[i].visuals.markArray[j].data[0];
						Material* m2 = def->elemDefs[i].visuals.markArray[j].data[1];
						addMaterial(info, m1->name, (char*)m1, 0);
						addMaterial(info, m2->name, (char*)m2, 0);
					}
				}
				else if(def->elemDefs[i].visualCount > 1)
				{
					switch(def->elemDefs[i].elemType)
					{
					case 7:
						{
							XModel* m = def->elemDefs[i].visuals.array[j].xmodel;
							addXModel(info, m->name, (char*)m, 0);
							break;
						}
					case 12:
						{
							FxEffectDefRef* fx = def->elemDefs[i].visuals.array[j].effectDef;
							addFxEffectDef(info, fx->name, (char*)fx, 0);
							def->elemDefs[i].visuals.array[j].effectDef->name = fx->name;
							break;
						}
					case 10:
						{
							const char* str = def->elemDefs[i].visuals.array[j].soundName;
							// do something here later and hope it just works for now.
						}
					default:
						{
							if(def->elemDefs[i].elemType != 8 && def->elemDefs[i].elemType != 9)
							{
								Material* m = def->elemDefs[i].visuals.array[j].material;
								addMaterial(info, m->name, (char*)m, 0);
								break;
							}
						}
					}
				}
			}
		}
	}
	return data;
}