// ==========================================================
// IW4M project
// 
// Component: clientdll
// Sub-component: steam_api
// Purpose: ISteamMatchmaking007 implementation
//
// Initial author: NTAuthority
// Started: 2010-09-27
// ==========================================================

#include "StdInc.h"
#include "SteamMatchmaking007.h"

unsigned int GetPlayerSteamID();

// ^{[a-z0-9_]*} {.*}\({.*}\) \{

int CSteamMatchmaking007::GetFavoriteGameCount() {
	return 0;
}

bool CSteamMatchmaking007::GetFavoriteGame( int iGame, AppId_t *pnAppID, uint32 *pnIP, uint16 *pnConnPort, uint16 *pnQueryPort, uint32 *punFlags, RTime32 *pRTime32LastPlayedOnServer ) {
	return false;
}

int CSteamMatchmaking007::AddFavoriteGame( AppId_t nAppID, uint32 nIP, uint16 nConnPort, uint16 nQueryPort, uint32 unFlags, RTime32 rTime32LastPlayedOnServer ) {
	return 0;
}

bool CSteamMatchmaking007::RemoveFavoriteGame( AppId_t nAppID, uint32 nIP, uint16 nConnPort, uint16 nQueryPort, uint32 unFlags ) {
	return false;
}

SteamAPICall_t CSteamMatchmaking007::RequestLobbyList() {
	return NULL;
}

void CSteamMatchmaking007::AddRequestLobbyListStringFilter( const char *pchKeyToMatch, const char *pchValueToMatch, ELobbyComparison eComparisonType ) {
	
}

void CSteamMatchmaking007::AddRequestLobbyListNumericalFilter( const char *pchKeyToMatch, int nValueToMatch, ELobbyComparison nComparisonType ) {
	
}

void CSteamMatchmaking007::AddRequestLobbyListNearValueFilter( const char *pchKeyToMatch, int nValueToBeCloseTo ) {
	
}

void CSteamMatchmaking007::AddRequestLobbyListFilterSlotsAvailable( int ) {
	
}

CSteamID CSteamMatchmaking007::GetLobbyByIndex( int iLobby ) {
	return CSteamID( 1337132, 0x40000, k_EUniversePublic, k_EAccountTypeChat );
}

SteamAPICall_t CSteamMatchmaking007::CreateLobby( ELobbyType eLobbyType, int ) {
	return NULL;
}

SteamAPICall_t CSteamMatchmaking007::JoinLobby( CSteamID steamIDLobby ) {
	return NULL;
}

void CSteamMatchmaking007::LeaveLobby( CSteamID steamIDLobby ) {
	
}

bool CSteamMatchmaking007::InviteUserToLobby( CSteamID steamIDLobby, CSteamID steamIDInvitee ) {
	
	return true;
}

int CSteamMatchmaking007::GetNumLobbyMembers( CSteamID steamIDLobby ) {
	//
	return 1;
}

CSteamID CSteamMatchmaking007::GetLobbyMemberByIndex( CSteamID steamIDLobby, int iMember ) {
	return CSteamID();
}

const char *CSteamMatchmaking007::GetLobbyData( CSteamID steamIDLobby, const char *pchKey ) {
	// do some switch magic (not supposed to be here, don't do this)
	return "";
}

bool CSteamMatchmaking007::SetLobbyData( CSteamID steamIDLobby, const char *pchKey, const char *pchValue ) {
	return true;
}

int CSteamMatchmaking007::GetLobbyDataCount( CSteamID steamIDLobby ) {
	
	return 0;
}

bool CSteamMatchmaking007::GetLobbyDataByIndex( CSteamID steamIDLobby, int iData, char *pchKey, int cubKey, char *pchValue, int cubValue ) {
	
	return false;
}

bool CSteamMatchmaking007::DeleteLobbyData( CSteamID steamIDLobby, const char *pchKey ) {
	
	return 0;
}

const char * CSteamMatchmaking007::GetLobbyMemberData( CSteamID steamIDLobby, CSteamID steamIDUser, const char *pchKey ) {
	
	return "";
}

void CSteamMatchmaking007::SetLobbyMemberData( CSteamID steamIDLobby, const char *pchKey, const char *pchValue ) {
	
}

bool CSteamMatchmaking007::SendLobbyChatMsg( CSteamID steamIDLobby, const void *pvMsgBody, int cubMsgBody ) {
	
	return true;
}

int CSteamMatchmaking007::GetLobbyChatEntry( CSteamID steamIDLobby, int iChatID, CSteamID *pSteamIDUser, void *pvData, int cubData, EChatEntryType *peChatEntryType ) {
	
	return 0;
}

bool CSteamMatchmaking007::RequestLobbyData( CSteamID steamIDLobby ) {
	
	return false;
}

void CSteamMatchmaking007::SetLobbyGameServer( CSteamID steamIDLobby, uint32 unGameServerIP, uint16 unGameServerPort, CSteamID steamIDGameServer ) {
	
}

bool CSteamMatchmaking007::GetLobbyGameServer( CSteamID steamIDLobby, uint32 *punGameServerIP, uint16 *punGameServerPort, CSteamID *psteamIDGameServer ) {
	
	return false;
}

bool CSteamMatchmaking007::SetLobbyMemberLimit( CSteamID steamIDLobby, int cMaxMembers ) {
	
	return true;
}

int CSteamMatchmaking007::GetLobbyMemberLimit( CSteamID steamIDLobby ) {
	
	return 0;
}

bool CSteamMatchmaking007::SetLobbyType( CSteamID steamIDLobby, ELobbyType eLobbyType ) {
	
	return true;
}

bool CSteamMatchmaking007::SetLobbyJoinable( CSteamID steamIDLobby, bool bJoinable ) {
	
	return true;
}

CSteamID CSteamMatchmaking007::GetLobbyOwner( CSteamID steamIDLobby ) {
	return CSteamID();
}

bool CSteamMatchmaking007::SetLobbyOwner( CSteamID steamIDLobby, CSteamID steamIDNewOwner ) {
	return true;
}
