#include "lobby.h"
#include "__gen.h"
#include "state.h"

#include "caulk.h"

static bool lobbying = false;
static size_t lobbyCountFr = 0;
static CSteamID curLobby = 0;

size_t lobbyCount() {
	return lobbyCountFr;
}

static void onCreateLobby(void* tmp) {
	LobbyCreated_t* data = tmp;
	lobbying = data->m_eResult == k_EResultOK;
	if (!lobbying)
		return;

	curLobby = data->m_ulSteamIDLobby;
	ISteamMatchmaking* mm = caulk_SteamMatchmaking();
	caulk_ISteamMatchmaking_SetLobbyData(mm, curLobby, "ruCheckers", "fimoz");
}

static void onEnterLobby() {
	setState(ST_GAME);
}

static void resolveCreateLobby(void* data, bool failed) {
	lobbying = !failed;
}

void lobbyInit() {
	caulk_Register(LobbyCreated_t_iCallback, onCreateLobby);
	caulk_Register(LobbyEnter_t_iCallback, onEnterLobby);
	requestLobbies();
}

void resolveLobbies(void* data, bool failed) {
	if (!failed)
		lobbyCountFr = ((LobbyMatchList_t*)data)->m_nLobbiesMatching;
}

void requestLobbies() {
	ISteamMatchmaking* mm = caulk_SteamMatchmaking();
	caulk_ISteamMatchmaking_AddRequestLobbyListStringFilter(mm, "ruCheckers", "fimoz", k_ELobbyComparisonEqual);
	SteamAPICall_t cb = caulk_ISteamMatchmaking_RequestLobbyList(mm);
	caulk_Resolve(cb, resolveLobbies);
	lobbyCountFr = 0;
}

void requestCreateLobby() {
	if (lobbying)
		return;
	ISteamMatchmaking* mm = caulk_SteamMatchmaking();
	SteamAPICall_t cb = caulk_ISteamMatchmaking_CreateLobby(mm, k_ELobbyTypeFriendsOnly, 2);
	caulk_Resolve(cb, resolveCreateLobby);
	lobbying = true;
}

bool getLobbyId(size_t idx) {
	if (idx >= lobbyCount())
		return false;
	ISteamMatchmaking* mm = caulk_SteamMatchmaking();
	CSteamID id = caulk_ISteamMatchmaking_GetLobbyByIndex(mm, idx);
	return true;
}

void leaveLobby() {
	if (lobbying && curLobby) {
		ISteamMatchmaking* mm = caulk_SteamMatchmaking();
		caulk_ISteamMatchmaking_LeaveLobby(mm, curLobby);
	}

	lobbying = false;
	curLobby = 0;

	setState(ST_LOBBIES);
}
