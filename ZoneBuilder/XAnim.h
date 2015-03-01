#include "StdInc.h"
#pragma once

typedef struct
{
	int name; // use DB_GetFFString
	float frame; // something like that
} Notetrack;

struct XAnimDeltaTransData
{
	char pad[24];
	char* unk;
}; // more data follows struct

struct XAnimDeltaTrans
{
	short deltaBase;
	short deltaBase2;
	union
	{
		float rawData[3];
		XAnimDeltaTransData data;
	};
};

struct XAnimDeltaQuatData
{
	char* data;
}; // more data follows end of struct

struct XAnimDeltaQuat
{
	short deltaBase;
	short deltaBase2;
	union
	{
		int rawData;
		XAnimDeltaQuatData data;
	};
};

struct XAnimDeltaUnkData
{
	char* data;
}; // more data follows end of struct, as usual

struct XAnimDeltaUnk
{
	short deltaBase;
	short deltaBase2;
	union
	{
		__int64 rawData;
		XAnimDeltaUnkData data;
	};
};

struct XAnimDeltaPart
{
	XAnimDeltaTrans* trans;
	XAnimDeltaQuat* quat;
	XAnimDeltaUnk* unk;
};


struct XAnim
{
	char * name; // 0
	short dataByteCount; // 4
	short dataShortCount; // 6
	short dataIntCount; // 8
	short randomDataByteCount; // 10 - 0xA
	short randomDataIntCount;// 12 - 0xC
	short framecount; // 14 - 0xE
	short flags; // 16 - 0x10
	char unknown1[8];
	char tagCount;
	char notetrackCount;
	short unknown2;
	short unknown3; // 30 - 0x1E
	int randomDataShortCount; // 32 - 0x20
	short indexcount; // 36 - 0x24
	short unknown4; // 38 - 0x26
	float framerate; // 40 - 0x28
	float frequency; // 44 - 0x2C
	short * tagnames; // 48 - 0x30
	char *dataByte;// 52 - 0x34
	short *dataShort; // 56 - 0x38
	int *dataInt; // 60 - 0x3C
	short *randomDataShort; // 64 - 0x40
	char *randomDataByte; // 68 - 0x44
	int *randomDataInt; // 72 - 0x48
	char* indicies; // 76 - 0x4C
	Notetrack* notetracks; // 80 - 0x50
	XAnimDeltaPart * delta; // 84 - 0x54
	// 88 - 0x58
};