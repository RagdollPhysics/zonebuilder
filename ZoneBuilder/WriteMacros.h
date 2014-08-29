#include "StdInc.h"

#define WRITE_ASSET(asset, type) type* dest = (type*)buf->at(); \
		buf->write(asset, sizeof(type), 1)

#define WRITE_ASSET_NUM(asset, type, num) type* dest = (type*)buf->at(); \
		buf->write(asset, sizeof(type), num)

#define WRITE_NAME(asset) buf->write(asset->name, strlen(asset->name) + 1, 1); \
		dest->name = (char*)-1

#define WRITE_FIELD(asset, field, type, count) if(asset->field) { \
			buf->write(asset->field, sizeof(type), asset->count); \
			dest->field = (type*)-1; \
		}

#define WRITE_FIELD_ON_SIZE(asset, field, type, count) 	if(asset->field) \
	{\
		if(asset->count > 255) \
		{ \
			buf->write(asset->field, asset->count * 2, 1); \
		} \
		else \
		{ \
			buf->write(asset->field, asset->count, 1); \
		} \
		dest->field = (type*)-1; \
	}

#define WRITE_STRING(asset, str) buf->write(asset->str, strlen(asset->str) + 1, 1); \
		dest->str = (char*)-1

#define HAS_FIELD(asset, field) asset->field != 0