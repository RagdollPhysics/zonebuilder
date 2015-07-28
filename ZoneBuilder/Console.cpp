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
#include "Tool.h"
#include "s10e5.h"

#include <curses.h>
#include <cctype>
#include <clocale>

void DoWinMainInit() {}

// patch locations
#define HOOK_Sys_Print 0x4B2080
#define HOOK_Sys_Error 0x43D570
#define HOOK_Sys_ConsoleInput 0x4859A5

// should use funcionality?
extern bool console, useEntryNames;

// patch handlers
StompHook sysPrintHook;
StompHook sysErrorHook;
CallHook consoleInputHook;

// patch functions
#define WIDTH 80
#define HEIGHT 25
#define OUTPUT_HEIGHT 500
#define OUTPUT_MAX_TOP (OUTPUT_HEIGHT - (HEIGHT - 2))
static WINDOW* outputWindow;
static WINDOW* inputWindow;
static WINDOW* infoWindow;

static int currentOutputTop = 0;
static int currentOutBuffer = 0; // for initial counting of output buffer

void RefreshOutput()
{
	prefresh(outputWindow, (currentOutputTop >= 1) ? currentOutputTop - 1 : 0, 0, 1, 0, HEIGHT - 2, WIDTH);
}

void ScrollOutput(int amount)
{
	currentOutputTop += amount;

	if (currentOutputTop > OUTPUT_MAX_TOP)
	{
		currentOutputTop = OUTPUT_MAX_TOP;
	}
	else if (currentOutputTop < 0)
	{
		currentOutputTop = 0;
	}

	// make it only scroll the top if there's more than HEIGHT lines
	if (currentOutBuffer >= 0)
	{
		currentOutBuffer += amount;

		if (currentOutBuffer >= (HEIGHT))
		{
			currentOutBuffer = -1;
		}

		if (currentOutputTop < HEIGHT)
		{
			currentOutputTop = 0;
		}
	}
}

void Sys_CreateConsole()
{
	initscr();
	raw();
	noecho();

	//outputWindow = newwin(HEIGHT - 1, WIDTH, 1, 0);
	outputWindow = newpad(OUTPUT_HEIGHT, WIDTH);
	inputWindow = newwin(1, WIDTH, HEIGHT - 1, 0);
	infoWindow = newwin(1, WIDTH, 0, 0);

	scrollok(outputWindow, true);
	scrollok(inputWindow, true);
	nodelay(inputWindow, true);
	keypad(inputWindow, true);

	if (has_colors())
	{
		start_color();
		init_pair(1, COLOR_BLACK, COLOR_WHITE);
		init_pair(2, COLOR_WHITE, COLOR_BLACK);
		init_pair(3, COLOR_RED, COLOR_BLACK);
		init_pair(4, COLOR_GREEN, COLOR_BLACK);
		init_pair(5, COLOR_YELLOW, COLOR_BLACK);
		init_pair(6, COLOR_BLUE, COLOR_BLACK);
		init_pair(7, COLOR_CYAN, COLOR_BLACK);
		init_pair(8, COLOR_RED, COLOR_BLACK);
		init_pair(9, COLOR_WHITE, COLOR_BLACK);
		init_pair(10, COLOR_WHITE, COLOR_BLACK);
		//init_pair(2, COLOR_WHITE, COLOR_BLACK);
	}

	wbkgd(infoWindow, COLOR_PAIR(1));

	wclear(infoWindow);
#if ZB_DEBUG
	wprintw(infoWindow, "ZoneBuilder-%d.%d.%d built by %s on %s at %s (Debug)", MAJOR_VERSION, MINOR_VERSION, COMMIT, BUILDHOST, __DATE__, __TIME__);
#else
	wprintw(infoWindow, "ZoneBuilder-%d.%d.%d built by %s on %s at %s", MAJOR_VERSION, MINOR_VERSION, COMMIT, BUILDHOST, __DATE__, __TIME__);
#endif
	wnoutrefresh(infoWindow);

	wrefresh(infoWindow);
	//wrefresh(outputWindow);
	wrefresh(inputWindow);
	RefreshOutput();
}

void Sys_DestroyConsole()
{
	delwin(outputWindow);
	delwin(inputWindow);
	delwin(infoWindow);
	endwin();
}

static bool didWeHaveConsole = false;

