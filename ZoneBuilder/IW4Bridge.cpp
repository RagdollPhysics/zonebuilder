#include "StdInc.h"
#include "Hooking.h"
#include "Utils.h"
#include <list>
#include <time.h>
#include <winsock.h>
#include <dbghelp.h>
#include <shellapi.h>
#include "Tool.h"
#include "WeaponDef.h"

void PatchMW2_Load();
void PatchMW2_StringList();
void PatchMW2_CryptoFiles();
void PatchMW2_CModels();
void PatchMW2_FifthInfinity();
void PatchMW2_AssetRestrict();
void doWeaponEntries();

DWORD Sys_InitializeCriticalSections = 0x42F0A0;
DWORD Sys_InitMainThread = 0x4301B0;
DWORD Win_InitLocalization = 0x406D10;
DWORD SL_Init = 0x4D2280;
DWORD Swap_Init = 0x47F390;
DWORD Com_AllocMemInfo = 0x420830;
DWORD PMem_Init = 0x64A020;
DWORD DB_InitThread = 0x4E0FB0;
DWORD Com_InitDvars = 0x60AD10;
DWORD R_RegisterDvars = 0x5196C0;
DWORD init11 = 0x4A62A0;
DWORD FS_InitFilesystem = 0x429080;
DWORD G_SetupWeaponDef = 0x4E1F30;

bool useEntryNames = false;

void doInit()
{
	__asm
	{
		call Sys_InitializeCriticalSections
		call Sys_InitMainThread
		push 0
		call Win_InitLocalization
		add esp, 4
		call SL_Init
		call Swap_Init
		call Com_AllocMemInfo
		call PMem_Init
		call DB_InitThread
		call Com_InitDvars
		call R_RegisterDvars
		call init11
		call FS_InitFilesystem
	}
}

LONG WINAPI CustomUnhandledExceptionFilter(LPEXCEPTION_POINTERS ExceptionInfo)
{
	// step 1: write minidump
	static LPEXCEPTION_POINTERS exceptionData;

	exceptionData = ExceptionInfo;

	// create a temporary stack for these calls
	DWORD* tempStack = new DWORD[16000];
	static DWORD* origStack;

	__asm
	{
		mov origStack, esp
		mov esp, tempStack
		add esp, 0FA00h
		sub esp, 1000h // local stack space over here, sort of
	}

	char error[1024];
	char filename[MAX_PATH];
	__time64_t time;
	tm* ltime;

	_time64(&time);
	ltime = _localtime64(&time);
	strftime(filename, sizeof(filename) - 1, "ZoneBuilder-%Y%m%d%H%M%S.dmp", ltime);
	_snprintf(error, sizeof(error) - 1, "A minidump has been written to %s.", filename);

	HANDLE hFile = CreateFile(filename, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile != INVALID_HANDLE_VALUE)
	{
		MINIDUMP_EXCEPTION_INFORMATION ex;
		memset(&ex, 0, sizeof(ex));
		ex.ThreadId = GetCurrentThreadId();
		ex.ExceptionPointers = exceptionData;
		ex.ClientPointers = FALSE;

		MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpNormal, &ex, NULL, NULL);		

		CloseHandle(hFile);
	}
	else
	{
		_snprintf(error, sizeof(error) - 1, "An error (0x%x) occurred during creating %s.", GetLastError(), filename);
	}

	Com_Error(true, "Fatal error (0x%08x) at 0x%08x.\n%s", ExceptionInfo->ExceptionRecord->ExceptionCode, ExceptionInfo->ExceptionRecord->ExceptionAddress, error);	

	__asm mov esp, origStack

	delete[] tempStack;

	return 0;
}

list<string> constants;
int constantHash[256];
int constantCount = 0;

