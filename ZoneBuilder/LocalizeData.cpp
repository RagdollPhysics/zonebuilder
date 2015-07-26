#include "StdInc.h"
#include "Tool.h"

void writeLocalize(zoneInfo_t* info, ZStream* buf, Localize* data)
{
	WRITE_ASSET(data, Localize);
	if (data->localizedString)
	{
		buf->write(data->localizedString, 1, strlen(data->localizedString) + 1);
		data->localizedString = (const char*)-1;
	}
	WRITE_NAME(data);
}

void* addLocalize(zoneInfo_t* info, const char* name, char* data, size_t dataLen)
{
	if (dataLen > 0) { Com_Error(false, "no custom localized files yet\n"); return NULL; }
	return data;
}
