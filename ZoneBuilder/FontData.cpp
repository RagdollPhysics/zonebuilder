#include "StdInc.h"
#include "Tool.h"

void writeFont(zoneInfo_t* info, ZStream* buf, Font* data)
{
	int image = NULL, glowImage = NULL;
	if (data->image)
		image = requireAsset(info, ASSET_TYPE_MATERIAL, data->image->name, buf);
	if (data->glowImage)
		glowImage = requireAsset(info, ASSET_TYPE_MATERIAL, data->glowImage->name, buf);
	WRITE_ASSET(data, Font);
	WRITE_NAME(data);

	dest->image = (Material*)image;
	dest->glowImage = (Material*)glowImage;

	if (data->characters)
	{
		buf->write(data->characters, sizeof(FontEntry), data->entries);
		dest->characters = (FontEntry*)-1;
	}
}

void * addFont(zoneInfo_t* info, const char* name, char* data, int dataLen)
{
	if (dataLen > 0) { Com_Error(false, "Can't add new fonts!"); return NULL; }

	Font* font = (Font*)data;

	addAsset(info, ASSET_TYPE_MATERIAL, font->image->name, addMaterial(info, font->image->name, (char*)font->image, -1));
	addAsset(info, ASSET_TYPE_MATERIAL, font->glowImage->name, addMaterial(info, font->glowImage->name, (char*)font->glowImage, -1));

	return data;
}