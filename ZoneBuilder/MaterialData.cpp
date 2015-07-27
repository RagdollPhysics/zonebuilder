#include "StdInc.h"
#include "Tool.h"

void writeGfxImage(zoneInfo_t* info, ZStream* buf, GfxImage* data)
{
	GfxImage * img = (GfxImage*)buf->at();

	buf->write(data, sizeof(GfxImage), 1);
	buf->write(data->name, strlen(data->name) + 1, 1);
	img->name = (char*)-1;

	if (data->texture)
	{
		buf->write(data->texture, sizeof(GfxImageLoadDef), 1);
		img->texture = (GfxImageLoadDef*)-1;
	}
}

void writeMaterial(zoneInfo_t* info, ZStream* buf, Material* data)
{
	// require this asset
	int techsetOffset = requireAsset(info, ASSET_TYPE_TECHSET, (char*)data->techniqueSet->name, buf);

	Material* dest = (Material*)buf->at();
	buf->write(data, sizeof(Material), 1);
	buf->write(data->name, strlen(data->name) + 1, 1);
	dest->name = (char*)-1;

	// write techset here
	// we are just going to require it and use the offset
	dest->techniqueSet = (MaterialTechniqueSet*)(techsetOffset);

	// write texturedefs here
	for(int i=0; i<data->textureCount; i++)
	{
		MaterialTextureDef * tex = (MaterialTextureDef*)buf->at();
		buf->write(&data->textureTable[i], sizeof(MaterialTextureDef), 1);
		tex->info.image = (GfxImage*)-1;
	}

	for (int i = 0; i<data->textureCount; i++)
	{
		// TODO, make with work with water images too
		writeGfxImage(info, buf, data->textureTable[i].info.image);
	}

	dest->textureTable = (MaterialTextureDef*)-1;

	if (data->constantTable)
	{
		buf->write((char*)data->constantTable, data->constantCount * sizeof(MaterialConstantDef), 1);
		dest->constantTable = (MaterialConstantDef*)-1;
	}

	if (data->stateBitTable)
	{
		buf->write(data->stateBitTable, data->stateBitsCount * 8, 1);
		dest->stateBitTable = (void*)-1;
	}
}

char baseMatName [64];
map<string, string> materialMaps;
map<string, vec4_t*> materialProperties;

// finally went and made this one sane with our new class for csv files
int parseMatFile(char* data, size_t dataLen)
{
	CSVFile * file = new CSVFile(data, dataLen);
	int curRow = 0;
	char* param = file->getData(curRow, 0);

	// new material format
	if (!strcmp(param, "map") || !strcmp(param, "prop"))
	{
		while (param != NULL)
		{
			if (!strcmp(param, "map"))
			{
				param = file->getData(curRow, 1);
				if (!strcmp("basemat", param))
				{
					strncpy(baseMatName, file->getData(curRow, 2), sizeof(baseMatName));
					curRow++;
					param = file->getData(curRow, 0);
					continue;
				}

				materialMaps[param] = file->getData(curRow, 2);
			}

			if (!strcmp(param, "prop"))
			{
				char vecstr[256];
				strncpy(vecstr, file->getData(curRow, 2), 256);
				vec4_t* vec = (vec4_t*)malloc(sizeof(vec4_t)); // this is a memory leak but idk where to free it
				sscanf_s(vecstr, "(%g, %g, %g, %g)", vec[0], vec[1], vec[2], vec[3]);
				materialProperties[file->getData(curRow, 1)] = vec;
			}

			curRow++;
			param = file->getData(curRow, 0);
		}
	}

	// fallback format
	while(param != NULL)
	{
		if(!strcmp("basemat", param))
		{
			strncpy(baseMatName, file->getData(curRow, 1), sizeof(baseMatName));
			curRow++;
			param = file->getData(curRow, 0);
			continue;
		}

		materialMaps[param] = file->getData(curRow, 1);

		curRow++;
		param = file->getData(curRow, 0);
	}
	return 0;
}

_IWI* LoadIWIHeader(const char* name)
{
	char fname[64] = { 0 };
	_snprintf(fname, sizeof(fname), "images/%s.iwi", name);

	_IWI* buf = new _IWI;
	int handle = 0;

	FS_FOpenFileRead(fname, &handle, 1);

	if (handle == 0)
	{
		Com_Error(1, "Image does not exist: %s!", fname);
		delete buf;
		return NULL;
	}

	FS_Read(buf, sizeof(_IWI), handle);
	FS_FCloseFile(handle);
	return buf;
}

GfxImageLoadDef* GenerateLoadDef(GfxImage* image, short iwi_format )
{
	GfxImageLoadDef* texture = new GfxImageLoadDef;
	memset(texture, 0, sizeof(GfxImageLoadDef));

	switch (iwi_format)
	{
	case IWI_ARGB:
		texture->format = 21;
		break;
	case IWI_RGB8:
		texture->format = 20;
		break;
	case IWI_DXT1:
		texture->format = 0x31545844;
		break;
	case IWI_DXT3:
		texture->format = 0x33545844;
		break;
	case IWI_DXT5:
		texture->format = 0x35545844;
		break;
	}
	texture->dimensions[0] = image->width;
	texture->dimensions[1] = image->height;
	texture->dimensions[2] = image->depth;

	return texture;
}

