#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "caulk.h"
#include "raylib.h"

static bool death = false;
static const char* panic = NULL;

enum {
	ST_INIT,
	ST_LOBBIES,
};

struct State {
	void (*update)(), (*draw)();
};
static void exitOnEsc() {
	if (IsKeyPressed(KEY_ESCAPE))
		death = true;
}

static size_t curState = ST_INIT;
static void setState(int state) {
	curState = state;
}

static bool steamInited = false;
static void initUpdate() {
	exitOnEsc();

	if (caulk_Init()) {
		steamInited = true;
		setState(ST_LOBBIES);
	} else
		panic = "Failed to connect to Steam!";
}

static void initDraw() {
	ClearBackground(GREEN);
}

static void lobbiesUpdate() {
	exitOnEsc();
}

static void lobbiesDraw() {
	ClearBackground(RAYWHITE);
}

static const struct State states[] = {
    [ST_INIT] = {initUpdate, initDraw},
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

	while (!death && !WindowShouldClose()) {
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
