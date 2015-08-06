#include "StdInc.h"
#include "Tool.h"

void writeTracer(zoneInfo_t* info, ZStream* buf, Tracer* data)
{
	data->material = (Material*)requireAsset(info, ASSET_TYPE_MATERIAL, (char*)data->material->name, buf);

	WRITE_ASSET(data, Tracer);
	buf->pushStream(ZSTREAM_VIRTUAL);
	WRITE_NAME(data);

	buf->popStream();
}

void * addTracer(zoneInfo_t* info, const char* name, char* data, int dataLen)
{
	if (dataLen > 0)  { Com_Error(false, "Can only export built in Tracers!");  return NULL; }

	Tracer* t = (Tracer*)data;
	addAsset(info, ASSET_TYPE_MATERIAL, t->material->name, addMaterial(info, t->material->name, (char*)t->material, -1));

	return t;
}