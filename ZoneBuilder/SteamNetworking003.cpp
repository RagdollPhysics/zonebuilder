// ==========================================================
// IW4M project
// 
// Component: clientdll
// Sub-component: steam_api
// Purpose: ISteamNetworking003 implementation
//
// Initial author: NTAuthority
// Started: 2010-09-19
// ==========================================================

#include "StdInc.h"
#include "SteamNetworking003.h"

bool CSteamNetworking003::SendP2PPacket( CSteamID steamIDRemote, const void *pubData, uint32 cubData, EP2PSend eP2PSendType ) {
	return false;
}

bool CSteamNetworking003::IsP2PPacketAvailable( uint32 *pcubMsgSize ) {
	return false;
}

bool CSteamNetworking003::ReadP2PPacket( void *pubDest, uint32 cubDest, uint32 *pcubMsgSize, CSteamID *psteamIDRemote ) {
	return false;
}

bool CSteamNetworking003::AcceptP2PSessionWithUser( CSteamID steamIDRemote ) {
	return false;
}

bool CSteamNetworking003::CloseP2PSessionWithUser( CSteamID steamIDRemote ) {
	return false;
}

bool CSteamNetworking003::GetP2PSessionState( CSteamID steamIDRemote, P2PSessionState_t *pConnectionState ) {
	return false;
}

SNetListenSocket_t CSteamNetworking003::CreateListenSocket( int nVirtualP2PPort, uint32 nIP, uint16 nPort, bool bAllowUseOfPacketRelay ) {
	return NULL;
}

SNetSocket_t CSteamNetworking003::CreateP2PConnectionSocket( CSteamID steamIDTarget, int nVirtualPort, int nTimeoutSec, bool bAllowUseOfPacketRelay ) {
	return NULL;
}

SNetSocket_t CSteamNetworking003::CreateConnectionSocket( uint32 nIP, uint16 nPort, int nTimeoutSec ) {
	return NULL;
}

bool CSteamNetworking003::DestroySocket( SNetSocket_t hSocket, bool bNotifyRemoteEnd ) {
	return false;
}

bool CSteamNetworking003::DestroyListenSocket( SNetListenSocket_t hSocket, bool bNotifyRemoteEnd ) {
	return false;
}

bool CSteamNetworking003::SendDataOnSocket( SNetSocket_t hSocket, void *pubData, uint32 cubData, bool bReliable ) {
	return false;
}

bool CSteamNetworking003::IsDataAvailableOnSocket( SNetSocket_t hSocket, uint32 *pcubMsgSize ) {
	return false;
}

bool CSteamNetworking003::RetrieveDataFromSocket( SNetSocket_t hSocket, void *pubDest, uint32 cubDest, uint32 *pcubMsgSize ) {
	return false;
}

bool CSteamNetworking003::IsDataAvailable( SNetListenSocket_t hListenSocket, uint32 *pcubMsgSize, SNetSocket_t *phSocket ) {
	return false;
}

bool CSteamNetworking003::RetrieveData( SNetListenSocket_t hListenSocket, void *pubDest, uint32 cubDest, uint32 *pcubMsgSize, SNetSocket_t *phSocket ) {
	return false;
}

bool CSteamNetworking003::GetSocketInfo( SNetSocket_t hSocket, CSteamID *pSteamIDRemote, int *peSocketStatus, uint32 *punIPRemote, uint16 *punPortRemote ) {
	return false;
}

bool CSteamNetworking003::GetListenSocketInfo( SNetListenSocket_t hListenSocket, uint32 *pnIP, uint16 *pnPort ) {
	return false;
}

ESNetSocketConnectionType CSteamNetworking003::GetSocketConnectionType( SNetSocket_t hSocket ) {
	return k_ESNetSocketConnectionTypeNotConnected;
}

int CSteamNetworking003::GetMaxPacketSize( SNetSocket_t hSocket ) {
	return 0;
}