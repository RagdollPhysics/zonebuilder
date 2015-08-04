#include "StdInc.h"
#include "Tool.h"

void writeMapEnts(zoneInfo_t* info, ZStream* buf, MapEnts* data)
{
	MapEnts* dest = (MapEnts*)buf->at();
	buf->write(data, sizeof(MapEnts), 1);
	buf->write(data->name, strlen(data->name) + 1, 1);
	dest->name = (char*)-1;

	if(dest->entityString)
	{
		buf->write(data->entityString, strlen(data->entityString) + 1, 1);
		dest->entityString = (char*)-1;
	}

	if(dest->trigger.models)
	{
		buf->align(ALIGN_TO_4);
		buf->write(data->trigger.models, 8 * dest->trigger.modelCount << 3, 1);
		dest->trigger.models = (TriggerModel*)-1;
	}

	if (dest->trigger.hulls)
	{
		buf->align(ALIGN_TO_4);
		buf->write(data->trigger.hulls, dest->trigger.hullCount << 5, 1);
		dest->trigger.hulls = (TriggerHull*)-1;
	}

	if (dest->trigger.slabs)
	{
		buf->align(ALIGN_TO_4);
		buf->write(data->trigger.slabs, (dest->trigger.slabCount << 2) + dest->trigger.slabCount << 2, 1);
		dest->trigger.slabs = (TriggerSlab*)-1;
	}

	if(dest->stages)
	{
		Stage* stages = (Stage*)buf->at();
		buf->write(data->stages, sizeof(Stage), data->stageCount);

		for(int i=0; i<dest->stageCount; i++)
		{
			buf->write(data->stages[i].name, strlen(data->stages[i].name) + 1, 1);
			stages[i].name = (const char*)-1;
		}

		dest->stages = (Stage*)-1;
	}
}

void * addMapEnts(zoneInfo_t* info, const char* name, char* data, int dataLen)
{
	if(dataLen < 0)
	{
		MapEnts* asset = (MapEnts*)data;

		char bspName[256] = { 0 };
		_snprintf(bspName, sizeof(bspName), "maps/mp/%s.d3dbsp", info->name);

		asset->name = strdup(bspName);
		return data;
	}

	MapEnts* asset = new MapEnts;
	memset(asset, 0, sizeof(MapEnts));
	asset->name = strdup(name);
	asset->entityString = strdup(data);
	asset->numEntityChars = dataLen;
	asset->stageCount = 0;
	asset->stages = 0;

	return asset;
}