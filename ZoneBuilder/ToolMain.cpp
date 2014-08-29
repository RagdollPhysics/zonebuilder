#include "StdInc.h"
#include "Tool.h"
int desiredFFVersion = 277;

char header[] = {'I', 'W', 'f', 'f', 'u', '1', '0', '0', 
	desiredFFVersion, desiredFFVersion >> 8, desiredFFVersion >> 16, desiredFFVersion >> 24, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

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
		if(DB_IsAssetDefault(type, name)) {
			Com_Error(false, "Missing asset %s!\n", name);
			return;
		}
		size = 0;
	}
	else
	{
		size = FS_ReadFile(filename, (void**)&data);
		if(size < 0) // renamed stock asset
		{
			data = (char*)DB_FindXAssetHeader(type, filename);
			if(DB_IsAssetDefault(type, filename)) {
				Com_Error(false, "Missing asset %s!\n", filename);
				return;
			}
			((Rawfile*)data)->name = name;
			size = 0;
		}
	}

	

	void * asset;
	switch(type)
	{
		case ASSET_TYPE_XANIM:
			asset = addXAnim(info, name, data, size);
			break;
		case ASSET_TYPE_RAWFILE:
			asset = addRawfile(info, name, data, size);
			break;
		case ASSET_TYPE_MATERIAL:
			asset = addMaterial(info, name, data, size);
			break;
		case ASSET_TYPE_XMODEL:
			asset = addXModel(info, name, data, size);
			break;
		case ASSET_TYPE_COL_MAP_MP:
			asset = addColMap(info, name, data, size);
			break;
		case ASSET_TYPE_MAP_ENTS:
			asset = addMapEnts(info, name, data, size);
			break;
		case ASSET_TYPE_COM_MAP:
			asset = addComWorld(info, name, data, size);
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
		case ASSET_TYPE_FX:
			asset = addFxEffectDef(info, name, data, size);
			break;
	}
	addAsset(info, type, name, asset);
	if(size != 0)
		FS_FreeFile(data);
}

void loadAssetsFromFile(zoneInfo_t* info, const char* file)
{
	if(GetFileAttributesA(file) == INVALID_FILE_ATTRIBUTES) { Com_Error(true, "\nInput file %s does not exist!", file); return; }
	FILE* fp = fopen(file, "rb");
	int csvLength = flength(fp);
	char * buf = new char[csvLength];
	fread(buf, 1, csvLength, fp);
	CSVFile * csv = new CSVFile(buf, csvLength);
	int row = 0;
	char* typeStr = csv->getData(row, 0);
	while(typeStr != NULL)
	{
		int type = getAssetTypeForString(typeStr);
		char* realname = csv->getData(row, 1);
		char* filename = csv->getData(row, 2);
		loadAsset(info, type, filename, realname);

		row++;
		typeStr = csv->getData(row, 0);
	}

	delete[] buf;

	fclose(fp);
}

void ZoneBuild(char* building)
{
	string toBuild = string(building);
#ifdef ZB_DEBUG
	Com_Printf("----ZoneBuilder Startup----");
#endif
	zoneInfo_t * info = getZoneInfo(toBuild.c_str());

	Com_Printf("\nBuilding Zone : %s\n", toBuild.c_str());

	Com_Printf("Loading Assets...");
	char sourceFile[128];
	_snprintf(sourceFile, 128, "zone_source/%s.csv", toBuild.c_str());
	loadAssetsFromFile(info, sourceFile);
	
	doLastAsset(info, toBuild.c_str());

	Com_Printf("Writing Zone...");
	BUFFER* buf = writeZone(info);
#if ZB_DEBUG
    FILE* fp = fopen("uncompressed_zone", "wb");
    buf->writetofile(fp);
    fclose(fp);
#endif
	Com_Printf("Compressing Zone...");
	BUFFER* compressed = buf->compressZlib();
	Com_Printf("Writing to Disk...");
	CreateDirectoryA("zone", NULL);
	FILE* out = fopen(toBuild.insert(0, va("zone\\%s\\", ((char*(*)())0x45CBA0)())).append(".ff").c_str(), "wb");
	_setmode( _fileno( out ), _O_BINARY ); // it was fucking up zlib output
	FILETIME time;
	GetSystemTimeAsFileTime(&time);
	*((int*)&header[13]) = time.dwHighDateTime;
	*((int*)&header[17]) = time.dwLowDateTime;
	fwrite(header, 21, 1, out);
	compressed->writetofile(out);
	fclose(out);
	Com_Printf("Done!\n");

	if(IsDebuggerPresent()) {
		getchar();
	}
}
