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
	SAT_STREAMED = 0x2,
	SAT_PRIMED = 0x3,
	SAT_COUNT = 0x4,
};

// this is a very over-simplified version that lets us be easy with it
struct SoundFile	// 0x10
{
	snd_alias_type_t type;
	bool exists;
	const char *dir;
	const char *name;
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