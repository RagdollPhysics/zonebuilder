#include "StdInc.h"
#include "Tool.h"

void writeSndCurve(zoneInfo_t* info, ZStream* buf, SndCurve* data)
{
	WRITE_ASSET(data, SndCurve);
	WRITE_NAME(data);
}

void writeLoadedSound(zoneInfo_t* info, ZStream* buf, LoadedSound* data)
{
	LoadedSound* dest = (LoadedSound*)buf->at();
	buf->write(data, sizeof(LoadedSound), 1);

	buf->write(data->name, 1, strlen(data->name) + 1);
	dest->name = (const char*)-1;

	if (data->data.soundData)
	{
		buf->align(ALIGN_TO_1);
		buf->write(data->data.soundData, 1, data->data.dataLenth);
		dest->data.soundData = (char*)-1;
	}
}

void writeSoundAlias(zoneInfo_t* info, ZStream* buf, SoundAliasList* data)
{
	SoundAliasList* list = (SoundAliasList*)buf->at();
	buf->write(data, sizeof(SoundAliasList), 1);

	buf->write(data->name, strlen(data->name) + 1, 1);
	list->name = (char*)-1;

	SoundAlias* aliases = (SoundAlias*)buf->at();
	buf->write(data->head, sizeof(SoundAlias), data->count);

	list->head = (SoundAlias*)-1;

	for(int i=0; i<data->count; i++)
	{
		SoundAlias* alias = &aliases[i];
		buf->write(alias->name, strlen(alias->name) + 1, 1);
		alias->name = (char*)-1;

		if (alias->subtitle) 
		{
			buf->write(alias->subtitle, strlen(alias->subtitle) + 1, 1);
			alias->subtitle = (char*)-1;
		}
		
		if (alias->secondaryAliasName) 
		{
			buf->write(alias->secondaryAliasName, strlen(alias->secondaryAliasName) + 1, 1);
			alias->secondaryAliasName = (char*)-1;
		}

		if (alias->chainAliasName)
		{
			buf->write(alias->chainAliasName, strlen(alias->chainAliasName) + 1, 1);
			alias->chainAliasName = (char*)-1;
		}

		if(alias->string4) 
		{
			buf->write(alias->string4, strlen(alias->string4) + 1, 1);
			alias->string4 = (char*)-1;
		}

		if (alias->soundFile) // OffsetToPointer
		{
			buf->align(ALIGN_TO_4);
			SoundFile* stream = (SoundFile*)buf->at();
			buf->write(alias->soundFile, sizeof(SoundFile), 1);

			if (alias->soundFile->type == SAT_STREAMED)
			{
				if (alias->soundFile->data.stream.dir) 
				{
					buf->write(alias->soundFile->data.stream.dir, strlen(alias->soundFile->data.stream.dir) + 1, 1);
					stream->data.stream.dir = (char*)-1;
				}

				if (alias->soundFile->data.stream.name)
				{
					buf->write(alias->soundFile->data.stream.name, strlen(alias->soundFile->data.stream.name) + 1, 1);
					stream->data.stream.name = (char*)-1;
				}
			}
			else if (alias->soundFile->type == SAT_LOADED)
			{
				buf->align(ALIGN_TO_4);
				writeLoadedSound(info, buf, alias->soundFile->data.loaded);
				stream->data.loaded = (LoadedSound*)-1;
			}

			alias->soundFile = (SoundFile*)-1;
		}

		if (alias->volumeFalloffCurve)
		{
			buf->align(ALIGN_TO_4);
			writeSndCurve(info, buf, alias->volumeFalloffCurve);
			alias->volumeFalloffCurve = (SndCurve*)-1;
		}

		if(alias->speakerMap) // OffsetToPointer
		{
			buf->align(ALIGN_TO_4);
			SpeakerMap* map = (SpeakerMap*)buf->at();
			buf->write(alias->speakerMap, sizeof(SpeakerMap), 1);
			buf->write(map->name, strlen(map->name) + 1, 1);

			map->name = (char*)-1;
			alias->speakerMap = (SpeakerMap*)-1;
		}
	}

}

void * addSndCurve(zoneInfo_t* info, const char* name, char* data, int dataLen)
{
	if (dataLen > 0) { Com_Error(false, "Can't add new sndCurves!"); return NULL; }
	return data;
}

void * addLoadedSound(zoneInfo_t* info, const char* name, char* data, int dataLen)
{
	if (dataLen > 0) { Com_Error(false, "Can't add new loaded_sounds!"); return NULL; }
	return data;
}

void * addSoundAlias(zoneInfo_t* info, const char* name, char* data, int dataLen)
{
	if (dataLen < 0) 
	{
		SoundAliasList* lst = (SoundAliasList*)data;
		if (lst->head->soundFile->type == SAT_STREAMED || lst->head->soundFile->type == SAT_LOADED)
		{
			return data; // no fixups needed
		}
		Com_Error(false, "Unknown sound type %d!\n", lst->head->soundFile->type);
		return NULL;
	}

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
		folder = l.substr(0, slash);
		file = l.substr(slash + 1);
		hasfolder = true;
	}

	SoundAliasList* base = (SoundAliasList*)DB_FindXAssetHeader(ASSET_TYPE_SOUND, "AB_1mc_boost");
	SoundAliasList* snd = new SoundAliasList;
	memcpy(snd, base, sizeof(SoundAliasList));
	snd->name = strdup(name);
	snd->head->name = snd->name;

	if (hasfolder)
	{
		snd->head->soundFile->data.stream.dir = strdup((char*)folder.c_str());
	}
	else
	{
		snd->head->soundFile->data.stream.dir = NULL;
	}

	snd->head->soundFile->data.stream.name = strdup((char*)file.c_str());

	return snd;
}