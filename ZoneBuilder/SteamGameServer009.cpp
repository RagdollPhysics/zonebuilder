// ==========================================================
// IW4M project
// 
// Component: clientdll
// Sub-component: steam_api
// Purpose: ISteamGameServer009 implementation
//
// Initial author: NTAuthority
// Started: 2010-09-28
// ==========================================================

#include "StdInc.h"
#include "SteamGameServer009.h"

// TODO: cause Steam auth to be requested even after a map change (server reinit)
void CSteamGameServer009::LogOn() {
	
}

void CSteamGameServer009::LogOff() {
	
}

bool CSteamGameServer009::LoggedOn() {
	
	return true;
}

bool CSteamGameServer009::Secure() {
	
	return false;
}

CSteamID CSteamGameServer009::GetSteamID() {
		return CSteamID();
}

bool CSteamGameServer009::SendUserConnectAndAuthenticate( uint32 unIPClient, const void *pvAuthBlob, uint32 cubAuthBlobSize, CSteamID *pSteamIDUser ) {
	return true;
}

CSteamID CSteamGameServer009::CreateUnauthenticatedUserConnection() {
	return CSteamID();
}

void CSteamGameServer009::SendUserDisconnect( CSteamID steamIDUser ) {
	
}

bool CSteamGameServer009::UpdateUserData( CSteamID steamIDUser, const char *pchPlayerName, uint32 uScore ) {
	
	return true;
}

bool CSteamGameServer009::SetServerType( uint32 unServerFlags, uint32 unGameIP, uint16 unGamePort, uint16 unSpectatorPort, uint16 usQueryPort, const char *pchGameDir, const char *pchVersion, bool bLANMode ) {
	
	return true;
}

void CSteamGameServer009::UpdateServerStatus( int cPlayers, int cPlayersMax, int cBotPlayers, const char *pchServerName, const char *pSpectatorServerName, const char *pchMapName ) {
	
}

void CSteamGameServer009::UpdateSpectatorPort( uint16 unSpectatorPort ) { }

void CSteamGameServer009::SetGameType( const char *pchGameType ) { }

bool CSteamGameServer009::GetUserAchievementStatus( CSteamID steamID, const char *pchAchievementName ) {
	
	return false;
}

void CSteamGameServer009::GetGameplayStats( ) {}

bool CSteamGameServer009::RequestUserGroupStatus( CSteamID steamIDUser, CSteamID steamIDGroup ) {
	
	return false;
}

uint32 CSteamGameServer009::GetPublicIP() {
	
	return 0;
}

void CSteamGameServer009::SetGameData( const char *pchGameData) {
	
}

EUserHasLicenseForAppResult CSteamGameServer009::UserHasLicenseForApp( CSteamID steamID, AppId_t appID ) {
	return k_EUserHasLicenseResultHasLicense;
}
