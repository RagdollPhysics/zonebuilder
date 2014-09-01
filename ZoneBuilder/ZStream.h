
#pragma once
#include "StdInc.h"

class ZStream
{
private:
	size_t _size;
	size_t _offset;
	char * _location;
	char * _origin;
	size_t _maxsize;
	bool _freeOnDestroy;
	int _streamOffsets[8];

public:
	ZStream() { ZStream(0x100000); }
	ZStream(size_t size);
	~ZStream();
	void resize(size_t newsize);
	size_t getsize();
	size_t write(const void * _str, size_t size, size_t count);
	size_t write(int stream, const void * _str, size_t size, size_t count);
	size_t write(int stream, int value, size_t count);
	char * at();
	char* data();

	void writetofile(FILE * file);
	BUFFER* compressZlib();

	int getStreamOffset(int stream);

	void operator++() {
		_offset++;
		_location++;
	}
	void operator--() {
		_offset--;
		_location--;
	}
	void operator+=(int b) {
		_offset+=b;
		_location+=b;
	}
	void operator-=(int b) {
		_offset-=b;
		_location-=b;
	}
	operator char*() const {
		return _location;
	}

};

#ifndef SEEK_SET
	#define SEEK_SET 0
#endif
#ifndef SEEK_CUR
	#define SEEK_CUR 1
#endif
#ifndef SEEK_END
	#define SEEK_END 2
#endif

enum zStream
{
	ZSTREAM_TEMP,
	ZSTREAM_PHYSICAL,
	ZSTREAM_RUNTIME,
	ZSTREAM_VIRTUAL,
	ZSTREAM_LARGE,
	ZSTREAM_CALLBACK,
	ZSTREAM_VERTEX,
	ZSTREAM_INDEX
};
