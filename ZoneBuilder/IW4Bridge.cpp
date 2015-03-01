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

void PatchMW2_Console();
void PatchMW2_Load();
void PatchMW2_StringList();
void PatchMW2_CryptoFiles();
void PatchMW2_FifthInfinity();
void PatchMW2_FilePointers();
void doWeaponEntries();

DWORD init1 = 0x42F0A0;
DWORD init2 = 0x4301B0;
DWORD init3 = 0x406D10;
DWORD init4 = 0x4D2280;
DWORD init5 = 0x47F390;
DWORD init6 = 0x420830;
DWORD init7 = 0x64A020;
DWORD init8 = 0x4E0FB0;
DWORD init9 = 0x60AD10;
DWORD init10 = 0x5196C0;
DWORD init11 = 0x4A62A0;
DWORD init12 = 0x429080;

bool loadedFastfiles = false;
bool dumping = false;
bool verify = false;
bool useEntryNames = false;

void ZoneBuild(char* toBuild);

list<string> sources;
string zoneToBuild;
string toDump;
int dumpType;
void dumpModel(char * name);

void doInit()
{
	__asm
	{
		call init1
		call init2
		push 0
		call init3
		add esp, 4
		call init4
		call init5
		call init6
		call init7
		call init8
		call init9
		call init10
		call init11
		call init12
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

	__asm
	{
		mov esp, origStack
	}

	delete[] tempStack;

	return 0;
}

void RunTool()
{
	doInit();

	//if (verify) sources.clear();

	printf("Loading Source Zones...\n");
	// load source files
	XZoneInfo* info = new XZoneInfo[9];
	int i=0;
	if(sources.size() == 0) loadedFastfiles = true;
	for(list<string>::iterator it = sources.begin(); it != sources.end(); ++it)
	{
		info[i].name = strdup((*it).c_str());
		info[i].type1 = 3;
		info[i].type2 = 0;
		if(i >= 8)
		{
			DB_LoadXAssets(info, i, 0);
			i = 0;
		}
		else
			i++;
	}
	if(i > 0)
		DB_LoadXAssets(info, i, 0);
	while(!loadedFastfiles) Sleep(100);

	if(dumping)
	{
		printf("dumping stuff now");
		if(dumpType == ASSET_TYPE_XMODEL)
			dumpModel((char*)toDump.c_str());
		getchar();
		return;
	}

	if(verify)
	{
		useEntryNames = true;
		XZoneInfo info;
		info.name = zoneToBuild.c_str();
		info.type1 = 3;
		info.type2 = 0;
		loadedFastfiles = false;
		DB_LoadXAssets(&info, 1, 0);
		while(!loadedFastfiles) Sleep(100);
		return;
	}

	ZoneBuild((char*)zoneToBuild.c_str());
}

DWORD LoadFFDBThread = 0x5BC800;

void CheckZoneLoad(char* name, int atype)
{
	__asm {
		push atype
		push name
		call LoadFFDBThread
		add esp, 8
	}
	if(!strcmp(name, sources.back().c_str()))
	{
		printf("Done IW4 Initialization!\n");
		loadedFastfiles = true;
	}
	if(!strcmp(name, zoneToBuild.c_str()))
	{
		loadedFastfiles = true;
	}
}

void printUsage()
{
	printf("usage: ZoneBuilder.exe [zone name] -sSourceZones\n");
	printf("\t\t\t\t\t   -v [zone name]\n");
	TerminateProcess(GetCurrentProcess(), 0);
}

char* fs_basegame = "zonebuilder";

void parseArgs()
{
	sources.push_back(string("code_pre_gfx_mp"));
	sources.push_back(string("localized_code_pre_gfx_mp"));
	sources.push_back(string("code_post_gfx_mp"));
	sources.push_back(string("localized_code_post_gfx_mp"));
	sources.push_back(string("common_mp"));
	char** argv = getArgs();
	int argc = getArgc();
	if(argc == 1) printUsage();

	if(!strncmp("-v", argv[1], 2))
	{
		verify = true;
		//sources.clear();
		zoneToBuild = string(argv[2]);
		return;
	}
	for(int i=1; i<argc; i++)
	{
		if(!strncmp("-s", argv[i], 2))
		{
			sources.push_back(string(argv[i] + 2));
		}
		else if(!strncmp("-d", argv[i], 2))
		{
			char line[3];
			strncpy(line, argv[i] + 2, 2);
			line[2] = 0;
			dumpType = atoi(line);
			toDump = string(argv[i] + 4);
			dumping = true;
		}
		else if (!strncmp("-b", argv[i], 2))
		{
			i++;
			fs_basegame = strdup(argv[i]);
		}
		else
		{
			if(strlen(zoneToBuild.c_str())) { printf("Can't build more than one zone!\n"); TerminateProcess(GetCurrentProcess(), 0); }
			zoneToBuild = string(argv[i]);
		}
	}
	if(!strlen(zoneToBuild.c_str()) && !dumping) { printf("No zone specefied to build!\n"); TerminateProcess(GetCurrentProcess(), 0); }
}

typedef int (__cdecl * DB_GetXAssetSizeHandler_t)();

void** DB_XAssetPool = (void**)0x7998A8;
unsigned int* g_poolSize = (unsigned int*)0x7995E8;

DB_GetXAssetSizeHandler_t* DB_GetXAssetSizeHandlers = (DB_GetXAssetSizeHandler_t*)0x799488;

void* ReallocateAssetPool(assetType_t type, unsigned int newSize)
{
	int elSize = DB_GetXAssetSizeHandlers[type]();
	void* poolEntry = malloc(newSize * elSize);
	DB_XAssetPool[type] = poolEntry;
	g_poolSize[type] = newSize;
	return poolEntry;
}

static DWORD gameWorldSP;
static DWORD gameWorldMP;

CallHook addEntryNameHook;
DWORD addEntryNameHookLoc = 0x5BB697;

void AddEntryNameHookFunc(int type, const char* name)
{
	if (!useEntryNames)
	{
		return;
	}

	if(type == ASSET_TYPE_PIXELSHADER ||
		type == ASSET_TYPE_VERTEXSHADER ||
		type == ASSET_TYPE_VERTEXDECL ||
		type == ASSET_TYPE_TECHSET)
		return;

	char blah[1024];
	_snprintf(blah, 1024, "%s,%s\n", getAssetStringForType(type), name);
	OutputDebugString(blah);
	printf(blah);
}

void __declspec(naked) AddEntryNameHookStub()
{
	__asm
	{
		push ecx
		push eax
		call AddEntryNameHookFunc
		pop eax
		pop ecx
		jmp addEntryNameHook.pOriginal
	}
}

void InitBridge()
{
	printf("Initializing IW4...\n");
	parseArgs();

	// check version
	if (strcmp((char*)0x6E9638, "177"))
	{
		printf("Error loading IW4!\n");
		TerminateProcess(GetCurrentProcess(), 0);
	}

	PatchMW2_Console(); // redirect output
	PatchMW2_Load(); // load fastfiles from dlc and alter
	//PatchMW2_StringList(); // for some reason the SL is messed up?
	// no but why not
	PatchMW2_CryptoFiles(); // let us pull from iw4c fastfiles
	PatchMW2_FifthInfinity();

	SetConsoleTitle("ZoneBuilder"); // branding

	// add our entry point
	call(0x6BABA1, RunTool, PATCH_CALL);
	call(0x5BCA85, CheckZoneLoad, PATCH_CALL);

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
	ReallocateAssetPool(ASSET_TYPE_ADDON_MAP_ENTS, 128); // for codol fastfiles
}


typedef struct weaponEntry_s
{
	const char* name;
	int offset;
	int type;
} weaponEntry_t;

#define NUM_ENTRIES 672

#define WEAPON_DO_ARRAY(ar, c) \
{ \
	for (int _l_1 = 0; _l_1 < c; _l_1++) \
	{ \
		if (*(int*)data == _l_1) \
		{ \
			fprintf(file, "%s", ((char**)ar)[_l_1]); /* why do I have to explicitly define ar as being a char**? */ \
		} \
	} \
}

weaponEntry_t* weaponEntries = (weaponEntry_t*)0x795F00;

typedef struct 
{
	int offsetStart;
	int pointerOrigin;
} rtOffsetMap_t;

#define NUM_OFFSET_MAPS 14

static rtOffsetMap_t offsetMap[] =
{
	{ 116, 4 },
	{ 1784, 12 },
	{ 1848, 16 },
	{ 1996, 120 },
	{ 2060, 128 },
	{ 2208, 132 },
	{ 2356, 140 },
	{ 2388, 144 },
	{ 2420, 148 },
	{ 2452, 152 },
	{ 2484, 588 },
	{ 2548, 1208 },
	{ 2672, 1212 },
	{ 2796, 1576 },
	{ 0, 0 }
};

char* MapOffsetToPointer(char* origin, int offset)
{
	for (int i = (NUM_OFFSET_MAPS - 1); i >= 0; i--)
	{
		rtOffsetMap_t* current = &offsetMap[i];
		rtOffsetMap_t* next = &offsetMap[i + 1];

		int max = next->offsetStart;
		if (max == 0) max = 0xFFFFFF;

		if (offset >= current->offsetStart && offset < max)
		{
			char* pointer = *(char**)MapOffsetToPointer(origin, current->pointerOrigin);
			return (pointer + (offset - current->offsetStart));
		}
	}

	return (origin + offset);
}

bool compareEntries(weaponEntry_t* first, weaponEntry_t* second)
{
	return first->offset < second->offset;
}

void doWeaponEntries()
{
	std::list<weaponEntry_t*> entries;

	for(int i=0; i<NUM_ENTRIES; i++)
	{
		weaponEntry_t * e = &weaponEntries[i];
		char* filename = "entries.txt";

		if(e->type >= 16 && e->type <=37)
			filename = "special.txt";
		if(e->type == 10)
			filename = "effects.txt";
		if(e->type == 11)
			filename = "models.txt";
		if(e->type == 12)
			filename = "materials.txt";
		if(e->type == 14)
			filename = "sounds.txt";

		FILE* out = fopen(filename, "a");
		fprintf(out, "\"%s\",\n", e->name);
		fclose(out);
	}
	getchar();
}