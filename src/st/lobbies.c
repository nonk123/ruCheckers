#include "raylib.h"

#include "lobby.h"
#include "state.h"

void stLobbiesUpdate() {
	exitOnEsc();

	if (IsKeyPressed(KEY_C))
		requestCreateLobby();
}

void stLobbiesDraw() {
	const int fs = 40, pad = 5;

	ClearBackground(RAYWHITE);
	if (!lobbyCount())
		DrawText("Loading lobbies...", pad, pad, fs, BLACK);
	DrawText("C to create a lobby", pad, pad + fs, fs, RED);
}
