// ==========================================================
// IW4M project
// 
// Component: clientdll
// Sub-component: steam_api
// Purpose: Various generic utility functions.
//
// Initial author: NTAuthority
// Started: 2010-09-10
// ==========================================================

#include "StdInc.h"
#include <ShellAPI.h>
#include <sys/stat.h>
#include <direct.h>
#include <io.h>

/*
void Trace(char* source, char* message, ...)
{
	va_list args;
	char buffer[1024];
	char buffer2[1024];

	va_start(args, message);
	_vsnprintf(buffer, sizeof(buffer), message, args);
	va_end(args);

	_snprintf(buffer2, sizeof(buffer2), "[%s] %s", source, buffer);

	OutputDebugStringA(buffer2);
}

void Trace2(char* message, ...)
{
	va_list args;
	char buffer[1024];

	va_start(args, message);
	_vsnprintf(buffer, sizeof(buffer), message, args);
	va_end(args);

	OutputDebugStringA(buffer);
}
*/

bool FileExists(const char* file)
{
	struct stat st;

	// note that this doesn't count any of the other ways stat could fail, but that'd be more error checking elsewhere
	if (stat(file, &st) >= 0)
	{
		return true;
	}

	return false;
}

size_t FileSize(const char* file)
{
	struct stat st;

	if (stat(file, &st) >= 0)
	{
		return st.st_size;
	}

	return 0;
}

char * FileDir(const char * path)
{
	char * ret  = (char*)malloc(1024);
	strncpy(ret, path, 1024);
	for(int i=strlen(ret); i>=0; i--)
	{
		if(ret[i] == '/' || ret[i] == '\\')
		{
			ret[i] = 0;
			break;
		}
	}
	return ret;
}

// a funny thing is how this va() function could possibly come from leaked IW code.
#define VA_BUFFER_COUNT		4
#define VA_BUFFER_SIZE		4096

static char g_vaBuffer[VA_BUFFER_COUNT][VA_BUFFER_SIZE];
static int g_vaNextBufferIndex = 0;

const char *va( const char *fmt, ... )
{
	va_list ap;
	char *dest = &g_vaBuffer[g_vaNextBufferIndex][0];
	g_vaNextBufferIndex = (g_vaNextBufferIndex + 1) % VA_BUFFER_COUNT;
	va_start(ap, fmt);
	vsprintf( dest, fmt, ap );
	va_end(ap);
	return dest;
}

const char *getOutFile(const char *name)
{
	return va("%s/%s", CURRENT_ZONE_NAME, name);
}

void CreateDirectoryAnyDepth(const char *path)
{
	char opath[MAX_PATH]; 
	char *p; 
	size_t len; 
	strncpy_s(opath, path, sizeof(opath)); 
	len = strlen(opath); 
	if(opath[len - 1] == L'/') 
		opath[len - 1] = L'\0'; 

	for(p = opath; *p; p++) 
	{
		if(*p == L'/' || *p == L'\\') 
		{ 
			*p = L'\0'; 
			if(_access(opath, 0)) 
				_mkdir(opath);
			*p = L'\\';
		}
	}
	if(_access(opath, 0))
		_mkdir(opath);
}