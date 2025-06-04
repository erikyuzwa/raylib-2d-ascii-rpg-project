#include "project.h"

Color get_tile_color(char tile) {
    switch (tile) {
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


