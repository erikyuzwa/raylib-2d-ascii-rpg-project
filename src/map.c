#include "project.h"

Tile view_data[VIEW_TOTAL_HEIGHT][VIEW_TOTAL_WIDTH];

Color get_color_from_tile(Tile tile) {
    switch (tile.data) {
    case '*': return YELLOW;
    case '#': return GRAY; // wall 
    case '.': return DARKGREEN; // grass
    case '^': return BROWN; // mountain
    case '~': return BLUE; // water
    case 'M': return GREEN; // orc
    case 'g': return PURPLE;
    case 'o': return DARKGREEN;
    case 's': return GREEN;
    case 't': return GREEN;
    case 'w': return DARKGRAY;
    default: return RAYWHITE;
    }
}

void spawn_monster_to_map(Map* map, int x, int y) {

    // pick a random monster
    int m = GetRandomValue(0, MONSTER_TYPE_COUNT - 1);
    MonsterTemplate mt = monster_types[m];

    int number_of_enemies = map->number_of_enemies;
    map->enemies[number_of_enemies].name = mt.name;
    map->enemies[number_of_enemies].hp = mt.max_hp;
    map->enemies[number_of_enemies].max_hp = mt.max_hp;
    map->enemies[number_of_enemies].attack = mt.attack;
    map->enemies[number_of_enemies].alive = true;
    map->enemies[number_of_enemies].x = x;
    map->enemies[number_of_enemies].y = y;
    map->enemies[number_of_enemies].tile.data = mt.symbol;

    number_of_enemies++;
    map->number_of_enemies = number_of_enemies;
}

int load_map_from_file(Map* map, const char* filename, const char* name) {

    FILE* file = fopen(filename, "r");
    if (!file) {
        TraceLog(LOG_ERROR, "Failed to load map file: %s", filename);
        return 0;
    }

    strncpy(map->name, name, sizeof(map->name) - 1);
    map->name[sizeof(map->name) - 1] = '\0';
    map->player_spawn_x = 0;
    map->player_spawn_y = 0;
    map->number_of_cols = 0;
    map->number_of_rows = 0;
    map->number_of_enemies = 0;

    char line[MAP_WIDTH + 2]; // the +2 in length is to account for a newline and/or null terminator
    int row = 0;

    while (fgets(line, sizeof(line), file)) {
        int col = 0;
        while (line[col] != '\n' && line[col] != '\0') {
            if (line[col] == '@') {
                map->tiles[row][col].data = '.';
                map->tiles[row][col].id = 1;
                map->player_spawn_x = col;
                map->player_spawn_y = row;
            }
            else if (line[col] == 'M') {
                map->tiles[row][col].data = '.';
                map->tiles[row][col].id = 1;

                // spawn enemy
                spawn_monster_to_map(map, col, row);
            }
            else {
                map->tiles[row][col].data = line[col];
                map->tiles[row][col].id = 0;
            }

            col++;
        }

        if (col > map->number_of_cols) map->number_of_cols = col;
        row++;
        if (row >= MAP_WIDTH) break;
    }

    map->number_of_rows = row;

    fclose(file);

    return 1;
}

void spawn_player_to_map(Map* current_map, Player* player) {
    if (current_map) {
        player->x = current_map->player_spawn_x;
        player->y = current_map->player_spawn_y;
    }
}

void update_view_data(Map* map, Player* player, int view_width, int view_height) {

    int start_x = player->x - (view_width / 2);
    int start_y = player->y - (view_height / 2);
    int map_cols = map->number_of_cols;
    int map_rows = map->number_of_rows;

    for (int y = 0; y < VIEW_TOTAL_HEIGHT; y++) {
        for (int x = 0; x < VIEW_TOTAL_WIDTH; x++) {

            // fill border with special tile '*'
            if (x == 0 || y == 0 || x == VIEW_TOTAL_WIDTH - 1 || y == VIEW_TOTAL_HEIGHT - 1) {
                view_data[y][x].data = '*';
                view_data[y][x].id = -1;
            }
            else {
                int map_x = start_x + (x - 1);
                int map_y = start_y + (y - 1);

                if (map_x >= 0 && map_x < map_cols && map_y >= 0 && map_y < map_rows) {
                    view_data[y][x] = map->tiles[map_y][map_x];
                }
                else {
                    view_data[y][x].data = ' ';
                    view_data[y][x].id = 0;
                }
            }
        }
    }

    int enemy_count = map->number_of_enemies;
    for (int i = 0; i < enemy_count; i++) {
        if (!map->enemies[i].alive) continue;

        int m_x = map->enemies[i].x;
        int m_y = map->enemies[i].y;

        // only update if the monster is inside the current view
        if (m_x >= start_x && m_x < start_x + VIEW_TOTAL_WIDTH &&
            m_y >= start_y && m_y < start_y + VIEW_TOTAL_HEIGHT) {

            int view_x = (m_x - start_x) + 1; // +1 for the border
            int view_y = (m_y - start_y) + 1;

            view_data[view_y][view_x].data = map->enemies[i].tile.data;
            view_data[view_y][view_x].id = map->enemies[i].tile.id;
        }
    }

}

void draw_view_data(int view_width, int view_height) {

    for (int y = 0; y < view_height; y++) {
        for (int x = 0; x < view_width; x++) {
            Tile t = view_data[y][x];

            int screen_x = x * TILE_SIZE;
            int screen_y = y * TILE_SIZE;

            Color color = get_color_from_tile(t);

            Vector2 pos = {
                screen_x, screen_y
            };

            DrawTextEx(
                GetFontDefault(),
                TextFormat("%c", t.data),
                pos,
                TILE_SIZE,
                0,
                color);
        }
    }
}

void restore_player_to_overworld(int overworld_player_x, int overworld_player_y, int total_combat_experience, bool is_victorious) {
    if (!game_current_map) return;

    if (is_victorious) {
        int number_of_enemies = game_current_map->number_of_enemies;
        for (int i = 0; i < number_of_enemies; i++) {
            if (game_current_map->enemies[i].x == overworld_player_x && game_current_map->enemies[i].y == overworld_player_y) {
                game_current_map->enemies[i].alive = false;
            }
        }

        player.xp += total_combat_experience;
        player.x = overworld_player_x;
        player.y = overworld_player_y;
    }
    else {

        int new_x = overworld_player_x += (GetRandomValue(-1, 1));
        int new_y = overworld_player_y += (GetRandomValue(-1, 1));
        player.x = new_x;
        player.y = new_y;
       
    }

}

