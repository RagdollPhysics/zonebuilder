// ==========================================================
// IW4M project
// 
// Component: clientdll
// Sub-component: steam_api
// Purpose: ISteamFriends005 implementation
//
// Initial author: NTAuthority
// Started: 2010-09-27
// ==========================================================

#include "StdInc.h"
#include "SteamFriends005.h"
#include <time.h>

static char g_userName[256];
static bool g_nameSet = false;

const char *CSteamFriends005::GetPersonaName()
{
	return "TheApadayo";
}

void CSteamFriends005::SetPersonaName( const char *pchPersonaName )
{
}

EPersonaState CSteamFriends005::GetPersonaState()
{
	

	return k_EPersonaStateOnline;
}

int CSteamFriends005::GetFriendCount( EFriendFlags iFriendFlags )
{
	

	return 0;
}

CSteamID CSteamFriends005::GetFriendByIndex( int iFriend, int iFriendFlags )
{
	

	return CSteamID();
}

EFriendRelationship CSteamFriends005::GetFriendRelationship( CSteamID steamIDFriend )
{
	

	return k_EFriendRelationshipNone;
}

EPersonaState CSteamFriends005::GetFriendPersonaState( CSteamID steamIDFriend )
{
	

	return k_EPersonaStateOffline;
}

const char *CSteamFriends005::GetFriendPersonaName( CSteamID steamIDFriend )
{
	

	return "UnknownFriend";
}

int CSteamFriends005::GetFriendAvatar( CSteamID steamIDFriend, int eAvatarSize )
{
	

	return 0;
}

bool CSteamFriends005::GetFriendGamePlayed( CSteamID steamIDFriend, FriendGameInfo_t *pFriendGameInfo )
{
	

	return false;
}

const char *CSteamFriends005::GetFriendPersonaNameHistory( CSteamID steamIDFriend, int iPersonaName )
{
	

	return "";
}

bool CSteamFriends005::HasFriend( CSteamID steamIDFriend, EFriendFlags iFriendFlags )
{
	

	return false;
}

int CSteamFriends005::GetClanCount()
{
	

	return 0;
}

CSteamID CSteamFriends005::GetClanByIndex( int iClan )
{
	

	return CSteamID();
}

const char *CSteamFriends005::GetClanName( CSteamID steamIDClan )
{
	

	return "c14n";
}

int CSteamFriends005::GetFriendCountFromSource( CSteamID steamIDSource )
{
	

	return 0;
}

CSteamID CSteamFriends005::GetFriendFromSourceByIndex( CSteamID steamIDSource, int iFriend )
{
	

	return CSteamID();
}

bool CSteamFriends005::IsUserInSource( CSteamID steamIDUser, CSteamID steamIDSource )
{
	

	return false;
}

void CSteamFriends005::SetInGameVoiceSpeaking( CSteamID steamIDUser, bool bSpeaking )
{
	
}

void CSteamFriends005::ActivateGameOverlay( const char *pchDialog )
{

}

void CSteamFriends005::ActivateGameOverlayToUser( const char *pchDialog, CSteamID steamID )
{
	
}

void CSteamFriends005::ActivateGameOverlayToWebPage( const char *pchURL )
{
	
}

void CSteamFriends005::ActivateGameOverlayToStore( AppId_t nAppID )
{
	
}

void CSteamFriends005::SetPlayedWith( CSteamID steamIDUserPlayedWith )
{

}