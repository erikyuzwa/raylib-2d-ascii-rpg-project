#ifndef __PROJECT_H_
#define __PROJECT_H_

// if we're building on WIN32 or WIN64 (aka. Windows)
#if defined(_WIN64) || defined(_WIN32)
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

#define TILE_SIZE 32

#define MAP_WIDTH 80
#define MAP_HEIGHT 50

#define VIEW_COLS 40
#define VIEW_ROWS 25

#define UI_COLS 10
#define SCREEN_WIDTH (VIEW_COLS + UI_COLS) * TILE_SIZE
#define SCREEN_HEIGHT (VIEW_ROWS * TILE_SIZE)

#define MAX_ENEMIES 3

typedef struct {
	int x, y;
} Player;

typedef struct {
	const char* name;
	int hp;
	int max_hp;
	int attack;
	bool alive;
} Enemy;

typedef enum {
	STATE_MAP,
	STATE_COMBAT
} GameState;

typedef struct {
	int hp;
	int max_hp;
	int attack;
} Combatant;

typedef enum {
	COMBAT_TURN_PLAYER,
	COMBAT_TURN_MONSTER
} CombatTurnState;

typedef struct {
	const char* name;
	int max_hp;
	int attack;
} MonsterTemplate;

typedef struct {
	char data;
	int id;
} Tile;

typedef struct {
	char name[32];
	//char tiles[MAP_HEIGHT][MAP_WIDTH];
	Tile tiles[MAP_HEIGHT][MAP_WIDTH];
	int player_spawn_x;
	int player_spawn_y;
	int number_of_cols;
	int number_of_rows;
} Map;

extern Player player;
extern GameState game_state;

// game.c
void game_startup();
void game_update();
void game_draw();
void game_shutdown();
void game_reset();
char* generate_timestamp_filename();

// map.c
Color get_tile_color(char tile);
void draw_map(Map* map, Vector2 origin);
void draw_player(Vector2 origin);
int load_map_from_file(Map* map, const char* filename, const char* name);
void spawn_player_to_map(Map* map, Player* player);

// combat.c
void update_combat_log(const char* text);
void draw_combat_screen();
void startup_combat();
void update_combat();

#endif

