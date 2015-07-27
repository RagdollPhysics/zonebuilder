#include "StdInc.h"
#include "Tool.h"

char* fs_basegame = "zonebuilder";

string zoneToBuild;
bool build = false, verify = false;
bool console = false;
list<string> sources;

void printUsage()
{
	Com_Printf("usage: ZoneBuilder.exe zone name [-b] [-v] [-sSourceZone] [-dBuildFolder]\n");
	Com_Printf("Or run with no arguments for console.\n");
	Com_Quit();
}

void parseArgs()
{
	char** argv = getArgs();
	int argc = getArgc();
	if (argc == 1) { console = true; return; }

	zoneToBuild = string(argv[1]);

	for (int i = 2; i < argc;  i++)
	{
		if (!strcmp("-b", argv[i])) build = true;
		if (!strcmp("-v", argv[i])) verify = true;
		if (!strncmp("-s", argv[i], 2))
		{
			sources.push_back(string(argv[i] + 2));
		}
		if (!strncmp("-d", argv[i], 2))
		{
			fs_basegame = strdup(argv[i] + 2);
		}
	}
	if (!console && !build && !verify) printUsage();
}