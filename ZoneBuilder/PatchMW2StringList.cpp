// ==========================================================
// IW4M project
// 
// Component: clientdll
// Sub-component: steam_api
// Purpose: A safer reimplementation of scr_stringlist.cpp.
//
// Initial author: NTAuthority
// Started: 2013-02-06
// ==========================================================

#include "StdInc.h"
#include "Tool.h"
#include "Hooking.h"
#include <google/dense_hash_map>

#define MAX_SCRIPT_STRINGS 65535

#define VA_BUFFER_COUNT		4
#define VA_BUFFER_SIZE		32768

static char g_vaBuffer[VA_BUFFER_COUNT][VA_BUFFER_SIZE];
static int g_vaNextBufferIndex = 0;

const char *va_x( const char *fmt, ... )
{
	va_list ap;
	char *dest = &g_vaBuffer[g_vaNextBufferIndex][0];
	g_vaNextBufferIndex = (g_vaNextBufferIndex + 1) % VA_BUFFER_COUNT;
	va_start(ap, fmt);
	int res = _vsnprintf( dest, VA_BUFFER_SIZE, fmt, ap );
	dest[VA_BUFFER_SIZE - 1] = '\0';
	va_end(ap);

	if (res < 0 || res >= VA_BUFFER_SIZE)
	{
		Com_Error(1, "Attempted to overrun string in call to va() - return address 0x%x", _ReturnAddress());
	}

	return dest;
}

static HANDLE stringHeap;

//#define SL_Debug(x, ...) OutputDebugString(va_x(x, __VA_ARGS__))
#define SL_Debug(x, ...) 

struct ScriptStringData;

struct ScriptStringManagementData
{
	int32_t users; // bitmask
	uint32_t references; // reference count
	ScriptStringData* data; // backreference to the data we know about
};

struct ScriptStringData
{
	ScriptStringManagementData* data;
	char* string;
	int length;
};

static uint32_t slFirstFree; // might need to be optimized to be a linked list of free blocks?
static CRITICAL_SECTION slCritSec;
static ScriptStringData slStrings[MAX_SCRIPT_STRINGS];
static google::dense_hash_map<std::string, uint16_t> slHashMap;

void SL_Init()
{
	((void(*)())0x41DC60)(); // MT_Init

	slFirstFree = 1;

	InitializeCriticalSection(&slCritSec);
	EnterCriticalSection(&slCritSec);

	LeaveCriticalSection(&slCritSec);
}

void SL_AddRefToString(uint16_t string);

int SL_GetStringOfSize(const char* string, int system, int size)
{
	EnterCriticalSection(&slCritSec);

	// try to find the string first
	std::string str(string, size);
	google::dense_hash_map<std::string, uint16_t>::const_iterator iter = slHashMap.find(str);

	if (iter != slHashMap.end())
	{
		SL_Debug("GetStringOfSize '%s' found %i\n", string, iter->second);

		SL_AddRefToString(iter->second);

		LeaveCriticalSection(&slCritSec);
		return iter->second;
	}

	// add the string to the list
	ScriptStringData* data = &slStrings[slFirstFree];
	uint16_t stringID = slFirstFree;

	//data->data = new ScriptStringManagementData;
	data->data = (ScriptStringManagementData*)HeapAlloc(stringHeap, 0, sizeof(ScriptStringManagementData));
	//data->string = (char*)malloc(size);
	data->string = (char*)HeapAlloc(stringHeap, 0, size);
	memcpy(data->string, string, size);

	data->length = size;

	data->data->data = data;
	data->data->references = 1;
	data->data->users = system;

	// find the next free string
	for (int i = slFirstFree; i < MAX_SCRIPT_STRINGS; i++)
	{
		if (!slStrings[i].string)
		{
			slFirstFree = i;
			break;
		}
	}

	slHashMap[str] = stringID;

	LeaveCriticalSection(&slCritSec);

	SL_Debug("GetStringOfSize '%s' allocated %i (user %i)\n", string, stringID, system);

	return stringID;
}

int FindStringOfSize(const char* string, int size)
{
	EnterCriticalSection(&slCritSec);

	std::string str(string, size);

	google::dense_hash_map<std::string, uint16_t>::const_iterator iter = slHashMap.find(str);

	if (iter != slHashMap.end())
	{
		LeaveCriticalSection(&slCritSec);

		SL_Debug("FindStringOfSize '%s' found %i\n", string, iter->second);

		return iter->second;
	}

	SL_Debug("FindStringOfSize '%s' - no results!\n", string, iter->second);

	LeaveCriticalSection(&slCritSec);

	return 0;
}

// this function is way more optimized in IW SL
int SL_ConvertFromString(const char* string)
{
	SL_Debug("ConvertFromString '%s'\n", string);

	return FindStringOfSize(string, strlen(string) + 1);
}

void SL_FreeString(uint16_t string)
{
	EnterCriticalSection(&slCritSec);

	ScriptStringData* data = &slStrings[string];
	//delete data->data;
	HeapFree(stringHeap, 0, data->data);

	std::string str(data->string, data->length);

	slHashMap.erase(str);

	SL_Debug("FreeString %i '%s'\n", string, data->string);

	//free((void*)data->string);
	HeapFree(stringHeap, 0, data->string);

	if (string < slFirstFree)
	{
		slFirstFree = string;
	}

	data->data = NULL;
	data->string = NULL;

	LeaveCriticalSection(&slCritSec);
}

