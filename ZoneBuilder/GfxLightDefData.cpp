#include "StdInc.h"
#include "Tool.h"

void writeGfxLightDef(zoneInfo_t* info, ZStream* buf, GfxLightDef* data)
{
	int offset = NULL;
	if (data->attenuation.image)
	{
		 offset = requireAsset(info, ASSET_TYPE_IMAGE, data->attenuation.image->name, buf);
	}

	WRITE_ASSET(data, GfxLightDef);
	buf->pushStream(ZSTREAM_VIRTUAL);

	WRITE_NAME(data);

	if (data->attenuation.image)
	{
		dest->attenuation.image = (GfxImage*)offset;
	}

	buf->popStream(); // VIRTUAL
}

void * addGfxLightDef(zoneInfo_t* info, const char* name, char* data, int dataLen)
{
	if (dataLen < 0) 
	{
		GfxLightDef* def = (GfxLightDef*)data;
		addAsset(info, ASSET_TYPE_IMAGE, def->attenuation.image->name, addGfxImage(info, def->attenuation.image->name, (char*)def->attenuation.image, -1));
		return data;
	}

	char samplerState = data[0];
	char* imageName = &data[1];

	GfxLightDef* def = new GfxLightDef;
	memset(def, 0, sizeof(GfxLightDef));

	def->name = strdup(name);
	def->attenuation.image = (GfxImage*)addGfxImage(info, imageName, NULL, -1);
	def->attenuation.samplerState = samplerState;
	def->lmapLookupStart = 1;

	return def;
}