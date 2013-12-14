//***************************************
// Name: Buffers.cpp
// Author: Apadayo
// Date Started: 6/3/12
// Purpouse: Buffer reading, writing, and
//			 parsing utils
//***************************************

#include "StdInc.h"
#include "Tool.h"

BUFFER::BUFFER(char * buf, size_t size)
{
	_origin = buf;
	_location = _origin;
	_size = size;
	_offset = 0;
	_maxsize = 0;
	_freeOnDestroy = false;
}

BUFFER::BUFFER(size_t size)
{
	_origin = new char[size];
	memset(_origin, 0, size);
	_location = _origin;
	_size = size;
	_offset = 0;
	_maxsize = 0;
	_freeOnDestroy = true;
}
BUFFER::~BUFFER()
{
	if(_freeOnDestroy)
		delete[] _origin;
}

void BUFFER::open(char * buf, size_t size)
{
	_origin = buf;
	_location = buf;
	_size = size;
	_offset = 0;
	_maxsize = 0;
}
void BUFFER::resize(size_t newsize)
{
	if(newsize == -1) newsize = _maxsize;
	if(newsize < _maxsize) return;
	char* newdata = new char[newsize];
	memset(newdata, 0, newsize);
	memcpy(newdata, _origin, newsize);
	if(_freeOnDestroy)
		delete[] _origin;
	_origin = newdata;
	_size = newsize;
	_location = newdata + _offset;
	_freeOnDestroy = true;
}

size_t BUFFER::getsize()
{
	return _size;
}

size_t BUFFER::read(void * dest, size_t size, size_t count)
{
	if((size*count) + _origin > _origin + _size) return EOF;
	memcpy(dest, _location, size*count);
	_offset += size * count;
	_location += size * count;
	if(_offset > _maxsize) _maxsize = _offset;
	return count;
}

size_t BUFFER::write(const void * _str, size_t size, size_t count)
{
	if((size*count) + _location > _origin + _size) 
		resize(_size + size*count + 2048);
	memcpy(_location, _str, size*count);
	seek(size*count, SEEK_CUR);
	if(_offset > _maxsize) _maxsize = _offset;
	return count;
}
size_t BUFFER::write(int value, size_t count)
{
	return write(&value, 4, 1);
}

size_t BUFFER::seek(size_t offset, size_t type)
{
	switch(type)
	{
	case SEEK_SET:
		_offset = offset;
		_location = _origin + offset;
		return _offset;
		break;
	case SEEK_CUR:
		_offset += offset;
		_location += offset;
		return _offset;
		break;
	case SEEK_END:
		if(offset > 0) return NULL;
		_offset = _size + offset;
		_location = _origin + _size + offset;
		return _offset;
		break;
	default:
		return NULL;
	}
}

size_t BUFFER::tell()
{
	return _offset;
}

char * BUFFER::at()
{
	return _location;
}

char * BUFFER::data()
{
	return _origin;
}

size_t BUFFER::readstr(register char* str, size_t max_size)
{
    size_t count = 0;
    do {
        *str = (char) *_location;
        count++;
		_location++;
		_offset++;
    } while ((*str++ != '\0') && (count < max_size));
	if(_offset > _maxsize) _maxsize = _offset;
    return count;
}

void BUFFER::writetofile(FILE * file)
{
	fwrite(_origin, _size, 1, file);
}

bool BUFFER::find(char* _str, size_t len)
{
	int at = _offset;
	while(at < _size) {
		if(!memcmp(_str, _origin+at, len)) {
			_offset = at;
			return true;
		}
		at++;
	}
	return false;
}

BUFFER* BUFFER::compressZlib()
{
	z_stream strm;
	int ret;
	memset(&strm, 0, sizeof(z_stream));
	char* dest = (char*)malloc(_size*2); // WHY IS IT BIGGER!!!!!?
	if(deflateInit(&strm, Z_BEST_COMPRESSION) != Z_OK) { Com_Error(false, "Failed to compress zlib buffer!"); return NULL; }
	strm.next_out = (Bytef*)dest;
	strm.next_in = (Bytef*)_origin;
	strm.avail_out = _size*2;
	strm.avail_in = _size;
	
	ret = deflate(&strm, Z_FINISH);
	if(ret != Z_STREAM_END) { Com_Error(false, "Failed to compress zlib buffer!"); return NULL; }
	ret = deflateEnd(&strm);
	if(ret != Z_OK) { Com_Error(false, "Failed to compress zlib buffer!"); return NULL; }
	return new BUFFER(dest, strm.total_out);
}

