#include "StdInc.h"
#include "Tool.h"

int zero = 0;
int pad = 0xFFFFFFFF;

int zoneStreamSizes[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

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
	Com_Debug("\nWriting asset %s, of type %s at offset 0x%x", ((Rawfile*)asset->data)->name, getAssetStringForType(asset->type), asset->offset);
	switch(asset->type)
	{
	case ASSET_TYPE_RAWFILE:
		writeRawfile(info, buf, (Rawfile*)asset->data);
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
		writePixelShader(info, buf, (PixelShader*)asset->data);
		break;
	case ASSET_TYPE_VERTEXSHADER:
		writeVertexShader(info, buf, (VertexShader*)asset->data);
		break;
	case ASSET_TYPE_VERTEXDECL:
		writeVertexDecl(info, buf, (VertexDecl*)asset->data);
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
		writeStringTable(info, buf, (StringTable*)asset->data);
		break;
	case ASSET_TYPE_SOUND:
		writeSoundAlias(info, buf, (snd_alias_list_t*)asset->data);
		break;
	}
	asset->written = true;
	return asset->offset;
}

BUFFER* writeZone(zoneInfo_t * info)
{
    BUFFER* buf = new BUFFER(0x10000000);
    buf->seek(40, SEEK_SET);

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

    for(int i=0; i<info->assetCount; i++)
    {
		// NO
        //buf->write(info->assets[i].data, info->assets[i].length, 1);
		writeAsset(info, &info->assets[i], buf);
    }

    buf->resize(-1); // should be maxsize

    // YAY... now we get to compute XZoneMemory sizes!
    // got some nice values courtesy of IW4Tool
    zoneStreamSizes[0] = buf->getsize() - 39; // data length
    zoneStreamSizes[2] = (int)(buf->getsize() * 0.4);
    zoneStreamSizes[5] = (int)(buf->getsize() * 1.3);
	zoneStreamSizes[8] = (int)(zoneStreamSizes[8] * 1.2);
	zoneStreamSizes[9] = (int)(zoneStreamSizes[9] * 1.2);

    buf->seek(0, SEEK_SET);
    buf->write(zoneStreamSizes, 40, 1);

	Com_Debug("\nWrote %d assets, and %d script strings\n", info->assetCount, info->scriptStringCount);

    return buf;
}