#include "StdInc.h"
#include "Tool.h"

// easy
void writePhysPreset(zoneInfo_t* info, ZStream* buf, PhysPreset* data)
{
	WRITE_ASSET(data, PhysPreset);
	WRITE_NAME(data);
	WRITE_STRING(data, sndAliasPrefix);
}

void* addPhysPreset(zoneInfo_t* info, const char* name, char* data, size_t dataLen)
{
	if (dataLen > 0) { Com_Error(false, "no custom physPreset files yet\n"); return NULL; }
	return data;
}
