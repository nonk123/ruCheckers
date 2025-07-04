#pragma once

#include <stddef.h>

enum {
	ST_MENU,
	ST_GAME,
};

void setState(size_t);
void stateUpdate(), stateDraw();

extern void exitOnEsc();
void panic(const char*);
