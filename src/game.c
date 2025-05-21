#include "project.h"

Player player;

void game_startup() {

	InitAudioDevice();

	player = (Player){ 2, 2 };

}

void game_update() {

    // player movement
    if (IsKeyPressed(KEY_RIGHT) && map[player.y][player.x + 1] != '#') player.x++;
    if (IsKeyPressed(KEY_LEFT) && map[player.y][player.x - 1] != '#') player.x--;
    if (IsKeyPressed(KEY_DOWN) && map[player.y + 1][player.x] != '#') player.y++;
    if (IsKeyPressed(KEY_UP) && map[player.y - 1][player.x] != '#') player.y--;


}

void game_draw() {
	BeginDrawing();
	ClearBackground(BLACK);

	Vector2 origin = { 0, 0 };
	draw_map(origin);
	draw_player(origin);

	EndDrawing();
}

void game_shutdown() {

	CloseAudioDevice();
}


void draw_player(Vector2 origin) {
    Vector2 pos = {
        origin.x + player.x * TILE_SIZE + 8,
        origin.y + player.y * TILE_SIZE + 6
    };
    DrawTextEx(
        GetFontDefault(),
        "@",
        pos,
        24,
        1,
        YELLOW);

}