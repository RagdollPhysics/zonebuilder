#include "StdInc.h"
#pragma once

enum StructuredDataTypeCategory
{
	DATA_INT = 0x0,
	DATA_BYTE = 0x1,
	DATA_BOOL = 0x2,
	DATA_STRING = 0x3,
	DATA_ENUM = 0x4,
	DATA_STRUCT = 0x5,
	DATA_INDEXED_ARRAY = 0x6,
	DATA_ENUM_ARRAY = 0x7,
	DATA_FLOAT = 0x8,
	DATA_SHORT = 0x9,
	DATA_COUNT = 0xA,
};

#pragma pack(push,4)
struct StructuredDataEnumEntry
{
	const char *name;
	unsigned __int16 index;
};
#pragma pack(pop)

struct StructuredDataEnum
{
	int entryCount;
	int reservedEntryCount;
	StructuredDataEnumEntry *entries;
};

union StructuredDataTypeUnion
{
	unsigned int stringDataLength;
	int enumIndex;
	int structIndex;
	int indexedArrayIndex;
	int enumedArrayIndex;
};

struct StructuredDataType
{
	StructuredDataTypeCategory type;
	StructuredDataTypeUnion u;
};

#pragma pack(push,4)
struct StructuredDataStructProperty
{
	const char *name;
	StructuredDataType item;
	int offset;
};
#pragma pack(pop)

struct StructuredDataStruct
{
	int propertyCount;
	StructuredDataStructProperty *properties;
	int size;
	unsigned int bitOffset;
};

struct StructuredDataIndexedArray
{
	int arraySize;
	StructuredDataType elementType;
	unsigned int elementSize;
};

struct StructuredDataEnumedArray
{
	int enumIndex;
	StructuredDataType elementType;
	unsigned int elementSize;
};

struct StructuredDataDef
{
	int version;
	unsigned int formatChecksum;
	int enumCount;
	StructuredDataEnum *enums;
	int structCount;
	StructuredDataStruct *structs;
	int indexedArrayCount;
	StructuredDataIndexedArray *indexedArrays;
	int enumedArrayCount;
	StructuredDataEnumedArray *enumedArrays;
	StructuredDataType rootType;
	unsigned int size;
};

struct StructuredDataDefSet
{
	const char *name;
	unsigned int defCount;
	StructuredDataDef *defs;
};