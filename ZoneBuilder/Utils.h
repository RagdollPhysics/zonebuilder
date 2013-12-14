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

#define Trace(source, message, ...) Trace2("[" source "] " message, __VA_ARGS__)
#define Trace2(message, ...) Com_Printf(0, message "\n", __VA_ARGS__)

bool FileExists(const char* file);
size_t FileSize(const char* file);
char * FileDir(const char * path);

const char* va(const char* format, ...);
const char *getOutFile(const char *name);
void CreateDirectoryAnyDepth(const char *path);