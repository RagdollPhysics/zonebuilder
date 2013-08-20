#include "StdInc.h"

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
	char format; // see above
	short xsize;
	short ysize;
	short misc;
	int mipAddr4;
	int mipAddr3;
	int mipAddr2;
	int mipAddr1;
} _IWI;

int materialMapCount;
char materialMaps[8][16];
char materialTextureNames[8][64];
_IWI iwiHeaders[8];
char techsetName [64];

void parseMatFile(char* data, size_t dataLen)
{
	char* at = data;
	materialMapCount = 0;
	*(data + dataLen) = 0x0;
	while(at < data + dataLen)
	{
		if(materialMapCount == 8) { Com_Error(false, "Exceeded max number of material maps. Ignoring extra.\n"); break; }

		char line[128];
		sscanf(at, "%s", line);
		string l = string(line);
		int split = l.find(',');
		if(!strncmp("techset", line, 7))
		{
			strncpy(techsetName, l.substr(split + 1, strlen(line) - split).c_str(), 64);
			at += strlen(line) + 2;
			continue;
		}
		strncpy(materialMaps[materialMapCount], l.substr(0, split).c_str(), 16);
		strncpy(materialTextureNames[materialMapCount], l.substr(split + 1, strlen(line) - split).c_str(), 64);

		at += strlen(line);

		char fname [64 + 11];
		sprintf(fname, "images/%s.iwi", materialTextureNames[materialMapCount]);
		if(GetFileAttributesA(fname) == INVALID_FILE_ATTRIBUTES) { Com_Error(false, "File %s does not exist!", fname); return; }
		FILE * iwi = fopen(fname, "r");
		fread(&iwiHeaders[materialMapCount], sizeof(_IWI), 1, iwi);
		fclose(iwi);

		materialMapCount++;
	}
	
}

void addMaterial(zoneInfo_t* info, const char* name, char* data, size_t dataLen)
{
	parseMatFile(data, dataLen);

	// load up the techset
	char techsetfname [128];
	_snprintf(techsetfname, 128, "techsets/%s.techset", techsetName);
	loadAsset(info, ASSET_TYPE_TECHSET, techsetfname, techsetName);

	int asset = addAsset(info, ASSET_TYPE_MATERIAL, name, NULL, 0);

	BUFFER* buf = new BUFFER(4096);

	Material* mat = new Material;
	memset(mat, 0, sizeof(Material));
	mat->name = (char*)0xFFFFFFFF;
	mat->flags = 0x2F;
	mat->animationX = 1;
	mat->animationY = 1;
	mat->unknown2 = 0xFFFFFFFF;
	mat->unknown3 = 0xFFFFFF00;
	memset(mat->unknown4, 0xFF, sizeof(mat->unknown4));
	mat->numMaps = materialMapCount;
	mat->stateMapCount = 1;
	mat->unknown6 = 3;
	mat->unknown7 = 4;

	// null dem pointers!
	mat->techniqueSet = (MaterialTechniqueSet*)0x0;//0xFFFFFFFF;
	mat->maps = (MaterialTextureDef*)0xFFFFFFFF;
	mat->stateMap = (void*)0xFFFFFFFF;

	buf->write(mat, sizeof(Material), 1);
	buf->write((void*)name, strlen(name) + 1, 1);

	// techset
	int assetPatchTo = containsAsset(info, ASSET_TYPE_TECHSET, techsetName);
	addFixup(info, asset, 80, assetPatchTo);

	// maps
	for(int i=0; i<materialMapCount; i++)
	{
		MaterialTextureDef* tex = new MaterialTextureDef;
		memset(tex, 0, sizeof(MaterialTextureDef));
		tex->firstCharacter = materialMaps[i][0];
		tex->secondLastCharacter = materialMaps[i][strlen(materialMaps[i])];
		tex->typeHash = R_HashString(materialMaps[i]);
		tex->image = (GfxImage*)0xFFFFFFFF;
		tex->textureType = 0xE2;

		buf->write(tex, sizeof(MaterialTextureDef), 1);

		GfxImage* img = new GfxImage;
		memset(img, 0, sizeof(GfxImage));
		img->depth = 1;
		img->textureType = 3; // 2d texture
		img->textureType2 = 3;
		img->texture = (GfxImageLoadDef*)0xFFFFFFFF;
		img->name = (char*)0xFFFFFFFF;
		img->width = iwiHeaders[i].xsize;
		img->height = iwiHeaders[i].ysize;

		buf->write(img, sizeof(GfxImage), 1);
		buf->write((void*)materialTextureNames[i], strlen(materialTextureNames[i]) + 1, 1);

		GfxImageLoadDef * def = new GfxImageLoadDef;
		memset(def, 0, sizeof(GfxImageLoadDef));
		int format = 0;
		switch(iwiHeaders[i].format)
		{
		case IWI_ARGB:
			format = 21;
			break;
		case IWI_RGB8:
			format = 20;
			break;
		case IWI_DXT1:
			format = 0x31545844;
			break;
		case IWI_DXT3:
			format = 0x33545844;
			break;
		case IWI_DXT5:
			format = 0x35545844;
			break;
		}
		def->format = format;
		def->mipLevels = 1;

		buf->write(def, sizeof(GfxImageLoadDef), 1);
	}

	// unknown 8 goes here whenever we use it

	// statemap
	char statemap[] = {0x65, 0x51, 0x12, 0x18, 0x02, 0x00, 0x0E, 0xE0 };
	buf->write(statemap, 8, 1);

	buf->resize(-1);

	// fix the data
	setAssetData(info, asset, buf->data(), buf->getsize());
}