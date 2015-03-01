#include "StdInc.h"
#include "Tool.h"

/*

void writeComWorld(zoneInfo_t* info, ZStream* buf, ComWorld* data)
{
	ComWorld* dest = (ComWorld*)buf->at();
	buf->write(data, sizeof(ComWorld), 1);
	buf->write(dest->name, strlen(dest->name) + 1, 1); // bsp HAS to be the same name as the zone
	dest->name = (char*)-1;

	if(dest->lights)
	{
		ComPrimaryLight* lights = (ComPrimaryLight*)buf->at();
		buf->write(data->lights, sizeof(ComPrimaryLight), data->lightCount);
		for(int i=0; i<data->lightCount; i++)
		{
			if(data->lights[i].name)
			{
				buf->write(data->lights[i].name, strlen(data->lights[i].name) + 1, 1);
				lights[i].name = (char*)-1;
			}
		}
	}
}

void * addComWorld(zoneInfo_t* info, const char* name, char* data, size_t dataLen)
{
	if(dataLen == 0)
	{
		ComWorld* asset = (ComWorld*)data;
		char bspName[256];
		_snprintf(bspName, 256, "maps/mp/%s.d3dbsp", info->name);
		asset->name = strdup(bspName);
		return data;
	}

	return NULL;
}

*/