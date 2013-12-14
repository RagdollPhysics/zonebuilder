// ==========================================================
// IW4M project
// 
// Component: clientdll
// Sub-component: steam_api
// Purpose: ISteamFriends005 definition
//
// Initial author: Open Steamworks project
// Started: 2010-09-19
// ==========================================================

class CSteamFriends005 : public ISteamFriends005
{
public:
	// returns the local players name - guaranteed to not be NULL.
	// this is the same name as on the users community profile page
	// this is stored in UTF-8 format
	// like all the other interface functions that return a char *, it's important that this pointer is not saved
	// off; it will eventually be free'd or re-allocated
	const char *GetPersonaName();

	// sets the player name, stores it on the server and publishes the changes to all friends who are online
	void SetPersonaName( const char *pchPersonaName );

	// gets the status of the current user
	EPersonaState GetPersonaState();

	// friend iteration
	// takes a set of k_EFriendFlags, and returns the number of users the client knows about who meet that criteria
	// then GetFriendByIndex() can then be used to return the id's of each of those users
	int GetFriendCount( EFriendFlags eFriendFlags );

	// returns the steamID of a user
	// iFriend is a index of range [0, GetFriendCount())
	// iFriendsFlags must be the same value as used in GetFriendCount()
	// the returned CSteamID can then be used by all the functions below to access details about the user
	CSteamID GetFriendByIndex( int iFriend, int iFriendFlags );

	// returns a relationship to a user
	EFriendRelationship GetFriendRelationship( CSteamID steamIDFriend );

	// returns the current status of the specified user
	// this will only be known by the local user if steamIDFriend is in their friends list; on the same game server; in a chat room or lobby; or in a small group with the local user
	EPersonaState GetFriendPersonaState( CSteamID steamIDFriend );

	// returns the name another user - guaranteed to not be NULL.
	// same rules as GetFriendPersonaState() apply as to whether or not the user knowns the name of the other user
	// note that on first joining a lobby, chat room or game server the local user will not known the name of the other users automatically; that information will arrive asyncronously
	const char *GetFriendPersonaName( CSteamID steamIDFriend );

	// gets the avatar of the current user, which is a handle to be used in IClientUtils::GetImageRGBA(), or 0 if none set
	int GetFriendAvatar( CSteamID steamIDFriend, int eAvatarSize );
	// returns true if the friend is actually in a game, and fills in pFriendGameInfo with an extra details 
	bool GetFriendGamePlayed( CSteamID steamIDFriend, FriendGameInfo_t *pFriendGameInfo );
	// accesses old friends names - returns an empty string when their are no more items in the history
	const char *GetFriendPersonaNameHistory( CSteamID steamIDFriend, int iPersonaName );

	// returns true if the specified user meets any of the criteria specified in iFriendFlags
	// iFriendFlags can be the union (binary or, |) of one or more k_EFriendFlags values
	bool HasFriend( CSteamID steamIDFriend, EFriendFlags eFriendFlags );

	// clan (group) iteration and access functions
	int GetClanCount();
	CSteamID GetClanByIndex( int iClan );
	const char *GetClanName( CSteamID steamIDClan );

	// iterators for getting users in a chat room, lobby, game server or clan
	// note that large clans that cannot be iterated by the local user
	// steamIDSource can be the steamID of a group, game server, lobby or chat room
	int GetFriendCountFromSource( CSteamID steamIDSource );
	CSteamID GetFriendFromSourceByIndex( CSteamID steamIDSource, int iFriend );

	// returns true if the local user can see that steamIDUser is a member or in steamIDSource
	bool IsUserInSource( CSteamID steamIDUser, CSteamID steamIDSource );

	// User is in a game pressing the talk button (will suppress the microphone for all voice comms from the Steam friends UI)
	void SetInGameVoiceSpeaking( CSteamID steamIDUser, bool bSpeaking );

	// activates the game overlay, with an optional dialog to open 
	// valid options are "Friends", "Community", "Players", "Settings", "LobbyInvite", "OfficialGameGroup"
	void ActivateGameOverlay( const char *pchDialog );

	// activates game overlay to a specific place
	// valid options are
	//		"steamid" - opens the overlay web browser to the specified user or groups profile
	//		"chat" - opens a chat window to the specified user, or joins the group chat 
	void ActivateGameOverlayToUser( const char *pchDialog, CSteamID steamID );

	// activates game overlay web browser directly to the specified URL
	// full address with protocol type is required, e.g. http://www.steamgames.com/
	void ActivateGameOverlayToWebPage( const char *pchURL );

	// activates game overlay to store page for app
	void ActivateGameOverlayToStore( AppId_t nAppID );

	// Mark a target user as 'played with'. This is a client-side only feature that requires that the calling user is 
	// in game 
	void SetPlayedWith( CSteamID steamIDUserPlayedWith );
};
