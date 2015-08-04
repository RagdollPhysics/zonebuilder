
#include "StdInc.h"
#include "Tool.h"

xZoneMemory memory = { 0, 0, { 0, 0, 0, 0, 0, 0, 0, 0 } };

ZStream::ZStream(int scriptStrings, int assets)
{
	int size = 0x10000000;
	_origin = new char[size];
	memset(_origin, 0, size);
	_location = _origin;
	_size = size;
	_offset = 0;
	_maxsize = 0;
	memset(_streamOffsets, 0, sizeof(_streamOffsets));

	pushStream(ZSTREAM_TEMP);

	write(&memory, sizeof(xZoneMemory), 1);

	write(scriptStrings, 4, 1);
	write(scriptStrings > 0 ? (&pad) : (&zero), 4, 1);

	write(&assets, 4, 1);
	write(assets > 0 ? (&pad) : (&zero), 4, 1);
}

ZStream::~ZStream()
{
	delete[] _origin;
}

void ZStream::resize(size_t newsize)
{
	if(newsize == -1) newsize = _maxsize;
	if(newsize < _maxsize) return;

	char* newdata = new char[newsize];
	memcpy(newdata, _origin, newsize);

	if (_freeOnDestroy)
	{
		delete[] _origin;
	}

	_origin = newdata;
	_size = newsize;
	_location = newdata + _offset;
	_freeOnDestroy = true;
}

size_t ZStream::getsize()
{
	return _size;
}

size_t ZStream::write(int stream, const void * _str, size_t size, size_t count)
{
	if ((size*count) + _location > _origin + _size)
	{
		resize(_size + size*count + 2048);
	}

	memcpy(_location, _str, size*count);
	_offset += size*count;
	_location += size*count;

	if(_offset > _maxsize) _maxsize = _offset;

	_streamOffsets[stream] += size*count; // stay up to date on those streams

	return count;
}

size_t ZStream::write(const void * _str, size_t size, size_t count)
{
	return write(_curStream, _str, size, count);
}


size_t ZStream::write(int stream, int value, size_t count)
{
	size_t ret = 0;

	for(size_t i=0; i<count; i++)
	{
		ret += write(stream, &value, 4, 1);
	}

	return ret;
}

char * ZStream::at()
{
	return _location;
}

char * ZStream::data()
{
	return _origin;
}

void ZStream::writetofile(FILE * file)
{
	fwrite(_origin, _size, 1, file);
}

BUFFER* ZStream::compressZlib()
{
	z_stream strm;
	int ret;

	memset(&strm, 0, sizeof(z_stream));
	char* dest = (char*)malloc(_size*2); // WHY IS IT BIGGER!!!!!?

	if(deflateInit(&strm, Z_BEST_COMPRESSION) != Z_OK) { Com_Error(false, "Failed to compress zlib ZStream!"); free(dest); return NULL; }

	strm.next_out = (Bytef*)dest;
	strm.next_in = (Bytef*)_origin;
	strm.avail_out = _size*2;
	strm.avail_in = _size;
	
	ret = deflate(&strm, Z_FINISH);
	if(ret != Z_STREAM_END) { Com_Error(false, "Failed to compress zlib ZStream!"); return NULL; }

	ret = deflateEnd(&strm);
	if(ret != Z_OK) { Com_Error(false, "Failed to compress zlib ZStream!"); return NULL; }

	return new BUFFER(dest, strm.total_out);
}

int ZStream::getStreamOffset(int stream)
{
	return _streamOffsets[stream];
}

void ZStream::updateStreamOffsetHeader()
{
	xZoneMemory* mem = (xZoneMemory*)data();
	mem->zoneSize = getsize() - 39; // data length
	for (int i=0; i<8; i++)
		mem->streams[i] = _streamOffsets[i];
}

void ZStream::pushStream(int stream)
{
	_streamStack.push(_curStream);
	_curStream = stream;
}

void ZStream::popStream()
{
	if (_streamStack.empty()) Com_Error(true, "Tried to pop stream when no streams present on the stack!");
	_curStream = _streamStack.top();
	_streamStack.pop();
}

void ZStream::align(int alignment)
{
	_streamOffsets[_curStream] = (~alignment & (alignment + _streamOffsets[_curStream]));
}

