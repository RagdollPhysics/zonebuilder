#include "StdInc.h"
#pragma once

#pragma pack(push, 1)
struct SndCurve
{
	char* name;
	char field_4[132];
};
#pragma pack(pop)

struct SpeakerLevels
{
	int speaker;
	int numLevels;
	float levels[2];
};

struct ChannelMap
{
	int entryCount;	// how many entries are used
	SpeakerLevels speakers[6];
};

struct SpeakerMap
{
	bool isDefault;
	const char *name;
	ChannelMap channelMaps[2][2];
};

enum snd_alias_type_t : char
{
	SAT_UNKNOWN = 0x0,
	SAT_LOADED = 0x1,
	SAT_STREAMED = 0x2
};

struct MssSound
{
	char unknown1[8];
	int dataLenth;
	char unknown2[24];
	char * soundData;
};

struct LoadedSound
{
	const char *name;
	MssSound data;
};

struct StreamedSound
{
	const char *dir;
	const char *name;
};

union SoundData
{
	LoadedSound* loaded;
	StreamedSound stream;
};

struct SoundFile	// 0xC
{
	snd_alias_type_t type;
	bool exists;
	SoundData data;
};

struct SoundAlias
{
	const char *name;
	const char *subtitle;
	const char *secondaryAliasName;
	const char *chainAliasName;
	const char *string4;
	SoundFile *soundFile;
	int sequence;
	float volMin;
	float volMax;
	float pitchMin;
	float pitchMax;
	float distMin;
	float distMax;
	int flags;
	float slavePercentage;
	float probability;
	float lfePercentage;
	float centerPercentage;
	int startDelay;
	int pad;
	SndCurve *volumeFalloffCurve;
	float envelopMin;
	float envelopMax;
	float envelopPercentage;
	SpeakerMap *speakerMap;
};

struct SoundAliasList
{
	const char *name;
	SoundAlias *head;
	int count;
};