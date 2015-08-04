#include "StdInc.h"
#pragma once

union XAnimDynamicIndices
{
	char _1[1];
	unsigned __int16 _2[1];
};

union XAnimDynamicFrames
{
	char(*_1)[3];
	unsigned __int16(*_2)[3];
};

struct XAnimNotifyInfo
{
	unsigned __int16 name;
	float time;
};

union XAnimIndices
{
	char* _1;
	unsigned short *_2;
	void* data;
};

struct XAnimPartTransFrames
{
	float mins[3];
	float size[3];
	XAnimDynamicFrames frames;
	XAnimDynamicIndices indices;
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

enum XAnimPartType
{
	PART_TYPE_NO_QUAT = 0x0,
	PART_TYPE_HALF_QUAT = 0x1,
	PART_TYPE_FULL_QUAT = 0x2,
	PART_TYPE_HALF_QUAT_NO_SIZE = 0x3,
	PART_TYPE_FULL_QUAT_NO_SIZE = 0x4,
	PART_TYPE_SMALL_TRANS = 0x5,
	PART_TYPE_TRANS = 0x6,
	PART_TYPE_TRANS_NO_SIZE = 0x7,
	PART_TYPE_NO_TRANS = 0x8,
	PART_TYPE_ALL = 0x9,
};

enum XAnimFlags
{
	XANIM_LOOP_SYNC_TIME = 0x1,
	XANIM_NONLOOP_SYNC_TIME = 0x2,
	XANIM_SYNC_ROOT = 0x4,
	XANIM_COMPLETE = 0x8,
	XANIM_ADDITIVE = 0x10,
	XANIM_CLIENT = 0x20,
	XANIM_SEPARATE = 0x40,
	XANIM_FORCELOAD = 0x80,
	XANIM_PROPOGATE_FLAGS = 0x63,
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
	char pad1; // 16
	char boneCount[10]; // 17
	char notetrackCount; // 27
	bool bLoop; // 28
	bool bDelta; // 29
	char assetType; // 30
	char pad2; // 31
	int randomDataShortCount; // 32 - 0x20
	int indexcount; // 36 - 0x24
	float framerate; // 40 - 0x28
	float frequency; // 44 - 0x2C
	short * tagnames; // 48 - 0x30
	char *dataByte;// 52 - 0x34
	short *dataShort; // 56 - 0x38
	int *dataInt; // 60 - 0x3C
	short *randomDataShort; // 64 - 0x40
	char *randomDataByte; // 68 - 0x44
	int *randomDataInt; // 72 - 0x48
	XAnimIndices indices; // 76 - 0x4C
	XAnimNotifyInfo* notetracks; // 80 - 0x50
	XAnimDeltaPart * delta; // 84 - 0x54
	// 88 - 0x58
};


// used for loading only

union XAnimPartQuatFrames
{
	__int16(*frames)[4];
	__int16(*frames2)[2];
};

struct XAnimPartQuatDataFrames
{
	XAnimPartQuatFrames u;
	XAnimDynamicIndices indices;
};

union XAnimPartQuatData
{
	XAnimPartQuatDataFrames frames;
	__int16 frame0[4];
	__int16 frame02[2];
};

struct XAnimPartQuat
{
	unsigned __int16 size;
	XAnimPartQuatData u;
};

struct XAnimPartQuatPtr
{
	XAnimPartQuat *quat;
	char partIndex;
	char quatType;
};

struct XAnimPartTransPtr
{
	XAnimPartTrans *trans;
	char partIndex;
	char transType;
};