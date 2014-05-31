#include "StdInc.h"
#include "Tool.h"

void writeMapEnts(zoneInfo_t* info, BUFFER* buf, MapEnts* data)
{
	MapEnts* dest = (MapEnts*)buf->at();
	buf->write(data, sizeof(MapEnts), 1);
	buf->write(data->name, strlen(data->name) + 1, 1);
	dest->name = (char*)-1;

	if(dest->entitystring)
	{
		buf->write(data->entitystring, strlen(data->entitystring) + 1, 1);
		dest->entitystring = (char*)-1;
	}

	if(dest->brushes.unk1)
	{
		buf->write(data->brushes.unk1, 8 * dest->brushes.unkCount1, 1);
		dest->brushes.unk1 = (char*)-1;
	}

	if(dest->brushes.unk2)
	{
		buf->write(data->brushes.unk2, 32 * dest->brushes.unkCount2, 1);
		dest->brushes.unk2 = (char*)-1;
	}

	if(dest->brushes.unk3)
	{
		buf->write(data->brushes.unk3, 20 * dest->brushes.unkCount3, 1);
		dest->brushes.unk3 = (char*)-1;
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

void * addMapEnts(zoneInfo_t* info, const char* name, char* data, size_t dataLen)
{
	if(dataLen == 0)
	{
		MapEnts* asset = (MapEnts*)data;
		char bspName[256];
		_snprintf(bspName, 256, "maps/mp/%s.d3dbsp", info->name);
		asset->name = strdup(bspName);
		return data;
	}

	MapEnts* asset = new MapEnts;
	memset(asset, 0, sizeof(MapEnts));
	asset->name = strdup(name);
	asset->entitystring = strdup(data);
	asset->entitystrlen = dataLen;
	asset->stageCount = 0;
	asset->stages = 0;

	return asset;
}