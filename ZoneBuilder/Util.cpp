#include "StdInc.h"

void Com_Printf(const char* format, ...)
{
	static char buffer[32768];
	va_list va;
	va_start(va, format);
	//vprintf(format, va);
	_vsnprintf(buffer, sizeof(buffer), format, va);
	va_end(va);

	printf("%s", buffer);
}

void Com_Debug_(const char* format, ...)
{
	static char buffer[32768];
	va_list va;
	va_start(va, format);
	//vprintf(format, va);
	_vsnprintf(buffer, sizeof(buffer), format, va);
	va_end(va);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
	printf("%s", buffer);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}

void Com_Error(bool exit, const char* format, ...)
{
	static char buffer[32768];
	va_list va;
	va_start(va, format);
	//vprintf(format, va);
	_vsnprintf(buffer, sizeof(buffer), format, va);
	va_end(va);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
	printf("ERROR: %s\n", buffer);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	if(IsDebuggerPresent())
		getchar();
	if(exit)
		TerminateProcess(GetCurrentProcess(), -1);
}

const char* assetTypeStrings [] = {
	"physpreset",
	"phys_collmap",
	"xanim",
	"xmodelsurfs",
	"xmodel",
	"material",
	"pixelshader",
	"vertexshader",
	"vertexdecl",
	"techset",
	"image",
	"sound",
	"sndcurve",
	"loaded_sound",
	"col_map_sp",
	"col_map_mp",
	"com_map",
	"game_map_sp",
	"game_map_mp",
	"map_ents",
	"fx_map",
	"gfx_map",
	"lightdef",
	"ui_map",
	"font",
	"menufile",
	"menu",
	"localize",
	"weapon",
	"snddriverglobals",
	"fx",
	"impactfx",
	"aitype",
	"mptype",
	"character",
	"xmodelalias",
	"rawfile",
	"stringtable",
	"leaderboarddef",
	"structureddatadef",
	"tracer",
	"vehicle",
	"addon_map_ents",
};

int getAssetTypeForString(const char* str)
{
	int i = 42;
	while(i > 0)
	{
		if(!strcmp(assetTypeStrings[i], str)) return i;
		i--;
	}
	return -1;
}

const char* getAssetStringForType(int type)
{
	return assetTypeStrings[type];
}