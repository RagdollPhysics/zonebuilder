#include "StdInc.h"
#include "Tool.h"

int zero = 0;
int pad = 0xFFFFFFFF;

enum
{
	XFILE_BLOCK_ALL			= 0,
	XFILE_BLOCK_UNK			= 1,
	XFILE_BLOCK_TEMP		= 2,
	XFILE_BLOCK_PHYSICAL	= 3,
	XFILE_BLOCK_RUNTIME		= 4,
	XFILE_BLOCK_VIRTUAL		= 5,
	XFILE_BLOCK_LARGE		= 6,
	XFILE_BLOCK_CALLBACK	= 7,
	XFILE_BLOCK_VERTEX		= 8,
	XFILE_BLOCK_INDEX		= 9,
};

int zoneStreamSizes[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

static AssetWriter* assetWriters;

// must be called before you write anything in your asset!!!
int requireAsset(zoneInfo_t* info, int type, char* name, BUFFER* buf)
{
	int a = containsAsset(info, type, name);
	if(a >= 0)
	{
		return writeAsset(info, &info->assets[a], buf);
	}
	else
	{
		Com_Error(false, "Missing required asset %s (%d). Export may fail!", name, type);
	}
	return -1;
}

void addXZoneMemory(int index, int num)
{
	zoneStreamSizes[index] += num;
}

int writeAsset(zoneInfo_t* info, asset_t* asset, BUFFER* buf)
{
	if(asset->written) return asset->offset;
	asset->offset = buf->tell() + 1; // WHY IS THIS +1 ??????
	// hide the useless assets that we can't change
	if(asset->type != ASSET_TYPE_TECHSET &&
	   asset->type != ASSET_TYPE_PIXELSHADER &&
	   asset->type != ASSET_TYPE_VERTEXSHADER &&
	   asset->type != ASSET_TYPE_VERTEXDECL)
		Com_Debug("\nWriting asset %s, of type %s at offset 0x%x", ((Rawfile*)asset->data)->name, getAssetStringForType(asset->type), asset->offset);
	switch(asset->type)
	{
	case ASSET_TYPE_RAWFILE:
		assetWriters[ASSET_TYPE_RAWFILE].write(asset->data, buf);
		break;
	case ASSET_TYPE_XANIM:
		writeXAnim(info, buf, (XAnim*)asset->data);
		break;
	case ASSET_TYPE_MATERIAL:
		writeMaterial(info, buf, (Material*)asset->data);
		break;
	case ASSET_TYPE_TECHSET:
		writeTechset(info, buf, (MaterialTechniqueSet*)asset->data);
		break;
	case ASSET_TYPE_PIXELSHADER:
		assetWriters[ASSET_TYPE_PIXELSHADER].write(asset->data, buf);
		break;
	case ASSET_TYPE_VERTEXSHADER:
		assetWriters[ASSET_TYPE_VERTEXSHADER].write(asset->data, buf);
		break;
	case ASSET_TYPE_VERTEXDECL:
		assetWriters[ASSET_TYPE_VERTEXDECL].write(asset->data, buf);
		break;
	case ASSET_TYPE_XMODEL:
		writeXModel(info, buf, (XModel*)asset->data);
		break;
	case ASSET_TYPE_COL_MAP_MP:
		writeColMap(info, buf, (Col_Map*)asset->data);
		break;
	case ASSET_TYPE_MAP_ENTS:
		writeMapEnts(info, buf, (MapEnts*)asset->data);
		break;
	case ASSET_TYPE_COM_MAP:
		writeComWorld(info, buf, (ComWorld*)asset->data);
		break;
	case ASSET_TYPE_GAME_MAP_MP:
	case ASSET_TYPE_GAME_MAP_SP:
		writeGameMap(info, buf, (GameMap_MP*)asset->data);
		break;
	case ASSET_TYPE_STRINGTABLE:
		assetWriters[ASSET_TYPE_STRINGTABLE].write(asset->data, buf);
		break;
	case ASSET_TYPE_SOUND:
		writeSoundAlias(info, buf, (snd_alias_list_t*)asset->data);
		break;
	case ASSET_TYPE_FX:
		writeFxEffectDef(info, buf, (FxEffectDef*)asset->data);
		break;
	}
	asset->written = true;
	return asset->offset;
}

BUFFER* writeZone(zoneInfo_t * info)
{
    BUFFER* buf = new BUFFER(0x10000000);
    buf->seek(40, SEEK_SET);

	assetWriters = createAssetWriters();

    buf->write(&info->scriptStringCount, 4, 1);
    if(info->scriptStringCount > 0) buf->write(&pad, 4, 1);
    else buf->write(&zero, 4, 1);
    buf->write(&info->assetCount, 4, 1);
    buf->write(&pad, 4, 1);

    for(int i=0; i<info->scriptStringCount; i++)
    {
        buf->write(&pad, 4, 1);
    }

    for(int i=0; i<info->scriptStringCount; i++)
    {
        buf->write((void*)info->scriptStrings[i].c_str(), info->scriptStrings[i].length() + 1, 1);
    }

    for(int i=0; i<info->assetCount; i++)
    {
        buf->write(&info->assets[i].type, 4, 1);
        buf->write(-1, 1);
    }

	AssetWriter* writers = createAssetWriters();

    for(int i=0; i<info->assetCount; i++)
    {
		// NO
        //buf->write(info->assets[i].data, info->assets[i].length, 1);
		writeAsset(info, &info->assets[i], buf);
    }

    buf->resize(-1); // should be maxsize

    // YAY... now we get to compute XZoneMemory sizes!
    // got some nice values courtesy of IW4Tool
    zoneStreamSizes[XFILE_BLOCK_ALL] = buf->getsize() - 39; // data length
    zoneStreamSizes[XFILE_BLOCK_TEMP] = (int)(buf->getsize() * 0.4);
    zoneStreamSizes[XFILE_BLOCK_VIRTUAL] = (int)(buf->getsize() * 1.3);
	zoneStreamSizes[XFILE_BLOCK_VERTEX] = (int)(zoneStreamSizes[XFILE_BLOCK_VERTEX] * 1.2);
	zoneStreamSizes[XFILE_BLOCK_INDEX] = (int)(zoneStreamSizes[XFILE_BLOCK_INDEX] * 1.2);

    buf->seek(0, SEEK_SET);
    buf->write(zoneStreamSizes, 40, 1);

	Com_Debug("\nWrote %d assets, and %d script strings\n", info->assetCount, info->scriptStringCount);

    return buf;
}