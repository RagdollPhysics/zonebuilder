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

class CSteamRemoteStorage002 : public ISteamRemoteStorage002
{
public:
	bool FileWrite( const char *pchFile, const void *pvData, int32 cubData );
	int32 GetFileSize( const char *pchFile );
	int32 FileRead( const char *pchFile, void *pvData, int32 cubDataToRead );
	bool FileExists( const char *pchFile );
	int32 GetFileCount();
	const char *GetFileNameAndSize( int iFile, int32 *pnFileSizeInBytes );
	bool GetQuota( int32 *pnTotalBytes, int32 *puAvailableBytes );
};