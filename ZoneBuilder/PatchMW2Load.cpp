#include "StdInc.h"

#include "Hooking.h"

// zone\dlc patches
CallHook zoneLoadHook1;
DWORD zoneLoadHook1Loc = 0x5BC82C;

CallHook zoneLoadHook2;
DWORD zoneLoadHook2Loc = 0x4CCE08;

char zone_language[MAX_PATH];
char* loadedPath = "";
char* zonePath = "";

dvar_t** fs_basepath = (dvar_t**)0x63D0CD4;

char* GetZoneLocation(const char* name) {
	static char path[MAX_PATH];

	_snprintf(path, MAX_PATH, "%s\\zone\\%s\\%s.ff", (*fs_basepath)->current.string, "alter\\", name);

	if (FileExists(path)) {
		sprintf(path, "%s\\zone\\alter\\", (*fs_basepath)->current.string);
		return path;
	}

	_snprintf(path, MAX_PATH, "%s\\zone\\%s\\%s.ff", (*fs_basepath)->current.string, "dlc\\", name);

	if (FileExists(path)) {
		sprintf(path, "%s\\zone\\dlc\\", (*fs_basepath)->current.string);
		return path;
	}

	DWORD getLang = 0x45CBA0;
	char* language;

	__asm {
		call getLang
		mov language, eax
	}

	_snprintf(path, MAX_PATH, "%s\\zone\\%s\\%s.ff", (*fs_basepath)->current.string, language, name);

	if (FileExists(path)) {
		sprintf(path, "%s\\zone\\%s\\", (*fs_basepath)->current.string, language);
		return path;
	}

	return NULL;
}

char* GetZonePath(const char* fileName)
{
	// we do it a lot simpler than IW did.
	return GetZoneLocation(fileName);
}

void __declspec(naked) ZoneLoadHook1Stub() {
	__asm {
		mov loadedPath, esi // MAKE SURE TO EDIT THIS REGISTER FOR OTHER EXE VERSIONS
	}

	zonePath = GetZonePath(loadedPath);

	__asm {
		mov eax, zonePath
		retn
	}
}

void __declspec(naked) ZoneLoadHook2Stub() {
	__asm {
		mov loadedPath, eax
	}

	zonePath = GetZonePath(loadedPath);

	__asm {
		mov eax, zonePath
		retn
	}
}

CallHook zoneTestHook;
DWORD zoneTestHookLoc = 0x4CCE23;

void ZoneTestHookFunc(char* buffer, int length, const char* format, const char* absPath, const char* zonePath, const char* zoneName)
{
	_snprintf(buffer, length, "%s%s.ff", zonePath, zoneName);
}

CallHook zoneSprintfHook;
DWORD zoneSprintfHookLoc = 0x4B2F15;

void ZoneSprintfHookFunc(char* buffer, int length, const char* format, const char* absPath, const char* zonePath, const char* zoneName)
{
	_snprintf(buffer, length, "%s%s", zonePath, zoneName);
}

void PatchMW2_Load()
{
	zoneLoadHook1.initialize(zoneLoadHook1Loc, ZoneLoadHook1Stub);
	zoneLoadHook1.installHook();

	zoneLoadHook2.initialize(zoneLoadHook2Loc, ZoneLoadHook2Stub);
	zoneLoadHook2.installHook();

	// zone folders -> remove absolute stuff
	zoneTestHook.initialize(zoneTestHookLoc, ZoneTestHookFunc);
	zoneTestHook.installHook();

	zoneSprintfHook.initialize(zoneSprintfHookLoc, ZoneSprintfHookFunc);
	zoneSprintfHook.installHook();
}
