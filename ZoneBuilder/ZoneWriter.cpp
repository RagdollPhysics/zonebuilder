#include "StdInc.h"
#include "Tool.h"

int zero = 0;
int pad = 0xFFFFFFFF;

typedef struct 
{
	int zoneSize;
	int unk1;
	int streams[8];
} xZoneMemory;

xZoneMemory memory = { 0, 0, { 0, 0, 0, 0, 0, 0, 0, 0 } };

// must be called before you write anything in your asset!!!
int requireAsset(zoneInfo_t* info, int type, const char* name, ZStream* buf)
{
	int a = containsAsset(info, type, name);

	if(a >= 0)
	{		
		int off = writeAsset(info, &info->assets[a], buf);
		// this will make no sense... just let it
		// we are returning the offset to the pointer in the asset index which isn't generated until load time
		// go figure
		if (desiredFFVersion == 276)
			return (3 << 28) | ((info->index_start + (8 * a) + 4) & 0x0FFFFFFF) + 1;
		else if (desiredFFVersion == 277)
			return off | 0xF0000000;
	}
	else
	{
		Com_Error(false, "Missing required asset %s (%d). Export may (and probably will) fail!", name, type);
	}

	return -1;
}

int writeAsset(zoneInfo_t* info, asset_t* asset, ZStream* buf)
{
	if(asset->written) return asset->offset;
	asset->offset = getOffsetForWrite(info, 0x03, buf);

	const char* name = name = getAssetName(asset->type, asset->data);

	// hide the useless assets that we can't change
	if (asset->type != ASSET_TYPE_TECHSET &&
		asset->type != ASSET_TYPE_PIXELSHADER &&
		asset->type != ASSET_TYPE_VERTEXSHADER &&
		asset->type != ASSET_TYPE_VERTEXDECL)
	{
		Com_Debug("\nWriting asset %s, of type %s at offset 0x%x", name, getAssetStringForType(asset->type), (asset->offset));
	}
	else
	{
		Com_Debug_logOnly("\nWriting asset %s, of type %s at offset 0x%x", name, getAssetStringForType(asset->type), (asset->offset));
	}

	switch(asset->type)
	{
	case ASSET_TYPE_PHYSPRESET:
		writePhysPreset(info, buf, (PhysPreset*)asset->data);
		break;
	case ASSET_TYPE_PHYS_COLLMAP:
		writePhysCollmap(info, buf, (PhysGeomList*)asset->data);
		break;
	case ASSET_TYPE_XANIM:
		writeXAnim(info, buf, (XAnim*)asset->data);
		break;
	// ASSET_TYPE_XMODELSURFS - handled by xmodel
	case ASSET_TYPE_XMODEL:
		writeXModel(info, buf, (XModel*)asset->data);
		break;
	case ASSET_TYPE_MATERIAL:
		writeMaterial(info, buf, (Material*)asset->data);
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
	case ASSET_TYPE_TECHSET:
		writeTechset(info, buf, (MaterialTechniqueSet*)asset->data);
		break;
	case ASSET_TYPE_IMAGE:
		writeGfxImage(info, buf, (GfxImage*)asset->data);
		break;
	case ASSET_TYPE_SOUND:
		writeSoundAlias(info, buf, (SoundAliasList*)asset->data);
		break;
	case ASSET_TYPE_SNDCURVE:
		writeSndCurve(info, buf, (SndCurve*)asset->data);
		break;
	case ASSET_TYPE_LOADED_SOUND:
		writeLoadedSound(info, buf, (LoadedSound*)asset->data);
		break;
	case ASSET_TYPE_COL_MAP_MP:
		//writeColMap(info, buf, (Col_Map*)asset->data);
		break;
	case ASSET_TYPE_COM_MAP:
		//writeComWorld(info, buf, (ComWorld*)asset->data);
		break;
	case ASSET_TYPE_GAME_MAP_MP:
	case ASSET_TYPE_GAME_MAP_SP:
		writeGameMap(info, buf, (GameMap_MP*)asset->data);
		break;
	case ASSET_TYPE_MAP_ENTS:
		writeMapEnts(info, buf, (MapEnts*)asset->data);
		break;
	case ASSET_TYPE_FX_MAP:
	case ASSET_TYPE_GFX_MAP:
		Com_Error(true, "How did you get an asset that you can't write?\n");
		break;
	case ASSET_TYPE_LIGHTDEF:
		writeGfxLightDef(info, buf, (GfxLightDef*)asset->data);
		break;
	case ASSET_TYPE_FONT:
		writeFont(info, buf, (Font*)asset->data);
		break;
	case ASSET_TYPE_MENUFILE:
	case ASSET_TYPE_MENU:
		Com_Error(true, "How did you get an asset that you can't write?\n");
		break;
	case ASSET_TYPE_LOCALIZE:
		writeLocalize(info, buf, (Localize*)asset->data);
		break;
	case ASSET_TYPE_WEAPON:
		writeWeaponVariantDef(info, buf, (WeaponVariantDef*)asset->data);
		break;
	case ASSET_TYPE_FX:
		writeFxEffectDef(info, buf, (FxEffectDef*)asset->data);
		break;
	case ASSET_TYPE_IMPACTFX:
		break;
	case ASSET_TYPE_RAWFILE:
		writeRawfile(info, buf, (Rawfile*)asset->data);
		break;
	case ASSET_TYPE_STRINGTABLE:
		writeStringTable(info, buf, (StringTable*)asset->data);
		break;
	case ASSET_TYPE_LEADERBOARDDEF:
		writeLeaderboardDef(info, buf, (LeaderboardDef*)asset->data);
		break;
	case ASSET_TYPE_STRUCTUREDDATADEF:
		Com_Error(true, "How did you get an asset that you can't write?\n");
		break;
	case ASSET_TYPE_TRACER:
		writeTracer(info, buf, (Tracer*)asset->data);
		break;
	case ASSET_TYPE_VEHICLE:
		Com_Error(true, "How did you get an asset that you can't write?\n");
		break;
	}

	asset->written = true;
	return asset->offset;
}

ZStream* writeZone(zoneInfo_t * info)
{
    ZStream* buf = new ZStream(0x10000000);
	buf->write(&memory, sizeof(xZoneMemory), 1);

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

	info->index_start = buf->getStreamOffset(3) - 56;
	info->index_start = alignTo(info->index_start, ALIGN_TO_4); // align it to 4 bytes

	Com_Debug("Index start is at 0x%x", info->index_start);

	int neg1 = -1;
    for(int i=0; i<info->assetCount; i++)
    {
        buf->write(&info->assets[i].type, 4, 1);
        buf->write(&neg1, 4, 1);
    }

    for(int i=0; i<info->assetCount; i++)
    {
		writeAsset(info, &info->assets[i], buf);
    }

    buf->resize(-1); // should be maxsize

    // Grab the Stream sizes... should be different but who cares if we allocate a bit too much memory :)
	xZoneMemory* mem = (xZoneMemory*)buf->data();
    mem->zoneSize = buf->getsize() - 39; // data length
    mem->streams[0] = (int)(buf->getsize() * 0.4);
    mem->streams[3] = (int)(buf->getsize() * 1.3);
	mem->streams[6] = (int)(buf->getStreamOffset(6) * 1.2);
	mem->streams[7] = (int)(buf->getStreamOffset(7) * 1.2);

	Com_Debug("\nWrote %d assets, and %d script strings\n", info->assetCount, info->scriptStringCount);

    return buf;
}