#include "project.h"

void draw_player(Vector2 origin) {
    Vector2 pos = {
        origin.x + player.x * TILE_SIZE,
        origin.y + player.y * TILE_SIZE
    };

    DrawTextEx(
        GetFontDefault(),
        "@",
        pos,
        TILE_SIZE,
        0,
        YELLOW);

}

void update_player(Map* current_map, Player* player) {
    if (!current_map) return; // make sure a map is loaded

    int newX = player->x;
    int newY = player->y;

    if (IsKeyPressed(KEY_RIGHT)) newX++;
    if (IsKeyPressed(KEY_LEFT)) newX--;
    if (IsKeyPressed(KEY_DOWN)) newY++;
    if (IsKeyPressed(KEY_UP)) newY--;

    if (current_map->tiles[newY][newX] != '#') {
        player->x = newX;
        player->y = newY;
    }
}