void Sys_Print(const char* message)
{
	if (!console) { Com_Printf("%s", message); return; }

	const char* p = message;
	while (*p != '\0')
	{
		if (*p == '\n')
		{
			ScrollOutput(1);
		}

		if (*p == '^')
		{
			char color;
			p++;

			color = (*p - '0');

			if (color < 9 && color > 0)
			{
				wattron(outputWindow, COLOR_PAIR(color + 2));
				p++;
				continue;
			}
		}

		waddch(outputWindow, *p);

		p++;
	}

	//wattron(outputWindow, COLOR_PAIR(9));
	RefreshOutput();
}

extern void Com_Quit();
void Sys_Error(const char* format, ...)
{
	static char buffer[32768];
	va_list va;
	va_start(va, format);
	//vprintf(format, va);
	_vsnprintf(buffer, sizeof(buffer), format, va);
	va_end(va);

	if (!console) { Com_Error(true, "%s", buffer); return; }

	Sys_Print("^1ERROR: ");
	Sys_Print(buffer);
	Sys_Print("^7\n");
	RefreshOutput();

	if (IsDebuggerPresent())
	{
		DebugBreak();
	}

	Com_Quit();
}

static char consoleLineBuffer[1024];
static int consoleLineBufferIndex = 0;

static char consoleLineBuffer2[1024];

void ShowConsolePrompt()
{
	wattron(inputWindow, COLOR_PAIR(10) | A_BOLD);
	wprintw(inputWindow, "> ");
}

const char* Sys_ConsoleInput()
{
	if (!console) return NULL;
	if (!didWeHaveConsole)
	{
		ShowConsolePrompt();
		wrefresh(inputWindow);
		didWeHaveConsole = true;
	}
	int c = wgetch(inputWindow);

	if (c == ERR)
	{
		return NULL;
	}

	switch (c)
	{
	case '\r':
	case 459: // keypad enter
		wattron(outputWindow, COLOR_PAIR(10) | A_BOLD);
		wprintw(outputWindow, "%s", "]");
		if (consoleLineBufferIndex) wprintw(outputWindow, "%s", consoleLineBuffer);
		wprintw(outputWindow, "%s", "\n");
		wattroff(outputWindow, A_BOLD);
		wclear(inputWindow);
		ShowConsolePrompt();
		wrefresh(inputWindow);

		ScrollOutput(1);
		RefreshOutput();

		if (consoleLineBufferIndex)
		{
			strcpy(consoleLineBuffer2, consoleLineBuffer);
			strcat(consoleLineBuffer, "\n");
			consoleLineBufferIndex = 0;
			return consoleLineBuffer;
		}

		break;
	case 'c' - 'a' + 1: // ctrl-c
	case 27:
		consoleLineBuffer[0] = '\0';
		consoleLineBufferIndex = 0;
		wclear(inputWindow);
		ShowConsolePrompt();
		wrefresh(inputWindow);
		break;
	case 8: // backspace
		if (consoleLineBufferIndex > 0)
		{
			consoleLineBufferIndex--;
			consoleLineBuffer[consoleLineBufferIndex] = '\0';

			wprintw(inputWindow, "%c %c", (char)c, (char)c);
			wrefresh(inputWindow);
		}
		break;
	case KEY_PPAGE:
		ScrollOutput(-1);
		RefreshOutput();
		break;
	case KEY_NPAGE:
		ScrollOutput(1);
		RefreshOutput();
		break;
	case KEY_UP:
		wclear(inputWindow);
		ShowConsolePrompt();
		wprintw(inputWindow, "%s", consoleLineBuffer2);
		wrefresh(inputWindow);

		strcpy(consoleLineBuffer, consoleLineBuffer2);
		consoleLineBufferIndex = strlen(consoleLineBuffer);
		break;
	default:
		if (c <= 127 && consoleLineBufferIndex < 1022)
		{
			// temporary workaround for issue #9, find out what overwrites our index later on
			//consoleLineBufferIndex = strlen(consoleLineBuffer);

			consoleLineBuffer[consoleLineBufferIndex++] = (char)c;
			consoleLineBuffer[consoleLineBufferIndex] = '\0';
			wprintw(inputWindow, "%c", (char)c);
			wrefresh(inputWindow);
		}
		break;
	}

	return NULL;
}

