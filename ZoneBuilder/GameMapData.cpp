#include "StdInc.h"
#include "Tool.h"

void writeGameMap(zoneInfo_t* info, ZStream* buf, GameMap_MP* data)
{
	GameMap_MP* dest = (GameMap_MP*)buf->at();
	buf->write(data, sizeof(GameMap_MP), 1);

	buf->pushStream(ZSTREAM_VIRTUAL);

	buf->write(dest->name, strlen(dest->name) + 1, 1); // bsp HAS to be the same name as the zone
	dest->name = (char*)-1;

	if(data->data)
	{
		buf->write(data->data, sizeof(GameMap_Data), 1);

		if (data->data->unk1)
		{
			DebugBreak();
		}

		if (data->data->unk2)
		{
			DebugBreak();
		}
	}

	buf->popStream(); // VIRTUAL
}

void * addGameMap_MP(zoneInfo_t* info, const char* name, char* data, int dataLen)
{
	if(dataLen < 0)
	{
		GameMap_MP* asset =(GameMap_MP*)data;

		char bspName[256] = { 0 };
		_snprintf(bspName, sizeof(bspName), "maps/mp/%s.d3dbsp", info->name);

		asset->name = strdup(bspName);

		return asset;
	}

	return NULL;
}

void * addGameMap_SP(zoneInfo_t* info, const char* name, char* data, int dataLen)
{
	if(dataLen < 0)
	{
		GameMap_MP* asset = new GameMap_MP;

		char bspName[256] = { 0 };
		_snprintf(bspName, sizeof(bspName), "maps/mp/%s.d3dbsp", info->name);

		asset->name = strdup(bspName);
		asset->data = ((GameMap_SP*)data)->data;

		return asset;
	}

	return NULL;
}
