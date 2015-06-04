#pragma once

#if PRE_RELEASE_DEMO
#define PROTOCOL (146 | 0xF000)
#else
#define PROTOCOL 144
#endif

#define CODE_START 0x401000
#define CODE_END 0x6D7000

#define MAX_CLIENTS 18

#define CLIENTNUM(x) ((DWORD)x - 0x31D9390) / 681872

typedef enum assetType_e
{
	ASSET_TYPE_PHYSPRESET = 0,
	ASSET_TYPE_PHYS_COLLMAP = 1,
	ASSET_TYPE_XANIM = 2,
	ASSET_TYPE_XMODELSURFS = 3,
	ASSET_TYPE_XMODEL = 4,
	ASSET_TYPE_MATERIAL = 5,
	ASSET_TYPE_PIXELSHADER = 6,
	ASSET_TYPE_VERTEXSHADER = 7,
	ASSET_TYPE_VERTEXDECL = 8,
	ASSET_TYPE_TECHSET = 9,
	ASSET_TYPE_IMAGE = 10,
	ASSET_TYPE_SOUND = 11,
	ASSET_TYPE_SNDCURVE = 12,
	ASSET_TYPE_LOADED_SOUND = 13,
	ASSET_TYPE_COL_MAP_SP = 14,
	ASSET_TYPE_COL_MAP_MP = 15,
	ASSET_TYPE_COM_MAP = 16,
	ASSET_TYPE_GAME_MAP_SP = 17,
	ASSET_TYPE_GAME_MAP_MP = 18,
	ASSET_TYPE_MAP_ENTS = 19,
	ASSET_TYPE_FX_MAP = 20,
	ASSET_TYPE_GFX_MAP = 21,
	ASSET_TYPE_LIGHTDEF = 22,
	ASSET_TYPE_UI_MAP = 23,
	ASSET_TYPE_FONT = 24,
	ASSET_TYPE_MENUFILE = 25,
	ASSET_TYPE_MENU = 26,
	ASSET_TYPE_LOCALIZE = 27,
	ASSET_TYPE_WEAPON = 28,
	ASSET_TYPE_SNDDRIVERGLOBALS = 29,
	ASSET_TYPE_FX = 30,
	ASSET_TYPE_IMPACTFX = 31,
	ASSET_TYPE_AITYPE = 32,
	ASSET_TYPE_MPTYPE = 33,
	ASSET_TYPE_CHARACTER = 34,
	ASSET_TYPE_XMODELALIAS = 35,
	ASSET_TYPE_RAWFILE = 36,
	ASSET_TYPE_STRINGTABLE = 37,
	ASSET_TYPE_LEADERBOARDDEF = 38,
	ASSET_TYPE_STRUCTUREDDATADEF = 39,
	ASSET_TYPE_TRACER = 40,
	ASSET_TYPE_VEHICLE = 41,
	ASSET_TYPE_ADDON_MAP_ENTS = 42,
	ASSET_TYPE_MAX = 43
} assetType_t;

