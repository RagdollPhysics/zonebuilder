// ==========================================================
// project 'secretSchemes'
// 
// Component: clientdll
// Sub-component: steam_api
// Purpose: Game-specific code implementations.
//
// Initial author: NTAuthority
// Started: 2011-05-04
// ==========================================================

#include "StdInc.h"

// function definitions
Cbuf_AddText_t Cbuf_AddText = (Cbuf_AddText_t)0x404B20;
Cbuf_Execute_t Cbuf_Execute = (Cbuf_Execute_t)0x4E2C80;

CL_AddReliableCommand_t CL_AddReliableCommand = (CL_AddReliableCommand_t)0x454F40;

Cmd_AddCommand_t Cmd_AddCommand = (Cmd_AddCommand_t)0x470090;
Cmd_AddServerCommand_t Cmd_AddServerCommand = (Cmd_AddServerCommand_t)0x4DCE00;
Cmd_ExecuteSingleCommand_t Cmd_ExecuteSingleCommand = (Cmd_ExecuteSingleCommand_t)0x609540;
Cmd_SetAutoComplete_t Cmd_SetAutoComplete = (Cmd_SetAutoComplete_t)0x40EDC0;

Com_Error_t Com_Error_MW22 = (Com_Error_t)0x4B22D0;
Com_Printf_t Com_Printf_MW22 = (Com_Printf_t)0x402500;
Com_Milliseconds_t Com_Milliseconds = (Com_Milliseconds_t)0x42A660;
Com_PrintError_t Com_PrintError = (Com_PrintError_t)0x4F8C70;
Com_ParseExt_t Com_ParseExt = (Com_ParseExt_t)0x474D60;
Com_BeginParseSession_t Com_BeginParseSession = (Com_BeginParseSession_t)0x4AAB80;
Com_EndParseSession_t Com_EndParseSession = (Com_EndParseSession_t)0x4B80B0;
Com_Parse_MatchToken_t Com_Parse_MatchToken = (Com_Parse_MatchToken_t)0x447130;
Com_ParseInt_t Com_ParseInt = (Com_ParseInt_t)0x4EF770;
Com_ParseFloat_t Com_ParseFloat = (Com_ParseFloat_t)0x424360;
Com_Parse_UngetToken_t Com_Parse_UngetToken = (Com_Parse_UngetToken_t)0x4AEC70;

DB_FindXAssetHeader_t DB_FindXAssetHeader = (DB_FindXAssetHeader_t)0x407930;
DB_LoadXAssets_t DB_LoadXAssets = (DB_LoadXAssets_t)0x4E5930;
DB_IsAssetDefault_t DB_IsAssetDefault = (DB_IsAssetDefault_t)0x48E6A0;
DB_GetXAssetTypeSize_t DB_GetXAssetTypeSize = (DB_GetXAssetTypeSize_t)0x491960;
DB_EnumXAssets_t DB_EnumXAssets = (DB_EnumXAssets_t)0x42A770;

Dvar_RegisterBool_t Dvar_RegisterBool = (Dvar_RegisterBool_t)0x4CE1A0;
Dvar_RegisterFloat_t Dvar_RegisterFloat = (Dvar_RegisterFloat_t)0x648440;
Dvar_RegisterFloat2_t Dvar_RegisterFloat2 = (Dvar_RegisterFloat2_t)0x4F6070;
Dvar_RegisterFloat3_t Dvar_RegisterFloat3 = (Dvar_RegisterFloat3_t)0x4EF8E0;
Dvar_RegisterFloat4_t Dvar_RegisterFloat4 = (Dvar_RegisterFloat4_t)0x4F28E0;
Dvar_RegisterInt_t Dvar_RegisterInt = (Dvar_RegisterInt_t)0x479830;
Dvar_RegisterEnum_t Dvar_RegisterEnum = (Dvar_RegisterEnum_t)0x412E40;
Dvar_RegisterString_t Dvar_RegisterString = (Dvar_RegisterString_t)0x4FC7E0;
Dvar_RegisterColor_t Dvar_RegisterColor = (Dvar_RegisterColor_t)0x471500;

