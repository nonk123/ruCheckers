#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "__gen.h"
#include "caulk.h"
#include "raylib.h"

static bool death = false;
static const char* panic = NULL;

static bool steamInited = false;
static size_t lobbyCount = 0;

enum {
	ST_LOBBIES,
};

struct State {
	void (*update)(), (*draw)();
};
static void exitOnEsc() {
	if (IsKeyPressed(KEY_ESCAPE))
		death = true;
}

static void fillLobbyList(void* data) {
	lobbyCount = ((LobbyMatchList_t*)data)->m_nLobbiesMatching;
	printf("\n\nbish %lu\n\n", lobbyCount);
}

static void requestLobbies() {
	ISteamMatchmaking* mm = caulk_SteamMatchmaking();
	caulk_ISteamMatchmaking_AddRequestLobbyListStringFilter(mm, "ruCheckers", "fimoz", k_ELobbyComparisonEqual);
	SteamAPICall_t cb = caulk_ISteamMatchmaking_RequestLobbyList(mm);
	caulk_Gucci(cb, fillLobbyList);
	lobbyCount = 0;
}

static bool getLobbyId(size_t idx) {
	if (idx >= lobbyCount)
		return false;
	ISteamMatchmaking* mm = caulk_SteamMatchmaking();
	CSteamID id = caulk_ISteamMatchmaking_GetLobbyByIndex(mm, idx);
	return true;
}

static size_t curState = ST_LOBBIES;
static void setState(int state) {
	curState = state;
}

static void lobbiesUpdate() {
	exitOnEsc();
}

static void lobbiesDraw() {
	const int fs = 40, pad = 5;

	ClearBackground(RAYWHITE);
	if (!lobbyCount)
		DrawText("Loading lobbies...", pad, pad, fs, BLACK);
	DrawText("C to create a lobby", pad, pad + fs, fs, RED);
}

static const struct State states[] = {
    [ST_LOBBIES] = {lobbiesUpdate, lobbiesDraw},
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
