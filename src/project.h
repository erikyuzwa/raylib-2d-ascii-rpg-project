#ifndef __PROJECT_H_
#define __PROJECT_H_

#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

#define TILE_SIZE 32
#define MAP_WIDTH 20
#define MAP_HEIGHT 15

typedef struct {
	int x, y;
} Player;

typedef enum {
	STATE_MAP,
	STATE_COMBAT
} GameState;

extern GameState game_state;
extern char map[MAP_HEIGHT][MAP_WIDTH];

// game.c
void game_startup();
void game_update();
void game_draw();
void game_shutdown();

// map.c
Color get_tile_color(char tile);
void draw_map(Vector2 origin);
void draw_player(Vector2 origin);

// combat.c
void draw_combat_screen();

#endif