GfxImage* LoadImageFromIWI(const char* name, char semantic, char category, char flags)
{
	GfxImage* ret = new GfxImage;
	memset(ret, 0, sizeof(GfxImage));

	_IWI* buf = LoadIWIHeader(name);

	ret->height = buf->xsize;
	ret->width = buf->ysize;
	ret->depth = buf->depth;
	ret->dataLen1 = buf->mipAddr4 - 32;
	ret->dataLen2 = buf->mipAddr4 - 32;
	ret->name = strdup(name);
	ret->semantic = semantic;
	ret->category = category;
	ret->flags = flags;
	ret->mapType = 3; // hope that works lol

	ret->texture = GenerateLoadDef(ret, buf->format);

	delete buf;

	return ret;
}

void * addMaterial(zoneInfo_t* info, const char* name, char* data, int dataLen)
{
	if (data == NULL) return NULL;

	if (dataLen < 0)
	{
		Material* mat = (Material*)data;
		for (int i = 0; i < mat->textureCount; i++)
		{
			_IWI* buf = LoadIWIHeader(mat->textureTable[i].info.image->name);
			mat->textureTable[i].info.image->texture = GenerateLoadDef(mat->textureTable[i].info.image, buf->format);
			delete buf;
		}
		addAsset(info, ASSET_TYPE_TECHSET, mat->techniqueSet->name, addTechset(info, mat->techniqueSet->name, (char*)mat->techniqueSet, -1));
		return data;
	}
	else
	{
		parseMatFile(data, dataLen);
	}

	Material* basemat = (Material*)DB_FindXAssetHeader(ASSET_TYPE_MATERIAL, baseMatName);

	// duplicate the material
	Material* mat = new Material;
	memcpy(mat, basemat, sizeof(Material));

	// new info
	mat->name = strdup(name);

	mat->textureTable = new MaterialTextureDef[materialMaps.size()];
	mat->textureCount = materialMaps.size();

	int i = 0;
	for (auto it = materialMaps.begin(); it != materialMaps.end(); ++it)
	{
		MaterialTextureDef* cur = &mat->textureTable[i];
		memset(cur, 0, sizeof(MaterialTextureDef));
		switch (R_HashString(it->first.c_str()))
		{
		case HASH_COLORMAP: cur->nameStart = 'c'; cur->nameHash = HASH_COLORMAP; cur->semantic = SEMANTIC_COLOR_MAP;  break;
		case HASH_DETAILMAP: cur->nameStart = 'd'; cur->nameHash = HASH_DETAILMAP; cur->semantic = SEMANTIC_FUNCTION;  break;
		case HASH_SPECULARMAP: cur->nameStart = 's'; cur->nameHash = HASH_SPECULARMAP; cur->semantic = SEMANTIC_SPECULAR_MAP;  break;
		case HASH_NORMALMAP: cur->nameStart = 'n'; cur->nameHash = HASH_NORMALMAP; cur->semantic = SEMANTIC_NORMAL_MAP;  break;
		}
		cur->nameEnd = 'p';
		cur->info.image = LoadImageFromIWI(it->second.c_str(), cur->semantic, 0, 0);
		i++;
	}

	// only overwrite this stuff if we specifically want to
	if (materialProperties.size()) {
		mat->constantTable = new MaterialConstantDef[materialProperties.size()];
		mat->constantCount = materialProperties.size();

		i = 0;
		for (auto it = materialProperties.begin(); it != materialProperties.end(); ++it)
		{
			MaterialConstantDef* cur = &mat->constantTable[i];
			memset(cur, 0, sizeof(MaterialConstantDef));
			strncpy(cur->name, it->first.c_str(), 12);
			cur->nameHash = R_HashString(it->first.c_str());
			memcpy(cur->literal, it->second, sizeof(vec4_t));

			i++;
		}
	}

	// add techset to our DB here
	// this one is weird and is all handled internally cause of the shit it does
	addAsset(info, ASSET_TYPE_TECHSET, mat->techniqueSet->name, addTechset(info, mat->techniqueSet->name, (char*)mat->techniqueSet, -1));

	return mat;
}

void* addGfxImage(zoneInfo_t* info, const char* name, char* data, int dataLen)
{
	if (dataLen > 0) { Com_Error(false, "Can't create GfxImage's directly. Use Materials\n"); return NULL; }

	char fname[64] = { 0 };
	_snprintf(fname, sizeof(fname), "images/%s.iwi", name);

	_IWI* buf = LoadIWIHeader(name);

	GfxImage* ret = new GfxImage;
	memcpy(ret, data, sizeof(GfxImage));

	ret->texture = GenerateLoadDef(ret, buf->format);

	delete buf;

	return ret;
}