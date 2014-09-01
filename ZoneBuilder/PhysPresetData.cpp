#include "StdInc.h"
#include "Tool.h"

// easy
void writePhysPreset(zoneInfo_t* info, ZStream* buf, PhysPreset* data)
{
	WRITE_ASSET(data, PhysPreset);
	WRITE_NAME(data);
	WRITE_STRING(data, sndAliasPrefix);
}