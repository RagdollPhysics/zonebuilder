#include "StdInc.h"
#include "Tool.h"

// easy
void writePhysPreset(zoneInfo_t* info, ZStream* buf, PhysPreset* data)
{
	WRITE_ASSET(data, PhysPreset);
	WRITE_NAME(data);
	WRITE_STRING(data, sndAliasPrefix);
}

void* addPhysPreset(zoneInfo_t* info, const char* name, char* data, int dataLen)
{
	if (dataLen < 0) { return data; }

	PhysPreset* ret = new PhysPreset;
	memset(ret, 0, sizeof(PhysPreset));

	ret->name = strdup(name);

	if (strlen(Info_ValueForKey(data, "sndAliasPrefix")) > 0) ret->sndAliasPrefix = Info_ValueForKey(data, "sndAliasPrefix");
	if (strlen(Info_ValueForKey(data, "mass")) > 0) ret->mass = stof(Info_ValueForKey(data, "mass"));
	if (strlen(Info_ValueForKey(data, "friction")) > 0) ret->friction = stof(Info_ValueForKey(data, "friction"));
	if (strlen(Info_ValueForKey(data, "bounce")) > 0) ret->bounce = stof(Info_ValueForKey(data, "bounce"));
	if (strlen(Info_ValueForKey(data, "bulletForceScale")) > 0) ret->bulletForceScale = stof(Info_ValueForKey(data, "bulletForceScale"));
	if (strlen(Info_ValueForKey(data, "piecesSpreadFraction")) > 0) ret->piecesSpreadFraction = stof(Info_ValueForKey(data, "piecesSpreadFraction"));
	if (strlen(Info_ValueForKey(data, "piecesUpwardVelocity")) > 0) ret->piecesUpwardVelocity = stof(Info_ValueForKey(data, "piecesUpwardVelocity"));

#undef CONTAINS_KEY
#undef PARSE_STRING
#undef PARSE_INT
#undef PARSE_FLOAT
#undef PARSE_BOOL		
	return data;
}