typedef enum
{
	DVAR_FLAG_NONE			= 0x0,			//no flags
	DVAR_FLAG_ARCHIVE		= 0x1,			//saves in config_mp.cfg for clients
	DVAR_FLAG_LATCHED		= 0x2,			//no changing apart from initial value (although it might apply on a map reload, I think)
	DVAR_FLAG_CHEAT			= 0x4,			//cheat
	DVAR_FLAG_REPLICATED	= 0x8,			//on change, this is sent to all clients (if you are host)
	DVAR_FLAG_UNKNOWN10		= 0x10,			//unknown
	DVAR_FLAG_UNKNOWN20		= 0x20,			//unknown
	DVAR_FLAG_UNKNOWN40		= 0x40,			//unknown
	DVAR_FLAG_UNKNOWN80		= 0x80,			//unknown
	DVAR_FLAG_USERCREATED	= 0x100,		//a 'set' type command created it
	DVAR_FLAG_USERINFO		= 0x200,		//userinfo?
	DVAR_FLAG_SERVERINFO	= 0x400,		//in the getstatus oob
	DVAR_FLAG_WRITEPROTECTED= 0x800,		//write protected
	DVAR_FLAG_UNKNOWN1000	= 0x1000,		//unknown
	DVAR_FLAG_READONLY		= 0x2000,		//read only (same as 0x800?)
	DVAR_FLAG_UNKNOWN4000	= 0x4000,		//unknown
	DVAR_FLAG_UNKNOWN8000	= 0x8000,		//unknown
	DVAR_FLAG_NONEXISTENT	= 0xFFFFFFFF	//no such dvar
} dvar_flag;
typedef enum
{
	DVAR_TYPE_BOOL		= 0,
	DVAR_TYPE_FLOAT		= 1,
	DVAR_TYPE_FLOAT_2	= 2,
	DVAR_TYPE_FLOAT_3	= 3,
	DVAR_TYPE_FLOAT_4	= 4,
	DVAR_TYPE_INT		= 5,
	DVAR_TYPE_ENUM		= 6,
	DVAR_TYPE_STRING	= 7,
	DVAR_TYPE_COLOR		= 8,
	//DVAR_TYPE_INT64	= 9 only in Tx
} dvar_type;
// complete! 72/72 bytes figured out ;)
union dvar_value_t {
		char*	string;
		int		integer;
		float	value;
		bool	boolean;
		float	vec2[2];
		float	vec3[3];
		float	vec4[4];
		BYTE	color[4]; //to get float: multiply by 0.003921568859368563 - BaberZz
		//__int64 integer64; only in Tx
};
union dvar_maxmin_t {
	int i;
	float f;
};
typedef struct dvar_t
{
	//startbyte:endbyte
	const char*		name; //0:3
	const char*		description; //4:7
	short			flags; //8:9
	char			pad1[2]; //10:11
	char			type; //12:12
	char			pad2[3]; //13:15
	dvar_value_t	current; //16:31
	dvar_value_t	latched; //32:47
	dvar_value_t	default; //48:64
	dvar_maxmin_t min; //65:67
	dvar_maxmin_t max; //68:72 woooo
} dvar_t;

typedef struct cmd_function_s
{
	char pad[24];
} cmd_function_t;

// netadr_t
typedef enum {
	NA_BOT,
	NA_BAD,					// an address lookup failed
	NA_LOOPBACK,
	NA_BROADCAST,
	NA_IP,
	//NA_IP6,
} netadrtype_t;

typedef enum {
	NS_CLIENT,
	NS_SERVER
} netsrc_t;

typedef struct {
	netadrtype_t	type;

	BYTE	ip[4];

	unsigned short	port;

	BYTE	ipx[10];
} netadr_t;


/*#pragma pack(push, 1)
typedef struct
{
	netadrtype_t type;

	union
	{
		BYTE ip[4];
		BYTE ip6[16];
	};

	unsigned short port;
} netadr_t;
#pragma pack(pop)*/

typedef struct  
{
	int unknown1;
	int unknown2;
	char* data;
	int unknown3;
	int maxsize; // 16
	int cursize;
	int unknown4;
	int readcount; // 28
} msg_t;

typedef struct  
{
	int curClients;
	int maxClients;
	char map[128];
} svstatus_t;

struct XZoneInfo {
	const char* name;
	int type1;
	int type2;
};

typedef float vec4_t[4];

typedef struct
{
	float x, y, z;
} Vector3;

/*typedef struct
{
	float x;
	float y;
	float w;
	float h;
} rectangleDef_t;

typedef rectangleDef_t Rectangle;*/

typedef struct  
{
	int type;
	int id;
	void* extraData;
} menuExpToken_t;

typedef struct  
{
	int count;
	menuExpToken_t* tokens;
} menuExpression_t;

typedef struct  
{
	void* data;
	int type;
} menuStatement_t;

typedef struct  
{
	int numStatements;
	menuStatement_t** statements;
} menuScript_t;

