#include "StdInc.h"
#include "Tool.h"

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
		GfxImage * img = (GfxImage*)buf->at();

		buf->write(data->textureTable[i].info.image, sizeof(GfxImage), 1);
		buf->write(data->textureTable[i].info.image->name, strlen(data->textureTable[i].info.image->name) + 1, 1);
		img->name = (char*)-1;

		if (data->textureTable[i].info.image->texture)
		{
			buf->write(data->textureTable[i].info.image->texture, sizeof(GfxImageLoadDef), 1);
			img->texture = (GfxImageLoadDef*)-1;
		}
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

int materialMapCount;
int materialMaps[8];
char materialTextureNames[8][64];
char baseMatName [64];


// finally went and made this one sane with our new class for csv files
int parseMatFile(char* data, size_t dataLen)
{
	materialMapCount = 0;
	CSVFile * file = new CSVFile(data, dataLen);
	int curRow = 0;
	char* param = file->getData(curRow, 0);

	while(param != NULL)
	{
		if(!strcmp("basemat", param))
		{
			strncpy(baseMatName, file->getData(curRow, 1), sizeof(baseMatName));
			curRow++;
			param = file->getData(curRow, 0);
			continue;
		}

		materialMaps[materialMapCount] = R_HashString(param);
		strncpy(materialTextureNames[materialMapCount++], file->getData(curRow, 1), sizeof(materialTextureNames[0]));

		curRow++;
		param = file->getData(curRow, 0);

		if(materialMapCount == 8) 
		{
			Com_Error(false, "Too many maps in material file! Ignoring extra.");
			return 0;
		}
	}
	return 0;
}

GfxImage* LoadImageFromIWI(char* name, char semantic, char category, char flags)
{
	GfxImage* ret = new GfxImage;
	memset(ret, 0, sizeof(GfxImage));
	ret->texture = new GfxImageLoadDef;
	memset(ret->texture, 0, sizeof(GfxImageLoadDef));

	char fname[64] = { 0 };
	_snprintf(fname, sizeof(fname), "images/%s.iwi", name);

	_IWI* buf = new _IWI;
	int handle = 0;

	FS_FOpenFileRead(fname, &handle, 1);

	if (handle == 0)
	{
		Com_Error(1, "Image does not exist: %s!", fname);
		delete buf;
		delete ret;
		return NULL;
	}

	FS_Read(buf, sizeof(_IWI), handle);
	FS_FCloseFile(handle);

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

	switch (buf->format)
	{
	case IWI_ARGB:
		ret->texture->format = 21;
		break;
	case IWI_RGB8:
		ret->texture->format = 20;
		break;
	case IWI_DXT1:
		ret->texture->format = 0x31545844;
		break;
	case IWI_DXT3:
		ret->texture->format = 0x33545844;
		break;
	case IWI_DXT5:
		ret->texture->format = 0x35545844;
		break;
	}
	ret->texture->dimensions[0] = ret->width;
	ret->texture->dimensions[1] = ret->height;
	ret->texture->dimensions[2] = ret->depth;

	return ret;
}

void * addMaterial(zoneInfo_t* info, const char* name, char* data, size_t dataLen)
{
	if (data == NULL) return NULL;

	if(dataLen == 0) 
	{
		Material* mat = (Material*)data;
		strncpy(baseMatName, mat->name, 64);
		materialMapCount = mat->textureCount;

		for (int i = 0; i<mat->textureCount; i++)
		{
			switch(mat->textureTable[i].nameStart)
			{
			case 'c':
				materialMaps[i] = R_HashString("colorMap");
				break;
			case 'n':
				materialMaps[i] = R_HashString("normalMap");
				break;
			case 's':
				materialMaps[i] = R_HashString("specularMap");
				break;
			case 'd':
				materialMaps[i] = R_HashString("detailMap");
				break;
			}

			strncpy(materialTextureNames[i], mat->textureTable[i].info.image->name, 64);
		}
	}
	else
	{
		parseMatFile(data, dataLen);
	}

	Material* basemat = (Material*)DB_FindXAssetHeader(ASSET_TYPE_MATERIAL, baseMatName);

	// duplicate the material
	Material* mat = new Material;
	memcpy(mat, basemat, sizeof(Material));
	mat->textureTable = new MaterialTextureDef[materialMapCount];

	// new info
	mat->name = strdup(name);
	mat->textureCount = materialMapCount;

	for(int i=0; i<materialMapCount; i++)
	{
		MaterialTextureDef* cur = &mat->textureTable[i];
		memset(cur, 0, sizeof(MaterialTextureDef));
		switch (materialMaps[i])
		{
		case HASH_COLORMAP: cur->nameStart = 'c'; cur->nameHash = HASH_COLORMAP; cur->semantic = SEMANTIC_COLOR_MAP;  break;
		case HASH_DETAILMAP: cur->nameStart = 'd'; cur->nameHash = HASH_DETAILMAP; cur->semantic = SEMANTIC_FUNCTION;  break;
		case HASH_SPECULARMAP: cur->nameStart = 's'; cur->nameHash = HASH_SPECULARMAP; cur->semantic = SEMANTIC_SPECULAR_MAP;  break;
		case HASH_NORMALMAP: cur->nameStart = 'n'; cur->nameHash = HASH_NORMALMAP; cur->semantic = SEMANTIC_NORMAL_MAP;  break;
		}
		cur->nameEnd = 'p';
		cur->info.image = LoadImageFromIWI(materialTextureNames[i], cur->semantic, 0, 0);
	}

	// add techset to our DB here
	// this one is weird and is all handled internally cause of the shit it does
	addTechset(info, mat->techniqueSet->name, (char*)mat->techniqueSet, 0);

	return mat;
}
