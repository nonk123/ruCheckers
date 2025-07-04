#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "__gen.h"
#include "caulk.h"
#include "raylib.h"

enum {
	ST_LOBBIES,
	ST_GAME,
};

static bool death = false;
static const char* panic = NULL;

static bool steamInited = false;
static size_t lobbyCount = 0;

static size_t curState = ST_LOBBIES;
static void setState(int state) {
	curState = state;
}

struct State {
	void (*update)(), (*draw)();
};
static void exitOnEsc() {
	if (IsKeyPressed(KEY_ESCAPE))
		death = true;
}

static void resolveLobbies(void* data, bool failed) {
	if (!failed)
		lobbyCount = ((LobbyMatchList_t*)data)->m_nLobbiesMatching;
}

static void requestLobbies() {
	ISteamMatchmaking* mm = caulk_SteamMatchmaking();
	caulk_ISteamMatchmaking_AddRequestLobbyListStringFilter(mm, "ruCheckers", "fimoz", k_ELobbyComparisonEqual);
	SteamAPICall_t cb = caulk_ISteamMatchmaking_RequestLobbyList(mm);
	caulk_Resolve(cb, resolveLobbies);
	lobbyCount = 0;
}

static bool lobbying = false;
static CSteamID curLobby;

static void resolveCreateLobby(void* data, bool failed) {
	lobbying = !failed;
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

static void requestCreateLobby() {
	if (lobbying)
		return;
	ISteamMatchmaking* mm = caulk_SteamMatchmaking();
	SteamAPICall_t cb = caulk_ISteamMatchmaking_CreateLobby(mm, k_ELobbyTypeFriendsOnly, 2);
	caulk_Resolve(cb, resolveCreateLobby);
	lobbying = true;
}

static bool getLobbyId(size_t idx) {
	if (idx >= lobbyCount)
		return false;
	ISteamMatchmaking* mm = caulk_SteamMatchmaking();
	CSteamID id = caulk_ISteamMatchmaking_GetLobbyByIndex(mm, idx);
	return true;
}

static void lobbiesUpdate() {
	exitOnEsc();

	if (IsKeyPressed(KEY_C))
		requestCreateLobby();
}

static void lobbiesDraw() {
	const int fs = 40, pad = 5;

	ClearBackground(RAYWHITE);
	if (!lobbyCount)
		DrawText("Loading lobbies...", pad, pad, fs, BLACK);
	DrawText("C to create a lobby", pad, pad + fs, fs, RED);
}

static void gameUpdate() {
	exitOnEsc();
}

static void gameDraw() {
	ClearBackground(RAYWHITE);
	DrawText("GUCCI GANG", 5, 5, 50, GREEN);
}

static const struct State states[] = {
    [ST_LOBBIES] = {lobbiesUpdate, lobbiesDraw},
    [ST_GAME] = {gameUpdate, gameDraw},
};

static void panicUpdate() {
	exitOnEsc();
}

static void panicDraw() { // TODO: bluescreen.
	ClearBackground(BLUE);
}

int main(int argc, char* argv[]) {
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);

	InitWindow(800, 600, "Russian Checkers");
	SetWindowState(FLAG_WINDOW_MAXIMIZED);

	InitAudioDevice();

	SetExitKey(KEY_NULL);
	SetTargetFPS(60);

	if (caulk_Init()) {
		steamInited = true;
		caulk_Register(LobbyCreated_t_iCallback, onCreateLobby);
		caulk_Register(LobbyEnter_t_iCallback, onEnterLobby);
		requestLobbies();
	} else
		panic = "Failed to connect to Steam!";

	while (!death && !WindowShouldClose()) {
		if (steamInited)
			caulk_Dispatch();

		const struct State* state = &states[curState];
		if (panic)
			panicUpdate();
		else
			state->update();

		BeginDrawing();
		if (panic)
			panicDraw();
		else
			state->draw();
		EndDrawing();
	}

	if (steamInited)
		caulk_Shutdown();

	CloseAudioDevice();
	CloseWindow();

	return EXIT_SUCCESS;
}
