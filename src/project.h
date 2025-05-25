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
#define MAP_WIDTH 20
#define MAP_HEIGHT 15

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

extern Player player;
extern GameState game_state;
extern char map[MAP_HEIGHT][MAP_WIDTH]; 
extern int world_pos_x;
extern int world_pos_y;

// game.c
void game_startup();
void game_update();
void game_draw();
void game_shutdown();
void game_reset();
char* generate_timestamp_filename();

// map.c
Color get_tile_color(char tile);
void draw_map(Vector2 origin);
void draw_player(Vector2 origin);

// combat.c
void update_combat_log(const char* text);
void draw_combat_screen();
void startup_combat();
void update_combat();

#endif

