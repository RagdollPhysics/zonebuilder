#include "StdInc.h"
#include "Tool.h"

void writeMapEnts(zoneInfo_t* info, ZStream* buf, MapEnts* data)
{
	MapEnts* dest = (MapEnts*)buf->at();
	buf->write(data, sizeof(MapEnts), 1);

	buf->pushStream(ZSTREAM_VIRTUAL);

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
		buf->write(data->trigger.models, sizeof(TriggerModel), data->trigger.modelCount);
		dest->trigger.models = (TriggerModel*)-1;
	}

	if (dest->trigger.hulls)
	{
		buf->align(ALIGN_TO_4);
		buf->write(data->trigger.hulls, sizeof(TriggerHull), data->trigger.hullCount);
		dest->trigger.hulls = (TriggerHull*)-1;
	}

	if (dest->trigger.slabs)
	{
		buf->align(ALIGN_TO_4);
		buf->write(data->trigger.slabs, sizeof(TriggerSlab), data->trigger.slabCount);
		dest->trigger.slabs = (TriggerSlab*)-1;
	}

	if(dest->stageNames)
	{
		Stage* stages = (Stage*)buf->at();
		buf->write(data->stageNames, sizeof(Stage), data->stageCount);

		for(int i=0; i<dest->stageCount; i++)
		{
			if (data->stageNames[i].stageName)
			{
				buf->write(data->stageNames[i].stageName, strlen(data->stageNames[i].stageName) + 1, 1);
				stages[i].stageName = (char*)-1;
			}
		}

		dest->stageNames = (Stage*)-1;
	}

	buf->popStream(); // VIRTUAL
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
	asset->stageNames = 0;

	return asset;
}