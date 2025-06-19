#include "project.h"

void draw_player(int view_width, int view_height) {
    Vector2 pos = {
        (1 + view_width / 2) * TILE_SIZE,
        (1 + view_height / 2) * TILE_SIZE
    };

    DrawTextEx(
        GetFontDefault(),
        "@",
        pos,
        TILE_SIZE,
        0,
        YELLOW);

}

void draw_player_stats(Player* player) {
    Vector2 pos = {
        VIEW_TOTAL_WIDTH * TILE_SIZE,
        TILE_SIZE
    };

    DrawTextEx(
        GetFontDefault(),
        TextFormat("XP: %d", player->xp),
        pos,
        TILE_SIZE,
        1.5f,
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

    int enemy_count = current_map->number_of_enemies;
    for (int i = 0; i < enemy_count; i++) {
        if (!current_map->enemies[i].alive) continue;

        int m_x = current_map->enemies[i].x;
        int m_y = current_map->enemies[i].y;

        if (player->x == m_x && player->y == m_y) {
            game_state = STATE_COMBAT;
            startup_combat(player->x, player->y, &current_map->enemies[i]);
            break;
        }
    }

    
}

