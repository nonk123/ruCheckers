#include "raylib.h"

#include "lobby.h"

void stGameUpdate() {
	if (IsKeyPressed(KEY_ESCAPE))
		leaveLobby();
}

void stGameDraw() {
	ClearBackground(RAYWHITE);
	DrawText("GUCCI GANG", 5, 5, 50, GREEN);
}
