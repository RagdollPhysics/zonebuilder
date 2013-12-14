// ==========================================================
// IW4M project
// 
// Component: clientdll
// Sub-component: steam_api
// Purpose: ISteamRemoteStorage002 implementation
//
// Initial author: NTAuthority
// Started: 2010-09-10
// ==========================================================

#include "StdInc.h"
#include "SteamRemoteStorage002.h"

bool CSteamRemoteStorage002::FileWrite( const char *pchFile, const void *pvData, int32 cubData )
{
	return true;
}

int32 CSteamRemoteStorage002::GetFileSize( const char *pchFile )
{
	return 0;
}

int32 CSteamRemoteStorage002::FileRead( const char *pchFile, void *pvData, int32 cubDataToRead )
{
	return 0;
}

bool CSteamRemoteStorage002::FileExists( const char *pchFile )
{
	return false;
}

int32 CSteamRemoteStorage002::GetFileCount()
{
	return 0;
}

const char *CSteamRemoteStorage002::GetFileNameAndSize( int iFile, int32 *pnFileSizeInBytes )
{
	*pnFileSizeInBytes = 0;

	return "";
}

bool CSteamRemoteStorage002::GetQuota( int32 *pnTotalBytes, int32 *puAvailableBytes )
{
	*pnTotalBytes       = 0x10000000;
	*puAvailableBytes   = 0x10000000;

	return true;
}