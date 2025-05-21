#include "project.h"

char map[MAP_HEIGHT][MAP_WIDTH] = {
    "####################",
    "#..................#",
    "#..^^^^..~~~~~~....#",
    "#..^^^^..~~~~~~....#",
    "#.......######.....#",
    "#.......#..........#",
    "#.......#..........#",
    "#.....M............#",
    "#.....~~~~~........#",
    "#.....~~~~~........#",
    "#..........^^^^^...#",
    "#..........^^^^^...#",
    "#..................#",
    "#..................#",
    "####################"
};

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

void draw_map(Vector2 origin) {

    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            char tile = map[y][x];
            Color tile_color = get_tile_color(tile);
            Vector2 pos = {
                origin.x + x * TILE_SIZE + 8,
                origin.y + y * TILE_SIZE + 6
            };

            DrawTextEx(
                GetFontDefault(),
                TextFormat("%c", tile),
                pos,
                24,
                1,
                tile_color);
        }
    }

}