void GetMaterialConstants(void* varMaterial, int handle)
{
	Material* mat = (Material*)varMaterial;
	for (int i = 0; i < mat->constantCount; i++)
	{
		bool found = false;
		for (int j = 0; j < constantCount; j++)
		{
			if (mat->constantTable[i].nameHash == constantHash[j]) {
				found = true;
				break;
			}
		}
		if (found) continue;
		constants.push_back(mat->constantTable[i].name);
		constantHash[constantCount++] = mat->constantTable[i].nameHash;
	}
}

/*
XZoneInfo baseZones [] = { { "code_pre_gfx_mp", 0, 0 },
						   { "localized_code_pre_gfx_mp", 0, 0 },
						   { "code_post_gfx_mp", 0, 0 },
						   { "localized_code_post_gfx_mp", 0, 0 },
						   { "common_mp", 0, 0 },
						 };
*/

XZoneInfo baseZones[] = { 
							{ "defaults", 0, 0 },
							{ "shaders", 0, 0 } 
						};

void RunTool()
{
	doInit();

	Com_Printf("Loading Base Zones...\n");
	Com_LoadZones(baseZones, 2);
	Com_Printf("Done IW4 Initialization!\n");

	// allow us to load weapons from disk
	__asm call G_SetupWeaponDef

	if (console) { RunConsole(); return; }

	// this gives us a list of material constants
	//DB_EnumXAssets(ASSET_TYPE_MATERIAL, GetMaterialConstants, 0);

	if (build)
	{
		if (sources.size())
		{
			XZoneInfo* srcInfo = new XZoneInfo[sources.size()];
			int i = 0;
			for (auto it = sources.begin(); it != sources.end(); ++it)
			{
				srcInfo[i].name = strdup(it->c_str());
				srcInfo[i].type1 = 3;
				srcInfo[i].type2 = 0;
				i++;
				if (i == 9) { Com_Error(false, "Max of 9 source zones! Ignoring extra.\n"); break; }
			}
			Com_Printf("Loading source zones...\n");
			Com_LoadZones(srcInfo, i);
		}
		ZoneBuild((char*)zoneToBuild.c_str());
	}

	if (verify)
	{
		Com_Printf("Verifying fastfile integrity...\n");
		if(!build) useEntryNames = true; // only dump names when we arent building
		XZoneInfo verify;
		verify.name = zoneToBuild.c_str();
		verify.type1 = 3;
		verify.type2 = 0;
		Com_LoadZones(&verify, 1);

		Com_Printf("Done!");
	}

	if (IsDebuggerPresent())
	{
		getchar();
	}

	DestroyConsole();
}

DWORD LoadFFDBThread = 0x5BC800;

extern bool waitingOnLoad;
extern const char* zoneWaitingOn;
void CheckZoneLoad(char* name, int atype)
{
	__asm 
	{
		push atype
		push name
		call LoadFFDBThread
		add esp, 8
	}

	if(!strcmp(name, zoneWaitingOn))
	{
		waitingOnLoad = false;
	}
}

typedef int (__cdecl * DB_GetXAssetSizeHandler_t)();

void** DB_XAssetPool = (void**)0x7998A8;
unsigned int* g_poolSize = (unsigned int*)0x7995E8;

DB_GetXAssetSizeHandler_t* DB_GetXAssetSizeHandlers = (DB_GetXAssetSizeHandler_t*)0x799488;

void* ReallocateAssetPool(assetType_t type, unsigned int newSize)
{
	int elSize = DB_GetXAssetSizeHandlers[type]();
	char* poolEntry = (char*)malloc(newSize * elSize + 1);
	*poolEntry = type;
	DB_XAssetPool[type] = (poolEntry + 1);
	g_poolSize[type] = newSize;
	return poolEntry;
}

static DWORD gameWorldSP;
static DWORD gameWorldMP;

CallHook addEntryNameHook;
DWORD addEntryNameHookLoc = 0x5BB697;

