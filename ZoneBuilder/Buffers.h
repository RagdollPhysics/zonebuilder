//***************************************
// Name: Buffers.h
// Author: Apadayo
// Date Started: 6/3/12
// Purpouse: Header file for Buffers.cpp
//***************************************
#pragma once
#include "StdInc.h"

class BUFFER
{
private:
	size_t _size;
	size_t _offset;
	char * _location;
	char * _origin;
	size_t _maxsize;
	bool _freeOnDestroy;

public:
	BUFFER() { }
	BUFFER(char* buf, size_t size);
	BUFFER(size_t size);
	~BUFFER();
	void open(char* buf, size_t size);
	void resize(size_t newsize);
	size_t getsize();
	size_t read(void * dest, size_t size, size_t count);
	size_t write(const void * _str, size_t size, size_t count);
	size_t write(int value, size_t count);
	size_t seek(size_t offset, size_t type); // same as fseek
	size_t tell();
	char * at();
	char* data();
	size_t readstr(register char* str, size_t max_size);

	void writetofile(FILE * file);
	bool find(char* _str, size_t len);
	BUFFER* compressZlib();
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
