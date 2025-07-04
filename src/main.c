#include <stdint.h>
#include <stdlib.h>

#include "caulk.h"
#include "raylib.h"

#include "lobby.h"
#include "state.h"

static bool death = false;
bool steamInited = false;

void exitOnEsc() {
	if (IsKeyPressed(KEY_ESCAPE))
		death = true;
}

int main(int argc, char* argv[]) {
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);

	InitWindow(800, 600, "Russian Checkers");
	SetWindowState(FLAG_WINDOW_MAXIMIZED);

	InitAudioDevice();

	SetExitKey(KEY_NULL);
	SetTargetFPS(60);

	if (!caulk_Init())
		panic("Failed to connect to Steam!");
	else {
		steamInited = true;
		lobbyInit();
	}

	while (!death && !WindowShouldClose()) {
		stateUpdate();
		stateDraw();
	}

	leaveLobby();

	if (steamInited)
		caulk_Shutdown();

	CloseAudioDevice();
	CloseWindow();

	return EXIT_SUCCESS;
}
