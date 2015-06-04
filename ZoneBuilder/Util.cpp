#include "StdInc.h"
#include <shellapi.h>

void Com_Printf(const char* format, ...)
{
	static char buffer[32768] = { 0 };

	va_list va;
	va_start(va, format);

	//vprintf(format, va);
	_vsnprintf(buffer, sizeof(buffer), format, va);
	va_end(va);

	printf("%s", buffer);
}

void Com_Debug_(const char* format, ...)
{
	static char buffer[32768] = { 0 };

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
	static char buffer[32768] = { 0 };

	va_list va;
	va_start(va, format);

	//vprintf(format, va);
	_vsnprintf(buffer, sizeof(buffer), format, va);
	va_end(va);

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
	printf("ERROR: %s\n", buffer);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);

	if (exit)
	{
		if (IsDebuggerPresent())
		{
			DebugBreak();
		}

		TerminateProcess(GetCurrentProcess(), -1);
	}
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

int iArgCount = 0;

int getArgc()
{
	return iArgCount;
}

LPSTR* getArgs()
{
	int iIdx;

	LPWSTR * pszArgVectorWide = CommandLineToArgvW(GetCommandLineW(), &iArgCount);

	// Convert the wide string array into an ANSI array (input is ASCII-7)
	LPSTR * pszArgVectorAnsi = new LPSTR [iArgCount];

	for (iIdx = 0; iIdx < iArgCount; ++iIdx)
	{
		 size_t qStrLen = wcslen(pszArgVectorWide[iIdx]), qConverted = 0;
		 pszArgVectorAnsi[iIdx] = new CHAR [qStrLen+1];
		 wcstombs_s(&qConverted,pszArgVectorAnsi[iIdx],qStrLen+1,
		 pszArgVectorWide [iIdx],qStrLen+1);
	}

	return pszArgVectorAnsi;
}

long flength(FILE* fp)
{
	long i = ftell(fp);
	fseek(fp, 0, SEEK_END);
	long ret = ftell(fp);
	fseek(fp, i, SEEK_SET);
	return ret;
}

extern unsigned int R_HashString(const char* string);
void debugChecks()
{
	ASSERT(sizeof(PhysGeomList) == 0x48);
	ASSERT(sizeof(PhysGeomInfo) == 0x44);
	ASSERT(sizeof(BrushWrapper) == 0x44);
	ASSERT(sizeof(cPlane) == 0x14);
	ASSERT(sizeof(cBrushSide) == 8);
	ASSERT(sizeof(XAnim) == DB_GetXAssetTypeSize(ASSET_TYPE_XANIM));
	ASSERT(sizeof(XModelSurfaces) == DB_GetXAssetTypeSize(ASSET_TYPE_XMODELSURFS));
	ASSERT(sizeof(XModel) == DB_GetXAssetTypeSize(ASSET_TYPE_XMODEL));
	ASSERT(sizeof(Material) == 0x60);
	ASSERT(sizeof(GfxImage) == 0x20);
	ASSERT(sizeof(SoundAliasList) == DB_GetXAssetTypeSize(ASSET_TYPE_SOUND));
	ASSERT(sizeof(SoundAlias) == 100);
	ASSERT(sizeof(SpeakerMap) == 408);
	ASSERT(sizeof(SoundFile) == 12);
	ASSERT(sizeof(WeaponVariantDef) == 0x74);
	ASSERT(sizeof(WeaponDef) == 0x684)
}