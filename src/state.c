#include "caulk.h"
#include "raylib.h"

#include "state.h"

static size_t curState = ST_MENU;
struct State {
	void (*update)(), (*draw)();
};

extern void stMenuUpdate(), stMenuDraw();
extern void stGameUpdate(), stGameDraw();
static const struct State states[] = {
    [ST_MENU] = {stMenuUpdate, stMenuDraw},
    [ST_GAME] = {stGameUpdate, stGameDraw},
};

void setState(size_t state) {
	curState = state;
}

static const char* panicText = NULL;
void panic(const char* text) {
	panicText = text;
}

extern bool steamInited;
void stateUpdate() {
	if (steamInited)
		caulk_Dispatch();
	if (panicText)
		exitOnEsc();
	else
		states[curState].update();
}

void stateDraw() {
	BeginDrawing();
	if (panicText)
		// TODO: bluescreen.
		ClearBackground(BLUE);
	else
		states[curState].draw();
	EndDrawing();
}
