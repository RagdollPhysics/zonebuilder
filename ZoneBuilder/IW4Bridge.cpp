#include "StdInc.h"
#include "Hooking.h"
#include "Utils.h"
#include <list>
#include "Tool.h"

#pragma comment(linker,"/FIXED /BASE:0x8000000")

void PatchMW2_Console();
void PatchMW2_Fastfile();
void PatchMW2_Load();
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

void ZoneBuild(char* toBuild);

list<string> sources;
string zoneToBuild;

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

void RunTool()
{
	doInit();

	printf("Loading Source Zones...\n");
	// load source files
	XZoneInfo* info = new XZoneInfo[sources.size()];
	int i=0;
	for(list<string>::iterator it = sources.begin(); it != sources.end(); ++it)
	{
		info[i].name = strdup((*it).c_str());
		info[i].type1 = 3;
		info[i].type2 = 0;
		i++;
	}
	DB_LoadXAssets(info, sources.size(), 0);
	while(!loadedFastfiles) Sleep(100);
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
}

void printUsage()
{
	printf("usage: ZoneBuilder.exe [zone name] -sSourceZones\n");
	TerminateProcess(GetCurrentProcess(), 0);
}

void parseArgs()
{
	sources.push_back(string("code_post_gfx_mp"));
	sources.push_back(string("localized_code_post_gfx_mp"));
	sources.push_back(string("common_mp"));
	char** argv = getArgs();
	int argc = getArgc();
	if(argc == 1) printUsage();
	for(int i=1; i<argc; i++)
	{
		if(!strncmp("-s", argv[i], 2))
		{
			sources.push_back(string(argv[i] + 2));
		}
		else
		{
			if(strlen(zoneToBuild.c_str())) { printf("Can't build more than one zone!\n"); TerminateProcess(GetCurrentProcess(), 0); }
			zoneToBuild = string(argv[i]);
		}
	}
	if(!strlen(zoneToBuild.c_str())) { printf("No zone specefied to build!\n"); TerminateProcess(GetCurrentProcess(), 0); }
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
	PatchMW2_Fastfile(); // let us load 277 fastfiles
	//PatchMW2_Load(); // load fastfiles from dlc and alter

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
	*(DWORD*)0x6431D1 = (DWORD)"zonebuilder";

	// r_registerDvars hack
	*(BYTE*)0x51B1CD = 0xC3;

	// weapon entries stuff here
	//doWeaponEntries();
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
		entries.push_back(e);
	}
	entries.sort(compareEntries);
	for(std::list<weaponEntry_t*>::iterator it=entries.begin(); it!=entries.end(); ++it)
	{
		weaponEntry_t * e = *it;
		printf("%s: type %d, offset 0x%x\n", e->name, e->type, e->offset);
	}
	getchar();
}