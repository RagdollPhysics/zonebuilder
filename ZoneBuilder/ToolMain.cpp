#include "StdInc.h"
#include "Tool.h"
// 276 is original
// 277 is filepointers
int desiredFFVersion = 276;

char header[] = {'I', 'W', 'f', 'f', 'u', '1', '0', '0', 
	desiredFFVersion, desiredFFVersion >> 8, desiredFFVersion >> 16, desiredFFVersion >> 24, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

zoneInfo_t* currentInfo;

void loadAsset(zoneInfo_t* info, int type, const char* filename, const char* name)
{
	if(containsAsset(info, type, name) > 0) return;

	Com_Debug("Loading Asset %s of type %s", name, getAssetStringForType(type));

	if(filename != NULL) Com_Debug(" (%s)", filename);
	Com_Debug("\n");

	char* data;
	int size;

	if(filename == NULL) // stock asset
	{
		data = (char*)DB_FindXAssetHeader(type, name);
		if(DB_IsAssetDefault(type, name)) 
		{
			Com_Debug("Got Default asset! Make sure this is correct\n");
		}

		size = -1;
	}
	else
	{
		size = FS_ReadFile(filename, (void**)&data);

		if(size < 0) // renamed stock asset
		{
			data = (char*)DB_FindXAssetHeader(type, filename);

			if(DB_IsAssetDefault(type, filename)) 
			{
				Com_Debug("Got Default asset! Make sure this is correct\n");
			}
			size = -1;
		}
	}

	void * asset = NULL;

	switch(type)
	{
	case ASSET_TYPE_PHYSPRESET:
		asset = addPhysPreset(info, name, data, size);
		break;
	case ASSET_TYPE_PHYS_COLLMAP:
		asset = addPhysCollmap(info, name, data, size);
		break;
	case ASSET_TYPE_XANIM:
		asset = addXAnim(info, name, data, size);
		break;
	case ASSET_TYPE_RAWFILE:
		asset = addRawfile(info, name, data, size);
		break;
	case ASSET_TYPE_MATERIAL:
		asset = addMaterial(info, name, data, size);
		break;
	case ASSET_TYPE_PIXELSHADER:
		asset = addPixelShader(info, name, data, size);
		break;
	case ASSET_TYPE_VERTEXSHADER:
		asset = addVertexShader(info, name, data, size);
		break;
	case ASSET_TYPE_VERTEXDECL:
		asset = addVertexDecl(info, name, data, size);
		break;
	case ASSET_TYPE_IMAGE:
		asset = addGfxImage(info, name, data, size);
		break;
	case ASSET_TYPE_TECHSET:
		asset = addTechset(info, name, data, size);
		break;
	case ASSET_TYPE_XMODEL:
		asset = addXModel(info, name, data, size);
		break;
	case ASSET_TYPE_XMODELSURFS:
		Com_Error(false, "Can't create XModelSurfs directly. Use XModel.");
		break;
	case ASSET_TYPE_COL_MAP_MP:
		//asset = addColMap(info, name, data, size);
		break;
	case ASSET_TYPE_MAP_ENTS:
		asset = addMapEnts(info, name, data, size);
		break;
	case ASSET_TYPE_COM_MAP:
		//asset = addComWorld(info, name, data, size);
		break;
	case ASSET_TYPE_GAME_MAP_MP:
		asset = addGameMap_MP(info, name, data, size);
		break;
	case ASSET_TYPE_GAME_MAP_SP:
		asset = addGameMap_SP(info, name, data, size);
		type = ASSET_TYPE_GAME_MAP_MP;
		break;
	case ASSET_TYPE_STRINGTABLE:
		asset = addStringTable(info, name, data, size);
		break;
	case ASSET_TYPE_SOUND:
		asset = addSoundAlias(info, name, data, size);
		break;
	case ASSET_TYPE_SNDCURVE:
		asset = addSndCurve(info, name, data, size);
		break;
	case ASSET_TYPE_LOADED_SOUND:
		asset = addLoadedSound(info, name, data, size);
		break;
	case ASSET_TYPE_LIGHTDEF:
		asset = addGfxLightDef(info, name, data, size);
		break;
	case ASSET_TYPE_FONT:
		asset = addFont(info, name, data, size);
		break;
	case ASSET_TYPE_FX:
		asset = addFxEffectDef(info, name, data, size);
		break;
	case ASSET_TYPE_IMPACTFX:
		asset = addFxImpactTable(info, name, data, size);
		break;
	case ASSET_TYPE_LOCALIZE:
		addLocalize(info, name, data, size); // this one is weird b/c there's more than 1 asset in each file
		break;
	case ASSET_TYPE_WEAPON:
	{
		// its either already loaded or we need to load it here
		if (size > 0)
			asset = addWeaponVariantDef(info, name, (char*)filename, strlen(filename));
		else
			asset = addWeaponVariantDef(info, name, data, -1);
		break;
	}
	case ASSET_TYPE_LEADERBOARDDEF:
		asset = addLeaderboardDef(info, name, data, size);
		break;
	case ASSET_TYPE_STRUCTUREDDATADEF:
		asset = addStructuredDataDefSet(info, name, data, size);
		break;
	case ASSET_TYPE_TRACER:
		asset = addTracer(info, name, data, size);
		break;
	case ASSET_TYPE_VEHICLE:
		asset = addVehicleDef(info, name, data, size);
		break;
	case ASSET_TYPE_MPTYPE:
	case ASSET_TYPE_AITYPE:
	case ASSET_TYPE_CHARACTER:
	case ASSET_TYPE_XMODELALIAS:
	case ASSET_TYPE_SNDDRIVERGLOBALS:
	case ASSET_TYPE_ADDON_MAP_ENTS:
		Com_Error(0, "Cannot define a new asset of type %s! Ignoring asset and continuing...\n", getAssetStringForType(type));
		return;
	}

	if (type != ASSET_TYPE_LOCALIZE)
	{
		if (asset == NULL) Com_Error(false, "Failed to add asset %s!\n", name);
		else addAsset(info, type, name, asset);
	}

	if (size > 0 && type != ASSET_TYPE_WEAPON) // weapon loading destroys data for some reason
	{
		FS_FreeFile(data);
	}
}

void loadAssetsFromFile(zoneInfo_t* info, const char* file)
{
	if(GetFileAttributesA(file) == INVALID_FILE_ATTRIBUTES) { Com_Error(true, "\nInput file %s does not exist!", file); return; }

	FILE* fp = fopen(file, "rb");
	int csvLength = flength(fp);
	char* buf = new char[csvLength];
	fread(buf, 1, csvLength, fp);

	CSVFile csv(buf, csvLength);
	int row = 0;

	char* typeStr = csv.getData(row, 0);

	while(typeStr != NULL)
	{
		int type = getAssetTypeForString(typeStr);
		char* realname = csv.getData(row, 1);
		char* filename = csv.getData(row, 2);
		loadAsset(info, type, filename, realname);

		row++;
		typeStr = csv.getData(row, 0);
	}

	delete[] buf;

	fclose(fp);
}

void ZoneBuild(char* building)
{
	debugChecks();
	string toBuild = string(building);

#ifdef ZB_DEBUG
	Com_Printf("----ZoneBuilder Startup----");
#endif

	zoneInfo_t * info = getZoneInfo(toBuild.c_str());
	currentInfo = info;

	Com_Printf("\nBuilding Zone : %s\n", toBuild.c_str());

	Com_Printf("Loading Assets...");

	char sourceFile[128] = { 0 };
	_snprintf(sourceFile, sizeof(sourceFile), "zone_source/%s.csv", toBuild.c_str());

	loadAssetsFromFile(info, sourceFile);
	
	doLastAsset(info, toBuild.c_str());

	Com_Printf("Writing Zone...");
	ZStream* buf = writeZone(info);

#if ZB_DEBUG
    FILE* fp = fopen("uncompressed_zone", "wb");
    buf->writetofile(fp);
    fclose(fp);
#endif

	Com_Printf("Compressing Zone...");
	BUFFER* compressed = buf->compressZlib();
	delete buf;

	Com_Printf("Writing to Disk...");
	CreateDirectoryA("zone", NULL);

	//const char* outputdir = ((char*(*)())0x45CBA0)();
	const char* outputdir = "alter";

	FILE* out = fopen(toBuild.insert(0, va("zone\\%s\\", outputdir)).append(".ff").c_str(), "wb");
	_setmode( _fileno( out ), _O_BINARY ); // it was fucking up zlib output
	
	FILETIME time;
	GetSystemTimeAsFileTime(&time);

	*((int*)&header[13]) = time.dwHighDateTime;
	*((int*)&header[17]) = time.dwLowDateTime;

	fwrite(header, 21, 1, out);
	compressed->writetofile(out);
	delete compressed;

	fclose(out);

	Com_Printf("Done!\n");
}

#if ZB_DEBUG
void buildVerify(int type, const char* name, void* asset)
{
	// images & surfs dont get added
	if (type == ASSET_TYPE_IMAGE || type == ASSET_TYPE_XMODELSURFS) return;

	if (containsAsset(currentInfo, type, name) < 0)
		Com_Error(true, "Invalid zone created!\nSee asset %s of type 0x%x\n", name, type);
	verifyAsset(currentInfo, type, name);
}

void checkVerified()
{
	asset_t* a = nextUnverifiedAsset(currentInfo);
	while (a != NULL)
	{
		Com_Printf("Asset '%s' unverified\n", a->debugName.c_str());
		a = nextUnverifiedAsset(currentInfo);
	}
}
#endif