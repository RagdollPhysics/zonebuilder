#include "StdInc.h"
#include "Tool.h"

void writeSoundAlias(zoneInfo_t* info, BUFFER* buf, snd_alias_list_t* data)
{
	snd_alias_list_t* list = (snd_alias_list_t*)buf->at();
	buf->write(data, sizeof(snd_alias_list_t), 1);

	buf->write(data->name, strlen(data->name) + 1, 1);
	list->name = (char*)-1;

	snd_alias_t* aliases = (snd_alias_t*)buf->at();
	buf->write(data->aliases, sizeof(snd_alias_t), data->numAliases);

	list->aliases = (snd_alias_t*)-1;

	for(int i=0; i<data->numAliases; i++)
	{
		snd_alias_t* alias = &aliases[i];
		buf->write(alias->name, strlen(alias->name) + 1, 1);
		alias->name = (char*)-1;
		if(alias->string1) {
			buf->write(alias->string1, strlen(alias->string1) + 1, 1);
			alias->string1 = (char*)-1;
		}
		if(alias->string2) {
			buf->write(alias->string2, strlen(alias->string2) + 1, 1);
			alias->string2 = (char*)-1;
		}
		if(alias->string3) {
			buf->write(alias->string3, strlen(alias->string3) + 1, 1);
			alias->string3 = (char*)-1;
		}
		if(alias->string4) {
			buf->write(alias->string4, strlen(alias->string4) + 1, 1);
			alias->string4 = (char*)-1;
		}

		if(alias->stream)
		{
			StreamFile* file = (StreamFile*)buf->at();
			buf->write(alias->stream, sizeof(StreamFile), 1);
			if(file->type == 1) Com_Error(true, "Cannot export soundAliases that aren't of type 0!");
			if(file->folder) {
				buf->write(file->folder, strlen(file->folder) + 1, 1);
				file->folder = (char*)-1;
			}
			if(file->file)
			{
				buf->write(file->file, strlen(file->file) + 1, 1);
				file->file = (char*)-1;
			}
			alias->stream = (StreamFile*)-1;
		}

		if(alias->sndCurve)
		{
			sndcurve* curve = (sndcurve*)buf->at();
			buf->write(alias->sndCurve, sizeof(sndcurve), 1);
			buf->write(curve->name, strlen(curve->name) + 1, 1);
			curve->name = (char*)-1;
			alias->sndCurve = (sndcurve*)-1;
		}

		if(alias->speakerMap)
		{
			SpeakerMap* map = (SpeakerMap*)buf->at();
			buf->write(alias->speakerMap, sizeof(SpeakerMap), 1);
			buf->write(map->name, strlen(map->name) + 1, 1);
			map->name = (char*)-1;
			alias->speakerMap = (SpeakerMap*)-1;
		}
	}

}

void * addSoundAlias(zoneInfo_t* info, const char* name, char* data, size_t dataLen)
{
	if(dataLen == 0) return data; // no fixups needed
	// why the hell can't I fucking do string parsing?
	string l = string(data);
	int slash = l.find_last_of('/');
	bool hasfolder;
	string folder, file;
	if(slash == -1)
	{
		file = l;
		hasfolder = false;
	}
	else
	{
		folder = l.substr(0, slash + 1);
		file = l.substr(slash + 1);
		hasfolder = true;
	}

	snd_alias_list_t* base = (snd_alias_list_t*)DB_FindXAssetHeader(ASSET_TYPE_SOUND, "AB_1mc_boost");
	snd_alias_list_t* snd = new snd_alias_list_t;
	memcpy(snd, base, sizeof(snd_alias_list_t));
	snd->name = strdup(name);
	snd->aliases->name = snd->name;
	if(hasfolder)
		snd->aliases->stream->folder = (char*)folder.c_str();
	else
		snd->aliases->stream->folder = NULL;
	snd->aliases->stream->file = (char*)file.c_str();

	return snd;
}