void SL_AddRefToString(uint16_t string)
{
	EnterCriticalSection(&slCritSec);
	InterlockedIncrement(&slStrings[string].data->references);

	SL_Debug("AddRefToString %i (new refcount: %i)\n", string, slStrings[string].data->references + 1);

	LeaveCriticalSection(&slCritSec);
}

void SL_RemoveRefToString(uint16_t string)
{
	EnterCriticalSection(&slCritSec);

	SL_Debug("RemoveRefToString %i (new refcount: %i)\n", string, slStrings[string].data->references - 1);

	if (!InterlockedDecrement(&slStrings[string].data->references))
	{
		SL_FreeString(string);
	}
	
	LeaveCriticalSection(&slCritSec);
}

void SL_RemoveRefToStringOfSize(uint16_t string, int size)
{
	SL_RemoveRefToString(string);
}

void Scr_SetString(uint16_t* field, uint16_t string)
{
	if (string)
	{
		SL_AddRefToString(string);
	}

	if (*field)
	{
		SL_RemoveRefToString(*field);
	}

	*field = string;
}

int SL_GetStringLen(uint16_t string)
{
	if (slStrings[string].string)
	{
		//return strlen(slStrings[string].string);
		return slStrings[string].length - 1;
	}

	return 0;
}

void SL_TransferRefToUser(uint16_t string, char user)
{
	EnterCriticalSection(&slCritSec);
	ScriptStringManagementData* data = slStrings[string].data;

	SL_Debug("SL_TransferRefToUser: %i '%s' %i\n", string, slStrings[string].string, user);

	//data->references = 1;
	data->users = user;
	LeaveCriticalSection(&slCritSec);
	//SL_RemoveRefToString(string);
}

void SL_AddUser(uint16_t string, int user)
{
	ScriptStringManagementData* data = slStrings[string].data;

	if (data)
	{
		SL_Debug("SL_AddUser: %i '%s' %i\n", string, slStrings[string].string, user);

		if (!(data->users & user))
		{
			SL_AddRefToString(string);
		}

		_InterlockedOr((long*)&data->users, user);
	}
}

int SL_ConvertToLowercase(uint16_t string, int user)
{
	char tempString[8192];

	EnterCriticalSection(&slCritSec);

	ScriptStringData* data = &slStrings[string];

	int length = data->length;//strlen(data->string);

	if (length < sizeof(tempString))	
	{
		strcpy(tempString, data->string);

		for (int i = 0; i < length; i++)
		{
			tempString[i] = tolower(tempString[i]);
		}

		LeaveCriticalSection(&slCritSec);

		//SL_RemoveRefToString(string);

		return SL_GetStringOfSize(tempString, user, strlen(tempString) + 1);
	}

	LeaveCriticalSection(&slCritSec);

	return string;
}

void SL_ShutdownSystem(int system)
{
	EnterCriticalSection(&slCritSec);

	SL_Debug("SL_ShutdownSystem: %i\n", system);

	for (int i = 0; i < MAX_SCRIPT_STRINGS; i++)
	{
		if (slStrings[i].data)
		{
			ScriptStringManagementData* data = slStrings[i].data;

			if (data->users & system)
			{
				if (data->users == system)
				{
					//SL_FreeString(i);
				}
				else
				{
					SL_RemoveRefToString(i);
				}
			}
		}
	}

	LeaveCriticalSection(&slCritSec);
}

void SL_TransferSystem(int oldSystem, int newSystem)
{
	EnterCriticalSection(&slCritSec);

	for (int i = 0; i < MAX_SCRIPT_STRINGS; i++)
	{
		if (slStrings[i].data)
		{
			ScriptStringManagementData* data = slStrings[i].data;

			if (data->users & oldSystem)
			{
				InterlockedExchange((unsigned long*)&data->users, (data->users & ~oldSystem) | newSystem);
			}
		}
	}

	LeaveCriticalSection(&slCritSec);
}

const char* SL_ConvertToString_(uint16_t string)
{
	SL_Debug("SL_ConvertToString: %i '%s'\n", string, slStrings[string].string);

	return slStrings[string].string;
}

void PatchMW2_StringList()
{
	slHashMap.set_empty_key("*DUMMYDUMMY*");
	slHashMap.set_deleted_key("*DUMMYDUMM*");

	call(0x4D2280, SL_Init, PATCH_JUMP);
	call(0x436B40, SL_GetStringOfSize, PATCH_JUMP);
	call(0x4EC1D0, SL_ConvertToString_, PATCH_JUMP);
	call(0x61BCB0, FindStringOfSize, PATCH_JUMP);
	call(0x469D80, SL_ConvertFromString, PATCH_JUMP);
	call(0x4D9B00, SL_AddRefToString, PATCH_JUMP);
	call(0x4F1500, SL_RemoveRefToStringOfSize, PATCH_JUMP);
	call(0x47CD70, SL_RemoveRefToString, PATCH_JUMP);
	call(0x417730, Scr_SetString, PATCH_JUMP);
	call(0x4401E0, SL_GetStringLen, PATCH_JUMP);
	call(0x40C050, SL_TransferRefToUser, PATCH_JUMP);
	call(0x4B4310, SL_AddUser, PATCH_JUMP);
	call(0x430510, SL_ConvertToLowercase, PATCH_JUMP);
	call(0x4F46D0, SL_ShutdownSystem, PATCH_JUMP);
	call(0x4A44A0, SL_TransferSystem, PATCH_JUMP);

	stringHeap = HeapCreate(0, 1024 * 1024, 0);

	// path_node_constant_t marking function; has some terrible string references
	*(BYTE*)0x4F74B0 = 0xC3;
}