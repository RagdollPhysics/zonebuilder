// ==========================================================
// IW4M project
// 
// Component: clientdll
// Sub-component: steam_api
// Purpose: Modern Warfare 2 patches: console-based console
//
// Initial author: NTAuthority
// Started: 2011-01-20
// ==========================================================

#include "StdInc.h"
#undef MOUSE_MOVED

#include "Hooking.h"

void DoWinMainInit() {}

// patch locations
#define HOOK_Sys_CreateConsole 0x4305E0
#define HOOK_Sys_DestroyConsole 0x4528A0
#define HOOK_Sys_Print 0x4B2080
#define HOOK_Sys_Error 0x43D570
#define HOOK_Sys_ConsoleInput 0x4859A5

// patch handlers
StompHook createConsoleHook;
StompHook destroyConsoleHook;
StompHook sysPrintHook;
StompHook sysErrorHook;
CallHook consoleInputHook;


void Sys_CreateConsole() {}

void Sys_DestroyConsole() {}

static bool didWeHaveConsole = false;
static int lastRefresh = 0;

void Sys_Print(const char* message)
{
#ifdef ZB_DEBUG
	printf(message);
#endif
}

void Sys_Error(const char* format, ...)
{
	static char buffer[32768];
	va_list va;
	va_start(va, format);
	//vprintf(format, va);
	_vsnprintf(buffer, sizeof(buffer), format, va);
	va_end(va);

	printf("ERROR: %s\n", buffer);
}

const char* Sys_ConsoleInput()
{
	return NULL;
}

// patch entry point
void PatchMW2_Console()
{
	createConsoleHook.initialize(HOOK_Sys_CreateConsole, Sys_CreateConsole);
	createConsoleHook.installHook();

	destroyConsoleHook.initialize(HOOK_Sys_DestroyConsole, Sys_DestroyConsole);
	destroyConsoleHook.installHook();

	// [s]TODO: get rid of weird casts[/s] - mate, thats all sorted.
	sysPrintHook.initialize(HOOK_Sys_Print, Sys_Print);
	sysPrintHook.installHook();

	sysErrorHook.initialize(HOOK_Sys_Error, Sys_Error);
	sysErrorHook.installHook();

	consoleInputHook.initialize(HOOK_Sys_ConsoleInput, Sys_ConsoleInput);
	consoleInputHook.installHook();

	//PatchMW2_DebugAllocations();
}