// ==========================================================
// IW4M project
// 
// Component: clientdll
// Sub-component: steam_api
// Purpose: ISteamMasterServerUpdater001 implementation
//
// Initial author: NTAuthority
// Started: 2010-09-28
// ==========================================================

#include "StdInc.h"
#include "SteamMasterServerUpdater001.h"

void CSteamMasterServerUpdater001::SetActive( bool bActive ) { }
void CSteamMasterServerUpdater001::SetHeartbeatInterval( int iHeartbeatInterval ) { }
bool CSteamMasterServerUpdater001::HandleIncomingPacket( const void *pData, int cbData, uint32 srcIP, uint16 srcPort ) { return true; }
int CSteamMasterServerUpdater001::GetNextOutgoingPacket( void *pOut, int cbMaxOut, uint32 *pNetAdr, uint16 *pPort ) { return 0; }
void CSteamMasterServerUpdater001::SetBasicServerData(unsigned short nProtocolVersion, bool bDedicatedServer,	const char *pRegionName, const char *pProductName, unsigned short nMaxReportedClients, bool bPasswordProtected,	const char *pGameDescription ) { }
void CSteamMasterServerUpdater001::ClearAllKeyValues() { }
void CSteamMasterServerUpdater001::SetKeyValue( const char *pKey, const char *pValue ) {
}

void CSteamMasterServerUpdater001::NotifyShutdown() { }
bool CSteamMasterServerUpdater001::WasRestartRequested() { return false; }
void CSteamMasterServerUpdater001::ForceHeartbeat() { }
bool CSteamMasterServerUpdater001::AddMasterServer( const char *pServerAddress ) { return true; }
bool CSteamMasterServerUpdater001::RemoveMasterServer( const char *pServerAddress ) { return true; }
int CSteamMasterServerUpdater001::GetNumMasterServers() { return 0; }
int CSteamMasterServerUpdater001::GetMasterServerAddress( int iServer, char *pOut, int outBufferSize ) { return 0; }