void AddEntryNameHookFunc(int type, const char* name, void* asset)
{
	if (!useEntryNames)
	{
		return;
	}

	char blah[1024] = { 0 };
	_snprintf(blah, sizeof(blah), "%s,%s\n", getAssetStringForType(type), name);

	//OutputDebugString(blah);
	if (type == ASSET_TYPE_PIXELSHADER ||
		type == ASSET_TYPE_VERTEXSHADER ||
		type == ASSET_TYPE_VERTEXDECL ||
		type == ASSET_TYPE_TECHSET)
		Com_Printf_logOnly(blah);
	else
		Com_Printf(blah);
}

void __declspec(naked) AddEntryNameHookStub()
{
	__asm
	{
		push [esp + 36]
		push ecx
		push eax
		call AddEntryNameHookFunc
		pop eax
		pop ecx
		add esp, 4
		jmp addEntryNameHook.pOriginal
	}
}

int weaponNamesprintfHook(char* dest, size_t len, const char* format, ...)
{
	va_list va;
	va_start(va, format);

	char* type = va_arg(va, char*);
	char* name = va_arg(va, char*);

	return _snprintf(dest, len, "%s", name);
}

int __cdecl comErrorHook(int a1, char* format, ...)
{
	int result;
	va_list va;
	va_start(va, format);

	char dest[1024] = { 0 };
	result = _vsnprintf(dest, sizeof(dest), format + 1, va);

	Com_Error(false, "%s\n", dest);
	return 0;
}

const char* soundLoadingHook(const char* ptr)
{
	return strdup(ptr);
}

