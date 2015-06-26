#include "StdInc.h"
#pragma once

struct XAnimNotifyInfo
{
	unsigned __int16 name;
	float time;
};

struct XAnimPartTransFrames
{
	float mins[3];
	float size[3];
	char* frames;
	char indices[1];
};

union XAnimPartTransData
{
	XAnimPartTransFrames frames;
	float frame0[3];
};

struct XAnimPartTrans
{
	unsigned __int16 size;
	char smallTrans;
	XAnimPartTransData u;
};

struct XAnimDeltaPartQuatDataFrames2
{
	__int16 *frames;
	char indices[1];
};

union XAnimDeltaPartQuatData2
{
	XAnimDeltaPartQuatDataFrames2 frames;
	__int16 frame0[2];
};

struct XAnimDeltaPartQuat2
{
	unsigned __int16 size;
	XAnimDeltaPartQuatData2 u;
};

struct XAnimDeltaPartQuatDataFrames
{
	__int16 *frames;
	char indices[1];
};

union XAnimDeltaPartQuatData
{
	XAnimDeltaPartQuatDataFrames frames;
	__int16 frame0[4];
};

struct XAnimDeltaPartQuat
{
	unsigned __int16 size;
	XAnimDeltaPartQuatData u;
};

struct XAnimDeltaPart
{
	XAnimPartTrans *trans;
	XAnimDeltaPartQuat2 *quat2;
	XAnimDeltaPartQuat *quat;
};

struct XAnim
{
	char * name; // 0
	unsigned short dataByteCount; // 4
	unsigned short dataShortCount; // 6
	unsigned short dataIntCount; // 8
	unsigned short randomDataByteCount; // 10 - 0xA
	unsigned short randomDataIntCount;// 12 - 0xC
	unsigned short framecount; // 14 - 0xE
	unsigned short flags; // 16 - 0x10
	char unknown1[8];
	char tagCount;
	char notetrackCount;
	short unknown2;
	short unknown3; // 30 - 0x1E
	int randomDataShortCount; // 32 - 0x20
	unsigned short indexcount; // 36 - 0x24
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
	XAnimNotifyInfo* notetracks; // 80 - 0x50
	XAnimDeltaPart * delta; // 84 - 0x54
	// 88 - 0x58
};