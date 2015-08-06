#include "StdInc.h"
#include "Tool.h"

void writeLocalize(zoneInfo_t* info, ZStream* buf, Localize* data)
{
	WRITE_ASSET(data, Localize);
	buf->pushStream(ZSTREAM_VIRTUAL);

	if (data->localizedString)
	{
		buf->write(data->localizedString, 1, strlen(data->localizedString) + 1);
		data->localizedString = (const char*)-1;
	}
	WRITE_NAME(data);

	buf->popStream(); // VIRTUAL
}


void upercaseString(char* str)
{
	for (char* c = str; *c; c++)
	{
		*c = toupper(*c);
	}
}

void* addLocalize(zoneInfo_t* info, const char* name, char* data, int dataLen)
{
	if (dataLen < 0) return data;

	char* parse = data;

	char lang[64];
	_snprintf(lang, 64, "LANG_%s", ((char*(*)())0x45CBA0)());

	upercaseString(lang);

	Com_BeginParseSession("parse_localize");

	Com_Parse_MatchToken(&parse, "VERSION", false);
	Com_Parse_MatchToken(&parse, "1", false);
	char * token = Com_ParseExt(&parse);
	while (strncmp("REFERENCE", token, 9))
	{
		token = Com_ParseExt(&parse); // skip all the stuff at the beginning
	}

	char * ref, * entry;

	while (strncmp("ENDMARKER", token, 9))
	{
		if (strlen(token) == 0) { Com_Error(false, "Missing ENDMARKER in localization file."); break; }
		if (!strncmp("REFERENCE", token, 9)) 
		{
			ref = strdup(Com_ParseExt(&parse));
			upercaseString(ref);
			token = Com_ParseExt(&parse);
			continue;
		}

		if (ref)
		{
			if (strncmp("LANG_", token, 5)) {
				Com_Error(false, "Error parsing localization. Expected \"LANG_\""); return NULL;
			}

			if (!strncmp(lang, token, strlen(lang)))
			{
				entry = strdup(Com_ParseExt(&parse));
			}
			else
			{
				Com_ParseExt(&parse); // eat the next token
			}
		}

		if (ref && entry)
		{
			Localize* loc = new Localize;
			loc->name = ref;
			loc->localizedString = entry;
			addAsset(info, ASSET_TYPE_LOCALIZE, loc->name, loc);

			ref = NULL;
			entry = NULL;
		}

		token = Com_ParseExt(&parse);
	}

	Com_EndParseSession();

	return NULL;
}