void InitBridge()
{
#if ZB_DEBUG
	Com_Printf("ZoneBuilder-%d.%d.%d built by %s on %s at %s (Debug)\n", MAJOR_VERSION, MINOR_VERSION, COMMIT, BUILDHOST, __DATE__, __TIME__);
#else
	Com_Printf("ZoneBuilder-%d.%d.%d built by %s on %s at %s\n", MAJOR_VERSION, MINOR_VERSION, COMMIT, BUILDHOST, __DATE__, __TIME__);
#endif

	parseArgs();
	InitConsole();

	Com_Printf("Initializing IW4...\n");

	// check version
	if (strcmp((char*)0x6E9638, "177"))
	{
		printf("Error loading IW4!\n");
		Com_Quit();
	}

	PatchMW2_Load(); // load fastfiles from dlc and alter
	//PatchMW2_StringList(); // for some reason the SL is messed up?
	PatchMW2_CryptoFiles(); // let us pull from iw4c fastfiles
	PatchMW2_CModels();
	//PatchMW2_FifthInfinity();
	PatchMW2_AssetRestrict();

	SetConsoleTitle("ZoneBuilder"); // branding

	// add our entry point
	call(0x6BABA1, RunTool, PATCH_CALL);
	call(0x5BCA85, CheckZoneLoad, PATCH_CALL);

	// redirect com_error so it doesn't longjmp
	call(0x4B22D0, comErrorHook, PATCH_JUMP);

	// redirect loading point of weapon files
	call(0x57AEC1, weaponNamesprintfHook, PATCH_CALL);
	call(0x57AF35, weaponNamesprintfHook, PATCH_CALL);
	call(0x57B38F, weaponNamesprintfHook, PATCH_CALL);

	// disable loading of sounds in weapon loading
	call(0x64A8A1, soundLoadingHook, PATCH_CALL);

	// fuck exceptions
	memset((DWORD*)0x6114B1, 0x90, 10);

	// always enable system console, not just if generating reflection probes
	memset((void*)0x60BB58, 0x90, 11);

	// disable 'ignoring asset' notices
	memset((void*)0x5BB902, 0x90, 5);

	// ignore 'no iwd files found in main'
	memset((void*)0x642A4B, 0x90, 5);

	// disable safe mode ish
	memset((void*)0x451434, 0x90, 5);

	// disable optimal options dialog
	memset((void*)0x450063, 0x90, 5);

	// exceptions
	SetUnhandledExceptionFilter(&CustomUnhandledExceptionFilter);

	// allow loading of IWffu (unsigned) files
	*(BYTE*)0x4158D9 = 0xEB; // main function
	*(WORD*)0x4A1D97 = 0x9090; // DB_AuthLoad_InflateInit

	// NTA patches
	*(DWORD*)0x1CDE7FC = GetCurrentThreadId(); // patch main thread ID
	*(BYTE*)0x519DDF = 0; //r_loadForrenderer = 0 
	*(BYTE*)0x4CF7F0 = 0xCC; // dirty disk breakpoint
	*(BYTE*)0x51F450 = 0xC3; //r_delayloadimage retn
	*(BYTE*)0x51F03D = 0xEB; // image release jmp

	// basic checks (hash jumps, both normal and playlist)
	*(WORD*)0x5B97A3 = 0x9090;
	*(WORD*)0x5BA493 = 0x9090;

	*(WORD*)0x5B991C = 0x9090;
	*(WORD*)0x5BA60C = 0x9090;

	*(WORD*)0x5B97B4 = 0x9090;
	*(WORD*)0x5BA4A4 = 0x9090;

	// some other, unknown, check
	*(BYTE*)0x5B9912 = 0xB8;
	*(DWORD*)0x5B9913 = 1;

	*(BYTE*)0x5BA602 = 0xB8;
	*(DWORD*)0x5BA603 = 1;

	*(BYTE*)0x54ADB0 = 0xC3;
	*(BYTE*)0x647781 = 0xEB;

	memset((void *)0x51F4FA, 0x90, 6);
	memset((void *)0x505AFB, 0x90, 7);
	memset((void *)0x505BDB, 0x90, 7);
	memset((void *)0x51E5CB, 0x90, 5);

	// fs_basegame
	*(DWORD*)0x6431D1 = (DWORD)fs_basegame;

	// r_registerDvars hack
	*(BYTE*)0x51B1CD = 0xC3;


	// weapon entries stuff here
	//doWeaponEntries();

	addEntryNameHook.initialize(addEntryNameHookLoc, AddEntryNameHookStub);
	addEntryNameHook.installHook();

	// reallocate asset pools
	ReallocateAssetPool(ASSET_TYPE_IMAGE, 7168);
	ReallocateAssetPool(ASSET_TYPE_LOADED_SOUND, 2700);
	ReallocateAssetPool(ASSET_TYPE_FX, 1200);
	ReallocateAssetPool(ASSET_TYPE_LOCALIZE, 14000);
	ReallocateAssetPool(ASSET_TYPE_XANIM, 8192);
	//ReallocateAssetPool(ASSET_TYPE_XMODEL, 3072);
	ReallocateAssetPool(ASSET_TYPE_XMODEL, 5125);
	ReallocateAssetPool(ASSET_TYPE_PHYSPRESET, 128);
	ReallocateAssetPool(ASSET_TYPE_PIXELSHADER, 10000);
	//ReallocateAssetPool(ASSET_TYPE_ADDON_MAP_ENTS, 128);
	ReallocateAssetPool(ASSET_TYPE_VERTEXSHADER, 3072);
	//ReallocateAssetPool(ASSET_TYPE_TECHSET, 1024);
	//ReallocateAssetPool(ASSET_TYPE_MATERIAL, 8192);
	ReallocateAssetPool(ASSET_TYPE_VERTEXDECL, 196);
	ReallocateAssetPool(ASSET_TYPE_GAME_MAP_SP, 1);
	ReallocateAssetPool(ASSET_TYPE_WEAPON, 2000);
	//ReallocateAssetPool(ASSET_TYPE_ADDON_MAP_ENTS, 128); // for codol fastfiles
	ReallocateAssetPool(ASSET_TYPE_RAWFILE, 2048);
	// causes heap issues
}