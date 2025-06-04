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

    int new_x = player->x;
    int new_y = player->y;

    if (IsKeyPressed(KEY_RIGHT)) new_x++;
    if (IsKeyPressed(KEY_LEFT)) new_x--;
    if (IsKeyPressed(KEY_DOWN)) new_y++;
    if (IsKeyPressed(KEY_UP)) new_y--;

    if (current_map->tiles[new_y][new_x].data != '#') {
        player->x = new_x;
        player->y = new_y;
    }


    /*
    * TODO: fix me!
    if (next_tile == 'M') {
        game_state = STATE_COMBAT;

        startup_combat();
    }*/
}