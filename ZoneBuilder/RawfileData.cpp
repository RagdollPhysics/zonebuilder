#include "StdInc.h"
#include "Tool.h"

void writeRawfile(zoneInfo_t* info, ZStream* buf, Rawfile* data)
{
	Rawfile* dest = (Rawfile*)buf->at();
	buf->write(data, sizeof(Rawfile), 1);

	dest->name = (const char*)-1;
	dest->compressedData = (char*)-1;

	buf->write(data->name, strlen(data->name) + 1, 1);
	buf->write(data->compressedData, data->sizeCompressed, 1);
}

void * addRawfile(zoneInfo_t* info, const char* name, char* data, size_t dataLen)
{
	if(dataLen == 0) return data; // no fixups needed here

	z_stream strm;
	memset(&strm, 0, sizeof(z_stream));
	char* dest = new char[dataLen*2];

	strm.next_out = (Bytef*)dest;
	strm.next_in = (Bytef*)data;
	strm.avail_out = dataLen*2;
	strm.avail_in = dataLen;

	if(deflateInit(&strm, Z_BEST_COMPRESSION) != Z_OK) { Com_Error(false, "Failed to compress zlib buffer!"); return NULL; }
	if(deflate(&strm, Z_FINISH) != Z_STREAM_END) { Com_Error(false, "Failed to compress zlib buffer!"); return NULL; }
	if(deflateEnd(&strm) != Z_OK) { Com_Error(false, "Failed to compress zlib buffer!"); return NULL; }

	Rawfile* ret = new Rawfile;
	ret->name = strdup(name);
	ret->sizeCompressed = strm.total_out;
	ret->sizeUnCompressed = dataLen;
	ret->compressedData = dest;

	return ret;
}