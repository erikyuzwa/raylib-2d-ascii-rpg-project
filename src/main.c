#include "project.h"

int main(void) {
	InitWindow(
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		"Raylib 2D ASCII RPG");
	SetTargetFPS(60);
	srand(time(NULL));

	game_startup();

	while (!WindowShouldClose()) {
		// update our game
		game_update();

		// draw our game
		game_draw();
	}

	game_shutdown();

	CloseWindow();
	return 0;
}