#include "project.h"

int main(void) {
	InitWindow(
		MAP_WIDTH * TILE_SIZE,
		MAP_HEIGHT * TILE_SIZE,
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