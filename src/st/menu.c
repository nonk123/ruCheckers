#include "raylib.h"

#include "clock.h"
#include "lobby.h"
#include "state.h"

#define REFRESH_RATE (1.0)
static double refreshTimer = 0.0;

void stMenuUpdate() {
	exitOnEsc();

	const double now = elapsed();
	if (now - refreshTimer >= REFRESH_RATE) {
		refreshLobbiesList();
		refreshTimer = now;
	}

	if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE))
		requestJoinLobby();

	if (IsKeyPressed(KEY_C))
		requestCreateLobby();
}

void stMenuDraw() {
	const int fs = 40, pad = 5;

	ClearBackground(RAYWHITE);
	if (lobbyCount())
		DrawText("SPACE TO JOIN!!!", pad, pad, fs, GREEN);
	else
		DrawText("Loading lobbies...", pad, pad, fs, BLACK);
	DrawText("C to create a lobby", pad, pad + fs, fs, RED);
}