typedef struct
{
	char* name;
	vec4_t rect;
	char rectAlignX;
	char rectAlignY;
	// 22
	char pad[2];
	// 24
	float originX;
	float originY;
	// 32
	char pad2[12];
	// 44
	char* group;
	// 48
	int style;
	int border;
	int ownerDraw;
	int ownerDrawFlags;
	float borderSize;
	// 68
	int flags1;
	int flags2;
	int unknown1;
	// 80
	vec4_t foreColor;
	vec4_t backColor;
	vec4_t borderColor;
	vec4_t outlineColor;
	// 144
	vec4_t unknownColor;
	// 160
	void* background;
} windowDef_t;

typedef windowDef_t Window;

#define MAX_LB_COLUMNS 16

typedef struct columnInfo_s {
	int pos;
	int width;
	int maxChars;
	int unknown;
} columnInfo_t;

typedef struct listBoxDef_s {
	int cursorPos;
	int unk;
	int startPos;
	int endPos;
	float elementWidth;
	float elementHeight;
	int elementStyle;
	int numColumns;
	columnInfo_t columnInfo[MAX_LB_COLUMNS];
	menuScript_t* doubleClick;
	int notselectable;
	int noscrollbars;
	int usepaging;
} listBoxDef_t;

typedef struct  
{
	int type;
	menuExpression_t* expression;
} componentExpression_t;

typedef struct
{
	int expression;
	char* comp1;
	char* comp2;
} componentExpressionDef_t;

componentExpressionDef_t* componentExpressions = (componentExpressionDef_t*)0x745FA0;

typedef struct execKey_s
{
	union
	{
		char key[4];
		int keyInt;
	};
	menuScript_t* script;
	execKey_s* next;
} execKey_t;

typedef struct  
{
	Window window;
	// 164
	char pad1[20];
	// 184
	int type;
	int unknownType;
	int align;
	int textFont;
	int textAlign;
	float textAlignX;
	float textAlignY;
	float textScale;
	int textStyle;
	int unknown;
	int unknown2;
	char* text;
	int unknown3;
	int unknown4;
	menuScript_t* mouseEnterText;
	menuScript_t* mouseExitText;
	menuScript_t* mouseEnter;
	menuScript_t* mouseExit;
	menuScript_t* action;
	menuScript_t* accept;
	menuScript_t* onFocus;
	menuScript_t* leaveFocus;
	char* dvar;
	char* dvarTest;
	execKey_t* execKeys;
	void* testDvarValue;
	char* localVar;
	int testDvarType;
	char** focusSound;
	float feeder;
	int cursorPos;
	void* typeData;
	char pad2[4];
	int numExpressions;
	componentExpression_t* expressions;
	menuExpression_t* visibleExp;
	menuExpression_t* disabledExp;
	menuExpression_t* textExp;
	menuExpression_t* materialExp;
} itemDef_t;

typedef struct
{
	Window window;
	int unknown2;
	int fullscreen;
	int numItems;
	int unknown3;
	int unknown4;
	int fadeCycle;
	float fadeClamp;
	float fadeAmount;
	float fadeInAmount;
	float blurWorld;
	menuScript_t* onOpen;
	menuScript_t* onRequestClose;
	menuScript_t* onClose;
	menuScript_t* onESC;
	execKey_t* execKeys;
	void* visibleExp;
	char* allowedBinding;
	char* soundLoop;
	int unknown5;
	vec4_t focusColor;
	void* rectXExp;
	void* rectYExp;
	void* rectHExp;
	void* rectWExp;
	void* openSoundExp;
	void* closeSoundExp;
	itemDef_t** items;
} menuDef_t;

typedef struct
{
	char			pakFilename[256];			// c:\quake3\baseq3\pak0.pk3
	char			pakBasename[256];			// pak0
	char			pakGamename[256];			// baseq3
	int				handle;						// handle to zip file
	int				checksum;
} pack_t;

