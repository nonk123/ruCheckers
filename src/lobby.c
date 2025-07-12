#include "lobby.h"
#include "__gen.h"
#include "state.h"

#include "caulk.h"

static bool inLobby = false;
static size_t lobbyCountFr = 0;
static CSteamID curLobby = 0;

size_t lobbyCount() {
	return lobbyCountFr;
}

static void onCreateLobby(void* pData) {
	LobbyCreated_t* data = pData;
	inLobby = data->m_eResult == k_EResultOK;
	curLobby = data->m_ulSteamIDLobby;

	if (!inLobby || !curLobby) {
		leaveLobby();
		return;
	}

	ISteamMatchmaking* mm = caulk_SteamMatchmaking();
	caulk_ISteamMatchmaking_SetLobbyData(mm, curLobby, "ruCheckers", "fimoz");
}

static void onEnterLobby() {
	setState(ST_GAME);
}

void lobbyInit() {
	caulk_Register(LobbyCreated_t_iCallback, onCreateLobby);
	caulk_Register(LobbyEnter_t_iCallback, onEnterLobby);
}

void resolveLobbies(void* data, bool failed) {
	if (!failed)
		lobbyCountFr = ((LobbyMatchList_t*)data)->m_nLobbiesMatching;
}

void refreshLobbiesList() {
	ISteamMatchmaking* mm = caulk_SteamMatchmaking();
	caulk_ISteamMatchmaking_AddRequestLobbyListStringFilter(mm, "ruCheckers", "fimoz", k_ELobbyComparisonEqual);
	SteamAPICall_t cb = caulk_ISteamMatchmaking_RequestLobbyList(mm);
	caulk_Resolve(cb, resolveLobbies);
	lobbyCountFr = 0;
}

void requestCreateLobby() {
	if (inLobby)
		return;
	ISteamMatchmaking* mm = caulk_SteamMatchmaking();
	caulk_ISteamMatchmaking_CreateLobby(mm, k_ELobbyTypeFriendsOnly, 2);
	inLobby = true;
}

void requestJoinLobby() {
	CSteamID id;
	for (size_t idx = 0; idx < lobbyCount(); idx++)
		if (getLobbyId(idx, &id)) {
			ISteamMatchmaking* mm = caulk_SteamMatchmaking();
			caulk_ISteamMatchmaking_JoinLobby(mm, id);
			return;
		}
}

bool getLobbyId(size_t idx, CSteamID* out) {
	if (inLobby || idx >= lobbyCount())
		return false;
	ISteamMatchmaking* mm = caulk_SteamMatchmaking();
	*out = caulk_ISteamMatchmaking_GetLobbyByIndex(mm, idx);
	return true;
}

void leaveLobby() {
	if (inLobby && curLobby) {
		ISteamMatchmaking* mm = caulk_SteamMatchmaking();
		caulk_ISteamMatchmaking_LeaveLobby(mm, curLobby);
	}

	inLobby = false;
	curLobby = 0;

	setState(ST_MENU);
}
