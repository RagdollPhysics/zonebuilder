#include "StdInc.h"
#include "Tool.h"

void writeMaterial(zoneInfo_t* info, BUFFER* buf, Material* data)
{
	// require this asset
	int techsetOffset = requireAsset(info, ASSET_TYPE_TECHSET, (char*)data->techniqueSet->name, buf);

	Material* dest = (Material*)buf->at();
	buf->write(data, sizeof(Material), 1);
	buf->write(data->name, strlen(data->name) + 1, 1);
	dest->name = (char*)-1;

	// write techset here
	// we are just going to require it and use the offset
	dest->techniqueSet = (MaterialTechniqueSet*)(techsetOffset | 0xF0000000);

	// write texturedefs here
	for(int i=0; i<data->numMaps; i++)
	{
		MaterialTextureDef * tex = (MaterialTextureDef*)buf->at();
		buf->write(&data->maps[i], sizeof(MaterialTextureDef), 1);
		tex->image = (GfxImage*)-1;
	}

	for(int i=0; i<data->numMaps; i++)
	{
		GfxImage * img = (GfxImage*)buf->at();
		buf->write(data->maps[i].image, sizeof(GfxImage), 1);
		buf->write(data->maps[i].image->name, strlen(data->maps[i].image->name) + 1, 1);
		img->name = (char*)-1;
		if(img->texture)
		{
			buf->write(img->texture, sizeof(GfxImageLoadDef), 1);
			img->texture = (GfxImageLoadDef*)-1;
		}
	}
	dest->maps = (MaterialTextureDef*)-1;

	if(data->unknown8) {
		buf->write((char*)data->unknown8, data->unknown5 * 32, 1);
		dest->unknown8 = -1;
	}
	if(data->stateMap) {
		buf->write(data->stateMap, data->stateMapCount * 8, 1);	
		dest->stateMap = (void*)-1;
	}
}

enum IWI_COMPRESSION_e
{
	IWI_INVALID = 0x0,
	IWI_ARGB = 0x1,
	IWI_RGB8 = 0x2,
	IWI_DXT1 = 0xB,
	IWI_DXT3 = 0xC,
	IWI_DXT5 = 0xD,
} IWI_COMPRESSION;

typedef struct
{
	char magic[3]; //IWi
	char version; // 8
	int flags;
	short format; // see above
	short xsize;
	short ysize;
	short depth;
	int mipAddr4;
	int mipAddr3;
	int mipAddr2;
	int mipAddr1;
} _IWI;

#define MATERIAL_USAGE_UI 0
#define MATERIAL_USAGE_MODEL 1

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

		if(materialMapCount == 8) {
			Com_Error(false, "Too many maps in material file! Ignoring extra.");
			return 0;
		}
	}
	return 0;
}

GfxImage* LoadImageFromBase(char* name, GfxImage* base)
{
	GfxImage* ret = new GfxImage;
	memcpy(ret, base, sizeof(GfxImage));
	ret->texture = new GfxImageLoadDef;
	memcpy(ret->texture, base->texture, sizeof(GfxImageLoadDef));
	ret->texture->dataSize = 0;
	ret->name = name;

	char fname[64];
	_snprintf(fname, 64, "images/%s.iwi", name);
	_IWI* buf = new _IWI;
	int handle = 0;
	FS_FOpenFileRead(fname, &handle, 1);
	if(handle == 0) { Com_Error(1, "Image does not exist: %s!", fname); delete buf; delete ret; return NULL; }
	FS_Read(buf, sizeof(_IWI), handle);
	FS_FCloseFile(handle);
	ret->height = buf->xsize;
	ret->width = buf->ysize;
	ret->depth = buf->depth;
	switch(buf->format)
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
	ret->dataLen1 = buf->mipAddr4 - 32;
	ret->dataLen2 = buf->mipAddr4 - 32;
	ret->name = strdup(name);
	return ret;
}

void * addMaterial(zoneInfo_t* info, const char* name, char* data, size_t dataLen)
{
	if(dataLen == 0) {
		Material* mat = (Material*)data;
		strncpy(baseMatName, mat->name, 64);
		materialMapCount = mat->numMaps;
		for(int i=0; i<mat->numMaps; i++)
		{
			switch(mat->maps[i].firstCharacter)
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
			strncpy(materialTextureNames[i], mat->maps[i].image->name, 64);
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
	mat->maps = new MaterialTextureDef[mat->numMaps];
	memcpy(mat->maps, basemat->maps, sizeof(MaterialTextureDef) * mat->numMaps);

	// new info
	mat->name = strdup(name);
	for(int j=0; j<materialMapCount; j++)
	{
		int wantedMap = -1;
		for(int i=0; i<mat->numMaps; i++)
		{
			if(mat->maps[i].typeHash == materialMaps[j])
			{
				wantedMap = i;
				break;
			}
		}
		if(wantedMap == -1) continue; // meh who cares
		mat->maps[wantedMap].image = LoadImageFromBase(materialTextureNames[j], mat->maps[wantedMap].image);
	}

	// add techset to our DB here
	// this one is weird and is all handled internally cause of the shit it does
	addTechset(info, mat->techniqueSet->name, (char*)mat->techniqueSet, -1);

	return mat;
}