void InitConsole()
{
	sysPrintHook.initialize(HOOK_Sys_Print, Sys_Print);
	sysPrintHook.installHook();

	sysErrorHook.initialize(HOOK_Sys_Error, Sys_Error);
	sysErrorHook.installHook();

	if (!console) return;

	consoleInputHook.initialize(HOOK_Sys_ConsoleInput, Sys_ConsoleInput);
	consoleInputHook.installHook();

	Sys_CreateConsole();
}

void DestroyConsole()
{
	if (!console) return;
	Sys_DestroyConsole();
}

pair<string, string> parseCmd(const char* cmd)
{
	char* buf = strdup(cmd);
	char* first = strtok(buf, " \n");
	char* second = strtok(NULL, " \n");
	if (!second) second = "";

	if (first == NULL || second == NULL) return pair<string, string>("", "");

	for (char* p = first; *p; ++p) *p = tolower(*p);
	for (char* p = second; *p; ++p) *p = tolower(*p);
	return pair<string, string>(first, second);
}

void PrintNameOfAsset(void* data, int userdata)
{
	FILE* output = (FILE*)userdata;
	fprintf(output, "%s\n", *((char**)data));
}

void decryptFastfile(const char* param);
void dumpStuff(const char* param);
void buildDefaults();

void printHelp()
{
	Com_Printf("Commands: \n");
	Com_Printf("\tloadzone <zone> - load a zone as source\n");
	Com_Printf("\tbuildzone <zone> - build a zone\n");
	Com_Printf("\tverify <zone> - verify a zone\n");
	Com_Printf("\tunloadzones - unload all loaded zones\n");
	Com_Printf("\tlistassets <type> - list all loaded assets of a type\n");
	Com_Printf("\tdecryptzone <zone> - decrypts an iw4c zone\n");
	Com_Printf("\thelp - prints this message\n");
	Com_Printf("\tquit - quits the program\n");
}

void RunConsole()
{
	ScrollOutput(12);
	printHelp();	
	while (console)
	{
		RefreshOutput();
		Sleep(5);

		const char* input = Sys_ConsoleInput();
		if (!input) continue;
		pair<string, string> cmd = parseCmd(input);
		if (cmd.first == "loadzone")
		{
			if (cmd.second == "") { Com_Printf("loadzone requires 1 argument, zone"); }
			else
			{
				XZoneInfo info;
				info.name = cmd.second.c_str();
				info.type1 = 2;
				info.type2 = 0;
				Com_LoadZones(&info, 1);
				Com_Printf("Loaded!\n");
			}
		}

		else if(cmd.first == "verify")
		{
			if (cmd.second.length() == 0) { Com_Printf("verify requires 1 argument, zone\n"); }
			else
			{
				useEntryNames = true;
				XZoneInfo info;
				info.name = cmd.second.c_str();
				info.type1 = 4;
				info.type2 = 0;
				Com_LoadZones(&info, 1);
				useEntryNames = false;
				Com_UnloadZones(4);
				Com_Printf("Verified!\n");
			}
		}

		else if(cmd.first == "unloadzones")
		{
			Com_UnloadZones(2);
		}

		else if (cmd.first == "buildzone")
		{
			ZoneBuild((char*)cmd.second.c_str());
		}
		else if (cmd.first == "listassets")
		{
			if (cmd.second.length() == 0) { Com_Printf("listAssets requires 1 argument, type\n"); }
			else
			{
				Com_Printf("Listing all loaded %s assets to 'assets.txt'\n", cmd.second.c_str());
				assetType_t type = (assetType_t)getAssetTypeForString(cmd.second.c_str());
				if (type >= 0)
				{
					FILE* output = fopen("assets.txt", "w");
					DB_EnumXAssets(type, PrintNameOfAsset, (int)output);
					fclose(output);
				}
			}
		}
		else if (cmd.first == "dump")
		{
			dumpStuff(cmd.second.c_str());
		}
		else if (cmd.first == "decryptzone")
		{
			decryptFastfile(cmd.second.c_str());
		}
		else if (cmd.first == "quit")
		{
			Com_Quit();
		}
		else if (cmd.first == "help")
		{
			printHelp();
		}
#if ZB_DEBUG
		else if (cmd.first == "defaults")
		{
			buildDefaults();
		}
#endif
		else
		{
			Com_Error(false, "Error: Command '%s' unrecognized", cmd.first.c_str());
		}
	}
}