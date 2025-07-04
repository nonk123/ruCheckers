#include "raylib.h"

#include "state.h"

void stGameUpdate() {
	exitOnEsc();
}

void stGameDraw() {
	ClearBackground(RAYWHITE);
	DrawText("GUCCI GANG", 5, 5, 50, GREEN);
}
