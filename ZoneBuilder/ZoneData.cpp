#include "StdInc.h"
#include "Tool.h"

zoneInfo_t * getZoneInfo(const char* zoneName)
{
	zoneInfo_t * info = new zoneInfo_t;
	info->name = strdup(zoneName);

	info->assets = (asset_t*)malloc(sizeof(asset_t) * MAX_ASSET_COUNT);
	memset(info->assets, 0, sizeof(asset_t) * MAX_ASSET_COUNT);
	info->scriptStrings = new string[MAX_SCRIPT_STRINGS];

	info->assetCount = 0;
	info->scriptStringCount = 0;
	return info;
}

void freeZoneInfo(zoneInfo_t* info)
{
	free(info->assets);
	delete[] info->scriptStrings;

	delete info;
	info = NULL;
}

unsigned int R_HashString(const char* string)
{
	unsigned int hash = 0;

	while (*string)
	{
		hash = (*string | 0x20) ^ (0x21 * hash);
		string++;
	}

	return hash;
}

int containsAsset(zoneInfo_t* info, int type, const char* name)
{
	int str = R_HashString(name);
	for(int i=0; i<info->assetCount; i++)
	{
		if(info->assets[i].type != type) continue;
		if(info->assets[i].name != str) continue;
		return i;
	}
	return -1;
}

int containsScriptString(zoneInfo_t* info, const char* string)
{
	for(int i=0; i<info->scriptStringCount; i++)
	{
		if(!strcmp(info->scriptStrings[i].c_str(), string)) return i;
	}
	return -1;
}

int addAsset(zoneInfo_t* info, int type, const char* name, void* data)
{
	if(info->scriptStringCount > MAX_ASSET_COUNT) Com_Error(true, "Tell Apadayo to increase MAX_ASSET_COUNT!");
	int a = containsAsset(info, type, name);
	if(a >= 0) return a;
	info->assets[info->assetCount].name= R_HashString(name);
	info->assets[info->assetCount].type = type;
	info->assets[info->assetCount].data = data;
	return info->assetCount++;
}

int addScriptString(zoneInfo_t* info, string str)
{
	if(info->scriptStringCount > MAX_SCRIPT_STRINGS) Com_Error(true, "Tell Apadayo to increase MAX_SCRIPT_STRINGS!");
	int a = containsScriptString(info, str.c_str());	
	if(a >= 0) return a;
	info->scriptStrings[info->scriptStringCount] = str;
	info->scriptStringCount++;
	return 0;
}

int addScriptString(zoneInfo_t* info, char* str)
{
	return addScriptString(info, string(str));
}

void doLastAsset(zoneInfo_t* info, const char* name)
{
	Rawfile* data = new Rawfile;
	data->name = strdup(name);
	data->compressedData = "Made With ZoneBuilder";
	data->sizeUnCompressed = 0;
	data->sizeCompressed = strlen(data->compressedData) + 1;
	addAsset(info, ASSET_TYPE_RAWFILE, name, data);
}
