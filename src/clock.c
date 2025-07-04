#include "clock.h"
#include "raylib.h"

static double start = 0.0;

void clockInit() {
	start = GetTime();
}

double elapsed() {
	return GetTime() - start;
}
