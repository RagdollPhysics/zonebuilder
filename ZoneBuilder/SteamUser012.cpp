// ==========================================================
// IW4M project
// 
// Component: clientdll
// Sub-component: steam_api
// Purpose: ISteamUser012 implementation
//
// Initial author: NTAuthority
// Started: 2010-09-10
// ==========================================================

#include "StdInc.h"
#include "SteamUser012.h"

unsigned int steamID = 0;

HSteamUser CSteamUser012::GetHSteamUser()
{
	return NULL;
}

bool CSteamUser012::LoggedOn()
{
	return true;
}

CSteamID CSteamUser012::GetSteamID()
{
	return CSteamID(32, 1, k_EUniversePublic, k_EAccountTypeIndividual);
}

int CSteamUser012::InitiateGameConnection( void *pAuthBlob, int cbMaxAuthBlob, CSteamID steamIDGameServer, uint32 unIPServer, uint16 usPortServer, bool bSecure )
{
	return 0;
}

void CSteamUser012::TerminateGameConnection( uint32 unIPServer, uint16 usPortServer )
{
}

void CSteamUser012::TrackAppUsageEvent( CGameID gameID, EAppUsageEvent eAppUsageEvent, const char *pchExtraInfo )
{
}

bool CSteamUser012::GetUserDataFolder( char *pchBuffer, int cubBuffer )
{
	return true;
}

void CSteamUser012::StartVoiceRecording( )
{
}

void CSteamUser012::StopVoiceRecording( )
{
}

EVoiceResult CSteamUser012::GetCompressedVoice( void *pDestBuffer, uint32 cbDestBufferSize, uint32 *nBytesWritten )
{
	return k_EVoiceResultOK;
}

EVoiceResult CSteamUser012::DecompressVoice( void *pCompressed, uint32 cbCompressed, void *pDestBuffer, uint32 cbDestBufferSize, uint32 *nBytesWritten )
{
	return k_EVoiceResultOK;
}

HAuthTicket CSteamUser012::GetAuthSessionTicket( void *pTicket, int cbMaxTicket, uint32 *pcbTicket )
{
	return 0;
}

EBeginAuthSessionResult CSteamUser012::BeginAuthSession( const void *pAuthTicket, int cbAuthTicket, CSteamID steamID )
{
	return k_EBeginAuthSessionResultOK;
}

void CSteamUser012::EndAuthSession( CSteamID steamID )
{

}

void CSteamUser012::CancelAuthTicket( HAuthTicket hAuthTicket )
{

}

uint32 CSteamUser012::UserHasLicenseForApp( CSteamID steamID, AppId_t appID )
{
	return 1;
}
