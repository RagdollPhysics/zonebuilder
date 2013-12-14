// ==========================================================
// alterIWnet project
// 
// Component: aiw_client
// Sub-component: steam_api
// Purpose: Callbacks manager
//
// Initial author: NTAuthority
// Started: 2010-09-27
// ==========================================================

using namespace System;
using namespace System::Collections::Generic;

public ref class CallbackBase {
public:
	CCallbackBase* _callback;
	int _callbackType;
};

public ref class ResultBase {
public:
	void* _result;
	int _callbackType;
	SteamAPICall_t _id;
	int _size;
};

public ref class Callbacks {
public:
	static List<CallbackBase^>^ _callbacks;
	static List<ResultBase^>^ _results;
	static Dictionary<SteamAPICall_t, bool>^ _calls;
	static Dictionary<SteamAPICall_t, CallbackBase^>^ _callResults;
	static Random^ _rnd;

	static Callbacks() {
		_callbacks = gcnew List<CallbackBase^>();
		_results = gcnew List<ResultBase^>();
		_calls = gcnew Dictionary<SteamAPICall_t, bool>();
		_callResults = gcnew Dictionary<SteamAPICall_t, CallbackBase^>();
		_rnd = gcnew Random();
	}

	static void Run() {
		List<ResultBase^>^ resultdata = _results->GetRange(0, _results->Count); // thanks, stupid BCL!

		for each (ResultBase^ result in resultdata) {
			for each (CallbackBase^ call in _callbacks) {
				if (call->_callbackType == result->_callbackType) {
					try
					{
						call->_callback->Run(result->_result, false, 0);
					}
					catch (AccessViolationException^)
					{

					}
				}
			}

			for each (KeyValuePair<SteamAPICall_t, CallbackBase^>^ data in _callResults) {
				if (result->_id == data->Key) {
					//CallbackMsg_t retmsg;
					//retmsg.m_hSteamUser = 1;
					//retmsg.m_iCallback = data->Value->_callbackType;
					//retmsg.m_pubParam = (uint8*)result->_result;
					//retmsg.m_cubParam = result->_size;
					//data->Value->_callback->Run(&retmsg, false, result->_id);
					data->Value->_callback->Run(result->_result, false, result->_id);
				}
			}
		}

		_results->Clear();
	}

	static SteamAPICall_t RegisterCall() {
		SteamAPICall_t name = _rnd->Next();
		_calls->Add(name, false);

		return name;
	}

	static void Return(void* resultv, int type, SteamAPICall_t call, int size) {
		

		_calls[call] = true;

		ResultBase^ callback = gcnew ResultBase();
		callback->_result = resultv;
		callback->_callbackType = type;
		callback->_id = call;
		callback->_size = size;

		_results->Add(callback);
	}

	static void Register(CCallbackBase* callbackv, int type) {
		CallbackBase^ callback = gcnew CallbackBase();
		callback->_callback = callbackv;
		callback->_callbackType = type;

		_callbacks->Add(callback);
	}

	static void RegisterResult(CCallbackBase* callbackv, SteamAPICall_t type) {
		CallbackBase^ callback = gcnew CallbackBase();
		callback->_callback = callbackv;

		_callResults->Add(type, callback);
	}

	static void Unregister(CCallbackBase* callbackv) {
		List<CallbackBase^>^ remove = gcnew List<CallbackBase^>();

		for each (CallbackBase^ cb in _callbacks) {
			if (cb->_callback == callbackv) {
				remove->Add(cb);
			}
		}

		for each (CallbackBase^ cb in remove) {
			_callbacks->Remove(cb);
		}
	}

	static void ReturnLobbyCreatedCall(SteamAPICall_t call, int lobbyID) {
		LobbyCreated_t* retvals = (LobbyCreated_t*)malloc(sizeof(LobbyCreated_t));
		CSteamID id = CSteamID( lobbyID, 0x40000, k_EUniversePublic, k_EAccountTypeChat );

		retvals->m_eResult = k_EResultOK;
		retvals->m_ulSteamIDLobby = id.ConvertToUint64();

		Return(retvals, LobbyCreated_t::k_iCallback, call, sizeof(LobbyCreated_t));
	}

	static void ReturnLobbyEnteredCall(SteamAPICall_t call, int lobbyID, bool success) {
		CSteamID id = CSteamID( lobbyID, 0x40000, k_EUniversePublic, k_EAccountTypeChat );

		LobbyEnter_t* retvals = (LobbyEnter_t*)malloc(sizeof(LobbyEnter_t));
		retvals->m_bLocked = false;
		retvals->m_EChatRoomEnterResponse = (success) ? k_EChatRoomEnterResponseSuccess : k_EChatRoomEnterResponseDoesntExist;
		retvals->m_rgfChatPermissions = (EChatPermission)0xFFFFFFFF;
		retvals->m_ulSteamIDLobby = id.ConvertToUint64();

		Callbacks::Return(retvals, LobbyEnter_t::k_iCallback, call, sizeof(LobbyEnter_t));
	}
};