Dvar_FindVar_t Dvar_FindVar = (Dvar_FindVar_t)0x4D5390;
Dvar_InfoString_Big_t Dvar_InfoString_Big = (Dvar_InfoString_Big_t)0x4D98A0;
Dvar_SetCommand_t Dvar_SetCommand = (Dvar_SetCommand_t)0x4EE430;

FS_FreeFile_t FS_FreeFile = (FS_FreeFile_t)0x4416B0;
FS_ReadFile_t FS_ReadFile = (FS_ReadFile_t)0x4F4B90;
FS_ListFiles_t FS_ListFiles = (FS_ListFiles_t)0x441BB0;
FS_FreeFileList_t FS_FreeFileList = (FS_FreeFileList_t)0x4A5DE0;
FS_FOpenFileAppend_t FS_FOpenFileAppend = (FS_FOpenFileAppend_t)0x410BB0;
FS_FOpenFileAppend_t FS_FOpenFileWrite = (FS_FOpenFileAppend_t)0x4BA530;
FS_FOpenFileRead_t FS_FOpenFileRead = (FS_FOpenFileRead_t)0x46CBF0;
FS_FCloseFile_t FS_FCloseFile = (FS_FCloseFile_t)0x462000;
FS_WriteFile_t FS_WriteFile = (FS_WriteFile_t)0x426450;
FS_Write_t FS_Write = (FS_Write_t)0x4C06E0;
FS_Read_t FS_Read = (FS_Read_t)0x4A04C0;
FS_Seek_t FS_Seek = (FS_Seek_t)0x4A63D0;

G_LogPrintf_t G_LogPrintf = (G_LogPrintf_t)0x4B0150;

MSG_Init_t MSG_Init = (MSG_Init_t)0x45FCA0;
MSG_ReadData_t MSG_ReadData = (MSG_ReadData_t)0x4527C0;
MSG_ReadLong_t MSG_ReadLong = (MSG_ReadLong_t)0x4C9550;
MSG_WriteByte_t MSG_WriteByte = (MSG_WriteByte_t)0x48C520;
MSG_WriteData_t MSG_WriteData = (MSG_WriteData_t)0x4F4120;
MSG_WriteLong_t MSG_WriteLong = (MSG_WriteLong_t)0x41CA20;

NET_AdrToString_t NET_AdrToString = (NET_AdrToString_t)0x469880;
NET_CompareAdr_t NET_CompareAdr = (NET_CompareAdr_t)0x4D0AA0;
NET_StringToAdr_t NET_StringToAdr = (NET_StringToAdr_t)0x409010;

SV_GameSendServerCommand_t SV_GameSendServerCommand = (SV_GameSendServerCommand_t)0x4BC3A0;

Sys_SendPacket_t Sys_SendPacket = (Sys_SendPacket_t)0x48F500;

SL_ConvertToString_t SL_ConvertToString = (SL_ConvertToString_t)0x4EC1D0;
SL_GetString_t SL_GetString = (SL_GetString_t)0x48F890;
SL_GetString_system_t SL_GetString_2 = (SL_GetString_system_t)0x4CDC10;

SE_Load_t SE_Load = (SE_Load_t)0x502A30;

// other stuff
CommandCB_t Cbuf_AddServerText_f = (CommandCB_t)0x4BB9B0;

// swapping
BigShort_t BigShort = (BigShort_t)0x446E10;

// variables
searchpath_t* fs_searchpaths = (searchpath_t*)0x63D96E0;
int* clientState = (int*)0xB2C540;

// console commands
DWORD* cmd_id = (DWORD*)0x1AAC5D0;
DWORD* cmd_argc = (DWORD*)0x1AAC614;
DWORD** cmd_argv = (DWORD**)0x1AAC634;