typedef struct {
	char		path[256];		// c:\quake3
	char		gamedir[256];	// baseq3
} directory_t;

typedef struct searchpath_s
{
	searchpath_s* next;
	pack_t* pack;
	directory_t* dir;
} searchpath_t;

// vars
extern searchpath_t* fs_searchpaths;
extern dvar_t* sv_rconPassword;

// functions
const char* CL_GetServerIPAddress();

// inline cmd functions
extern DWORD* cmd_id;
extern DWORD* cmd_argc;
extern DWORD** cmd_argv;

inline int	Cmd_Argc( void )
{
	return cmd_argc[*cmd_id];
}

inline char *Cmd_Argv( int arg )
{
	if ( (unsigned)arg >= cmd_argc[*cmd_id] ) {
		return "";
	}
	return (char*)(cmd_argv[*cmd_id][arg]);	
}

// internal functions (custom)
void Com_BeginRedirect( char *buffer, int buffersize, void ( *flush )( char * ) );
void        Com_EndRedirect( void );

int	Cmd_ArgcSV( void );
char *Cmd_ArgvSV( int );

void SV_GetStatus(svstatus_t* status);
char* GetStringConvar(char* key);

void NET_OutOfBandPrint(int type, netadr_t adr, const char* message, ...);
int Party_NumPublicSlots();

bool SV_IsClientIP(unsigned int ip);

#define FS_SEEK_CUR 0
#define FS_SEEK_END 1
#define FS_SEEK_SET 2

// inline cmd functions
extern DWORD* cmd_id;
extern DWORD* cmd_argc;
extern DWORD** cmd_argv;

// types
typedef void (__cdecl * CommandCB_t)(void);

// original functions
typedef void (__cdecl * Cbuf_AddText_t)(int a1, const char* cmd);
extern Cbuf_AddText_t Cbuf_AddText;


typedef void (__cdecl * CL_AddReliableCommand_t)(int controller, const char* command);
extern CL_AddReliableCommand_t CL_AddReliableCommand;


typedef void (__cdecl * Cmd_AddCommand_t)(const char* name, CommandCB_t callback, cmd_function_t* data, char);
extern Cmd_AddCommand_t Cmd_AddCommand;

typedef void (__cdecl * Cmd_AddServerCommand_t)(const char* name, CommandCB_t callback, cmd_function_t* data);
extern Cmd_AddServerCommand_t Cmd_AddServerCommand;

typedef void (__cdecl * Cmd_ExecuteSingleCommand_t)(int controller, int a2, const char* cmd);
extern Cmd_ExecuteSingleCommand_t Cmd_ExecuteSingleCommand;

typedef void (__cdecl * Cmd_SetAutoComplete_t)(const char* name, const char* path, const char* extension);
extern Cmd_SetAutoComplete_t Cmd_SetAutoComplete;


typedef void (__cdecl * Com_Error_t)(int type, char* message, ...);
extern Com_Error_t Com_Error_MW22;

typedef void (__cdecl * Com_Printf_t)(int, const char*, ...);
extern Com_Printf_t Com_Printf_MW22;

typedef void (__cdecl * Com_PrintError_t)(int, const char*, ...);
extern Com_PrintError_t Com_PrintError;

typedef int (__cdecl * Com_Milliseconds_t)(void);
extern Com_Milliseconds_t Com_Milliseconds;

typedef char* (__cdecl * Com_ParseExt_t)(char**);
extern Com_ParseExt_t Com_ParseExt;

typedef void (__cdecl * Com_BeginParseSession_t)(const char* why);
extern Com_BeginParseSession_t Com_BeginParseSession;

typedef void (__cdecl * Com_EndParseSession_t)();
extern Com_EndParseSession_t Com_EndParseSession;

typedef bool (__cdecl * Com_Parse_MatchToken_t)(char** file, char* token, bool die);
extern Com_Parse_MatchToken_t Com_Parse_MatchToken;

typedef int (__cdecl * Com_ParseInt_t)(char**file);
extern Com_ParseInt_t Com_ParseInt;

