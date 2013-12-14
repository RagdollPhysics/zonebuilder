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

CL_AddReliableCommand_t CL_AddReliableCommand = (CL_AddReliableCommand_t)0x454F40;

Cmd_AddCommand_t Cmd_AddCommand = (Cmd_AddCommand_t)0x470090;
Cmd_AddServerCommand_t Cmd_AddServerCommand = (Cmd_AddServerCommand_t)0x4DCE00;
Cmd_ExecuteSingleCommand_t Cmd_ExecuteSingleCommand = (Cmd_ExecuteSingleCommand_t)0x609540;
Cmd_SetAutoComplete_t Cmd_SetAutoComplete = (Cmd_SetAutoComplete_t)0x40EDC0;

Com_Error_t Com_Error_MW22 = (Com_Error_t)0x4B22D0;
Com_Printf_t Com_Printf_MW22 = (Com_Printf_t)0x402500;
Com_Milliseconds_t Com_Milliseconds = (Com_Milliseconds_t)0x42A660;
Com_PrintError_t Com_PrintError = (Com_PrintError_t)0x4F8C70;

DB_FindXAssetHeader_t DB_FindXAssetHeader = (DB_FindXAssetHeader_t)0x407930;
DB_LoadXAssets_t DB_LoadXAssets = (DB_LoadXAssets_t)0x4E5930;

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

// other stuff
CommandCB_t Cbuf_AddServerText_f = (CommandCB_t)0x4BB9B0;

// swapping
BigShort_t BigShort = (BigShort_t)0x446E10;

// variables
searchpath_t* fs_searchpaths = (searchpath_t*)0x63D96E0;
int* clientState = (int*)0xB2C540;