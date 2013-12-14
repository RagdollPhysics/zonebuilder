// ==========================================================
// IW4M project
// 
// Component: clientdll
// Sub-component: steam_api
// Purpose: ISteamUtils005 implementation
//
// Initial author: NTAuthority
// Started: 2010-09-10
// ==========================================================

#include "StdInc.h"
#include "SteamUtils005.h"

uint32 CSteamUtils005::GetSecondsSinceAppActive() {
	return 0;
}

uint32 CSteamUtils005::GetSecondsSinceComputerActive() {
	return 0;
}

EUniverse CSteamUtils005::GetConnectedUniverse() {
	return k_EUniversePublic;
}

uint32 CSteamUtils005::GetServerRealTime() {
	return 0;
}

const char* CSteamUtils005::GetIPCountry() {
	return "US";
}

bool CSteamUtils005::GetImageSize( int iImage, uint32 *pnWidth, uint32 *pnHeight ) {
	return false;
}

bool CSteamUtils005::GetImageRGBA( int iImage, uint8 *pubDest, int nDestBufferSize ) {
	return false;
}

bool CSteamUtils005::GetCSERIPPort( uint32 *unIP, uint16 *usPort ) {
	return false;
}

uint8 CSteamUtils005::GetCurrentBatteryPower() {
	return 255;
}

uint32 CSteamUtils005::GetAppID() {
	return 440;
}

void CSteamUtils005::SetOverlayNotificationPosition( ENotificationPosition eNotificationPosition ) {
}

bool CSteamUtils005::IsAPICallCompleted( SteamAPICall_t hSteamAPICall, bool *pbFailed ) {
	//return (Callbacks::_calls->ContainsKey(hSteamAPICall)) ? Callbacks::_calls[hSteamAPICall] : false;
	return false;
}

ESteamAPICallFailure CSteamUtils005::GetAPICallFailureReason( SteamAPICall_t hSteamAPICall ) {
	return k_ESteamAPICallFailureNone;
}

bool CSteamUtils005::GetAPICallResult( SteamAPICall_t hSteamAPICall, void *pCallback, int cubCallback, int iCallbackExpected, bool *pbFailed ) {
	return false;
}

void CSteamUtils005::RunFrame() {

}

uint32 CSteamUtils005::GetIPCCallCount() {
	return 0;
}

void CSteamUtils005::SetWarningMessageHook( SteamAPIWarningMessageHook_t pFunction ) {
}

bool CSteamUtils005::IsOverlayEnabled() { return false; }

bool CSteamUtils005::BOverlayNeedsPresent() { return false; }
