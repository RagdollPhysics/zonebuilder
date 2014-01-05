#include "StdInc.h"
#include "Tool.h"
int ffVersion = 277;

char header[] = {'I', 'W', 'f', 'f', 'u', '1', '0', '0', 
	ffVersion, ffVersion >> 8, ffVersion >> 16, ffVersion >> 24, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

void loadAsset(zoneInfo_t* info, int type, const char* filename, const char* name)
{
	if(containsAsset(info, type, name) > 0) return;
	Com_Debug("Loading Asset %s of type %s", name, getAssetStringForType(type));
	if(strlen(filename)) Com_Debug(" (%s)", filename);
	Com_Debug("\n");

	char* data;
	int size;
	// non custom assets
	if(strlen(filename) == 0)
	{
		data = (char*)DB_FindXAssetHeader(type, name);
		size = 0;
	}
	else // custom assets
	{
		size = FS_ReadFile(filename, (void**)&data);
		if(size < 0) { Com_Error(false, "File %s does not exist!", filename); return; }
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
	}
	addAsset(info, type, name, asset);
	if(size != 0)
		FS_FreeFile(data);
}

void loadAssetsFromFile(zoneInfo_t* info, const char* file)
{
	if(GetFileAttributesA(file) == INVALID_FILE_ATTRIBUTES) { Com_Error(false, "\nFile %s does not exist!", file); return; }
	FILE* fp = fopen(file, "r");

	char l[256];
	while (fscanf(fp, "%256[^\n]\n", &l) != EOF)
	{
		string line, assetType, realname, filename;
		line = string(l);
		int first = line.find(",");
		int second = line.find(",", first + 1);
		int endline = line.find("\n");
		assetType = line.substr(0, first);
		realname = line.substr(first + 1, second - first - 1);
		filename = line.substr(second + 1, endline - second);
		int type = getAssetTypeForString(assetType.c_str());
		loadAsset(info, type, filename.c_str(), realname.c_str());
	}

	fclose(fp);
}

void ZoneBuild(char* building)
{
	string toBuild = string(building);
#ifdef _DEBUG
	Com_Printf("----ZoneBuilder Startup----");
#endif
	zoneInfo_t * info = getZoneInfo(toBuild.c_str());

	Com_Printf("\nBuilding Zone : %s\n", toBuild.c_str());

	Com_Printf("Loading assets...\n");
	char sourceFile[128];
	_snprintf(sourceFile, 128, "zone_source/%s.csv", toBuild.c_str());
	loadAssetsFromFile(info, sourceFile);
	
	doLastAsset(info, toBuild.c_str());

	Com_Printf("Writing Zone..... ");
	BUFFER* buf = writeZone(info);
#if _DEBUG
    FILE* fp = fopen("uncompressed_zone", "wb");
    buf->writetofile(fp);
    fclose(fp);
#endif
	Com_Printf("Compressing Zone..... ");
	BUFFER* compressed = buf->compressZlib();
	Com_Printf("Writing to Disk..... ");
	CreateDirectoryA("zone", NULL);
	FILE* out = fopen(toBuild.insert(0, "zone\\english\\").append(".ff").c_str(), "wb");
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
