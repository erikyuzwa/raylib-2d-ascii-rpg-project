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
    default: return RAYWHITE;
    }
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

    //for (int y = 0; y < MAP_HEIGHT; y++) {
    //    if (fgets(map->tiles[y], MAP_WIDTH + 1, file) == NULL) {
    //        break;
    //    }

    //    // Strip newline
    //    size_t len = strlen(map->tiles[y]);
    //    if (len > 0 && map->tiles[y][len - 1] == '\n') {
    //        map->tiles[y][len - 1] = '\0';
    //    }

    //    for (int x = 0; x < MAP_WIDTH; x++) {
    //        char ch = map->tiles[y][x];

    //        if (ch == '@') {
    //            map->player_spawn_x = x;
    //            map->player_spawn_y = y;
    //            map->tiles[y][x] = '.';
    //        }
    //    }

    //}

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

}

void draw_view_data(int view_width, int view_height) {

    for (int y = 0; y < view_height; y++) {
        for (int x = 0; x < view_width; x++) {
            Tile t = view_data[y][x];

            int screen_x = x * TILE_SIZE;
            int screen_y = y * TILE_SIZE;

            Color color = get_color_from_tile(t);

            /*Color color = LIGHTGRAY;
            if (t.id == -1) color = YELLOW;
            else if (t.data == '#') color = DARKGRAY;
            else if (t.data == '.') color = GREEN;
            else color = LIGHTGRAY;*/

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


/*
void draw_map(Map* map, Vector2 origin) {

    int rows = map->number_of_rows;
    int cols = map->number_of_cols;

    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < cols; x++) {
            char tile = map->tiles[y][x].data;
            Color tile_color = get_tile_color(tile);
            Vector2 pos = {
                origin.x + x * TILE_SIZE,
                origin.y + y * TILE_SIZE
            };

            DrawTextEx(
                GetFontDefault(),
                TextFormat("%c", tile),
                pos,
                TILE_SIZE,
                0,
                tile_color);
        }
    }

}
*/