typedef float (__cdecl * Com_ParseFloat_t)(char**file);
extern Com_ParseFloat_t Com_ParseFloat;

typedef float (__cdecl * Com_Parse_UngetToken_t)();
extern Com_Parse_UngetToken_t Com_Parse_UngetToken;

typedef void (*DB_LoadXAssets_t)(XZoneInfo* data, int count, int unknown);
extern DB_LoadXAssets_t DB_LoadXAssets;

typedef void* (__cdecl * DB_FindXAssetHeader_t)(int type, const char* filename);
extern DB_FindXAssetHeader_t DB_FindXAssetHeader;

typedef void* (__cdecl * DB_IsAssetDefault_t)(int type, const char* filename);
extern DB_IsAssetDefault_t DB_IsAssetDefault;

typedef int (__cdecl * DB_GetXAssetTypeSize_t)(int type);
extern DB_GetXAssetTypeSize_t DB_GetXAssetTypeSize;

typedef void(__cdecl * DB_EnumXAssets_t)(assetType_t type, void(*handler)(void*, int), int userData);
extern DB_EnumXAssets_t DB_EnumXAssets;

typedef dvar_t* (__cdecl * Dvar_RegisterBool_t)(const char* name, bool default, int flags, const char* description);
extern Dvar_RegisterBool_t Dvar_RegisterBool;

typedef dvar_t* (__cdecl * Dvar_RegisterFloat_t)(const char* name, float default, float min, float max, int flags, const char* description);
extern Dvar_RegisterFloat_t Dvar_RegisterFloat;

typedef dvar_t* (__cdecl * Dvar_RegisterFloat2_t)(const char* name, float defx, float defy, float min, float max, int flags, const char* description);
extern Dvar_RegisterFloat2_t Dvar_RegisterFloat2;

typedef dvar_t* (__cdecl * Dvar_RegisterFloat3_t)(const char* name, float defx, float defy, float defz, float min, float max, int flags, const char* description);
extern Dvar_RegisterFloat3_t Dvar_RegisterFloat3;

typedef dvar_t* (__cdecl * Dvar_RegisterFloat4_t)(const char* name, float defx, float defy, float defz, float defw, float min, float max, int flags, const char* description);
extern Dvar_RegisterFloat4_t Dvar_RegisterFloat4;

typedef dvar_t* (__cdecl * Dvar_RegisterInt_t)(const char* name, int default, int min, int max, int flags, const char* description);
extern Dvar_RegisterInt_t Dvar_RegisterInt;

typedef dvar_t* (__cdecl * Dvar_RegisterEnum_t)(const char* name, char** enumValues, int default, int flags, const char* description);
extern Dvar_RegisterEnum_t Dvar_RegisterEnum;

typedef dvar_t* (__cdecl * Dvar_RegisterString_t)(const char* name, const char* default, int, const char*);
extern Dvar_RegisterString_t Dvar_RegisterString;

typedef dvar_t* (__cdecl * Dvar_RegisterColor_t)(const char* name, float r, float g, float b, float a, int flags, const char* description);
extern Dvar_RegisterColor_t Dvar_RegisterColor;

typedef dvar_t* (__cdecl * Dvar_FindVar_t)(char* dvar);
extern Dvar_FindVar_t Dvar_FindVar;

typedef char* (__cdecl* Dvar_InfoString_Big_t)(int typeMask);
extern Dvar_InfoString_Big_t Dvar_InfoString_Big;

typedef dvar_t* (__cdecl * Dvar_SetCommand_t)(const char* name, const char* value);
extern Dvar_SetCommand_t Dvar_SetCommand;


typedef void (__cdecl * FS_FreeFile_t)(void* buffer);
extern FS_FreeFile_t FS_FreeFile;

typedef int (__cdecl * FS_ReadFile_t)(const char* qpath, void** buffer);
extern FS_ReadFile_t FS_ReadFile;

