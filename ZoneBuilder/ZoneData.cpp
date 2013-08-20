#include "StdInc.h"

zoneInfo_t * getZoneInfo()
{
	zoneInfo_t * info = new zoneInfo_t;

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
		hash = (*string | 0x20) ^ (33 * hash);
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

int addAsset(zoneInfo_t* info, int type, const char* name, char* data, size_t dataLen)
{
	if(data)
	{
		info->assets[info->assetCount].data = (char*)malloc(dataLen);
		memcpy(info->assets[info->assetCount].data, data, dataLen);
		info->assets[info->assetCount].length = dataLen;
	}
	info->assets[info->assetCount].name= R_HashString(name);
	info->assets[info->assetCount].type = type;
	info->assets[info->assetCount].fixups = new map<int, int>();
	return info->assetCount++;
}

void setAssetData(zoneInfo_t * info, int asset, char* data, size_t dataLen)
{
	info->assets[asset].data = (char*)malloc(dataLen);
	memcpy(info->assets[asset].data, data, dataLen);
	info->assets[asset].length = dataLen;
}

void addFixup(zoneInfo_t* info, int assetNum, int offsetInAsset, int toAsset)
{
	asset_t* asset = &info->assets[assetNum];
	(*asset->fixups)[offsetInAsset] = toAsset;
}

int addScriptString(zoneInfo_t* info, string str)
{
	info->scriptStrings[info->scriptStringCount] = str;
	return info->scriptStringCount++;
}

int addScriptString(zoneInfo_t* info, char* str)
{
	return addScriptString(info, string(str));
}

void doLastAsset(zoneInfo_t* info, const char* name)
{
	char* lastAsset = (char*)malloc(256);
	*((int*)lastAsset) = -1;
	*((int*)lastAsset + 1) = 12;
	*((int*)lastAsset + 2) = 0;
	_snprintf((lastAsset + 12), 243, "%s ZoneBuilder", name);
	*((lastAsset + 12 + strlen(name))) = 0;
	addAsset(info, ASSET_TYPE_RAWFILE, name, lastAsset, strlen(name) + 25);
}

void addXAnim(zoneInfo_t* info, const char* name, char* data, size_t dataLen)
{
	BUFFER* buf = new BUFFER(data, dataLen);
	int numTags;
	int numNotetracks;
	buf->read(&numTags, 4, 1);
	buf->read(&numNotetracks, 4, 1);
	char str[128];
	int firstTag = info->scriptStringCount;
	for(int i=0; i<numTags; i++)
	{
		buf->readstr(str, 128);
		addScriptString(info, str);
	}
	int firstNote = info->scriptStringCount;
	for(int i=0; i<numNotetracks; i++)
	{
		buf->readstr(str, 128);
		addScriptString(info, str);
	}
	int header = buf->tell();
	buf->seek(88, SEEK_CUR);

	buf->seek(strlen(buf->at()) + 1, SEEK_CUR);

	for(int i=0; i<numTags; i++)
	{
		int loc = firstTag + i;
		buf->write(&loc, 2, 1);
	}

	for(int i=0; i<numNotetracks; i++)
	{
		int loc = firstNote + i;
		buf->write(&loc, 4, 1);
		buf->seek(4, SEEK_CUR);
	}

	buf->seek(header, SEEK_SET);

	addAsset(info, ASSET_TYPE_XANIM, name, buf->at(), dataLen - buf->tell());
}

void addRawfile(zoneInfo_t* info, const char* name, char* data, size_t dataLen)
{
	z_stream strm;
	int ret;
	memset(&strm, 0, sizeof(z_stream));
	char* dest = (char*)malloc(dataLen);
	if(deflateInit(&strm, Z_BEST_COMPRESSION) != Z_OK) { Com_Error(false, "Failed to compress zlib buffer!"); return; }
	strm.next_out = (Bytef*)dest;
	strm.next_in = (Bytef*)data;
	strm.avail_out = dataLen;
	strm.avail_in = dataLen;
	
	ret = deflate(&strm, Z_FINISH);
	if(ret != Z_STREAM_END) { Com_Error(false, "Failed to compress zlib buffer!"); return; }
	ret = deflateEnd(&strm);
	if(ret != Z_OK) { Com_Error(false, "Failed to compress zlib buffer!"); return; }
	BUFFER buf = BUFFER(dataLen + strlen(name) + 12);
	buf.seek(0, SEEK_SET);
	buf.write(-1, 1);
	buf.write(strm.total_out, 1);
	buf.write(dataLen, 1);
	buf.write(-1, 1);
	buf.write((void*)name, strlen(name) + 1, 1);
	buf.write(dest, strm.total_out, 1);
	buf.resize(-1);
	addAsset(info, ASSET_TYPE_RAWFILE, name, buf.data(), buf.getsize());
	free(dest);
}