typedef char** (__cdecl * FS_ListFiles_t)(char* path, char* extension, int noclue, int* amount);
extern FS_ListFiles_t FS_ListFiles;

typedef void (__cdecl * FS_FreeFileList_t)(char** list);
extern FS_FreeFileList_t FS_FreeFileList;

typedef int (__cdecl * FS_FOpenFileAppend_t)(char* file);
extern FS_FOpenFileAppend_t FS_FOpenFileAppend;
extern FS_FOpenFileAppend_t FS_FOpenFileWrite;

typedef int (__cdecl * FS_FOpenFileRead_t)(const char* file, int* fh, int uniqueFile);
extern FS_FOpenFileRead_t FS_FOpenFileRead;

typedef int (__cdecl * FS_FCloseFile_t)(int fh);
extern FS_FCloseFile_t FS_FCloseFile;

typedef bool (__cdecl * FS_WriteFile_t)(char* filename, char* folder, void* buffer, int size);
extern FS_WriteFile_t FS_WriteFile;

typedef int (__cdecl * FS_Write_t)(void* buffer, size_t size, int file);
extern FS_Write_t FS_Write;

typedef int (__cdecl * FS_Read_t)(void* buffer, size_t size, int file);
extern FS_Read_t FS_Read;

typedef int (__cdecl * FS_Seek_t)(int fileHandle, int seekPosition, int seekOrigin);
extern FS_Seek_t FS_Seek;


typedef void (__cdecl * G_LogPrintf_t)(char*, ...);
extern G_LogPrintf_t G_LogPrintf;


typedef void (__cdecl * MSG_Init_t)(void* msg, void* data, int maxsize);
extern MSG_Init_t MSG_Init;


typedef int (__cdecl * MSG_ReadData_t)(msg_t* msg, char*, size_t);
extern MSG_ReadData_t MSG_ReadData;

typedef int (__cdecl * MSG_ReadLong_t)(msg_t* msg);
extern MSG_ReadLong_t MSG_ReadLong;

typedef void (__cdecl * MSG_WriteByte_t)(msg_t* msg, unsigned char);
extern MSG_WriteByte_t MSG_WriteByte;

typedef void (__cdecl * MSG_WriteData_t)(msg_t* msg, char*, size_t);
extern MSG_WriteData_t MSG_WriteData;

typedef void (__cdecl * MSG_WriteLong_t)(msg_t* msg, int);
extern MSG_WriteLong_t MSG_WriteLong;


typedef const char* (__cdecl * NET_AdrToString_t)(netadr_t adr);
extern NET_AdrToString_t NET_AdrToString;

typedef bool (__cdecl * NET_CompareAdr_t)(netadr_t, netadr_t);
extern NET_CompareAdr_t NET_CompareAdr;

typedef bool (__cdecl * NET_StringToAdr_t)(const char*, netadr_t*);
extern NET_StringToAdr_t NET_StringToAdr;

typedef void (__cdecl * SV_GameSendServerCommand_t)(int targetEntity, int a2, const char* command);
extern SV_GameSendServerCommand_t SV_GameSendServerCommand;


typedef bool (__cdecl * Sys_SendPacket_t)(int len, const char* buf, netadr_t adr);
extern Sys_SendPacket_t Sys_SendPacket;

typedef char * (__cdecl * DB_GetFFString_t)(int str);
extern DB_GetFFString_t DB_GetFFString;

typedef char* (__cdecl * SL_ConvertToString_t)(unsigned short);
extern SL_ConvertToString_t SL_ConvertToString;

typedef short (__cdecl * SL_GetString_t)(const char* string);
extern SL_GetString_t SL_GetString;

typedef short (__cdecl * SL_GetString_system_t)(const char* string, int a2);
extern SL_GetString_system_t SL_GetString_2;

// other stuff
extern CommandCB_t Cbuf_AddServerText_f;

// swap
typedef short (__cdecl * BigShort_t)(short);
extern BigShort_t BigShort;

#define CURRENT_ZONE_NAME (char*)(*(DWORD*)0x112A680 + 4)