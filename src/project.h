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

#define VIEW_WIDTH 15
#define VIEW_HEIGHT 11
#define VIEW_TOTAL_WIDTH (VIEW_WIDTH + 2)
#define VIEW_TOTAL_HEIGHT (VIEW_HEIGHT + 2)

#define UI_COLS 10
#define SCREEN_WIDTH (VIEW_WIDTH + UI_COLS) * TILE_SIZE
#define SCREEN_HEIGHT (VIEW_TOTAL_HEIGHT * TILE_SIZE)

#define MAX_ENEMIES 10
#define MAX_COMBAT_ENEMIES 3

typedef struct {
	char data;
	int id;
} Tile;


typedef struct {
	int x;
	int y;
	int xp;
} Player;

typedef struct {
	const char* name;
	int hp;
	int max_hp;
	int attack;
	bool alive;
	int xp;
	int x;
	int y;
	Tile tile;
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
	char name[32];
	Tile tiles[MAP_HEIGHT][MAP_WIDTH];
	Enemy enemies[MAX_ENEMIES];
	int player_spawn_x;
	int player_spawn_y;
	int number_of_cols;
	int number_of_rows;
	int number_of_enemies;
} Map;

typedef struct {
	const char* name;
	int max_hp;
	int attack;
	char symbol;
} MonsterTemplate;

static MonsterTemplate monster_types[] = {
	{"Goblin", 8, 2, 'g'},
	{"Orc", 12, 4, 'o'},
	{"Slime", 6, 1, 's'},
	{"Troll", 16, 5, 't'},
	{"Wraith", 10, 3, 'w'}
};

#define MONSTER_TYPE_COUNT (sizeof(monster_types) / sizeof(monster_types[0]))


extern Player player;
extern GameState game_state;
extern Map* game_current_map;

// game.c
void game_startup();
void game_update();
void game_draw();
void game_shutdown();
void game_reset();
char* generate_timestamp_filename();

// map.c
Color get_color_from_tile(Tile tile);
void draw_map(Map* map, Vector2 origin);
void draw_player(Vector2 origin);
int load_map_from_file(Map* map, const char* filename, const char* name);
void spawn_player_to_map(Map* map, Player* player);
void spawn_monster_to_map(Map* map, int x, int y);
void draw_view_data(int view_width, int view_height);
void update_view_data(Map* map, Player* player, int view_width, int view_height);
void restore_player_to_overworld(int overworld_player_x, int overworld_player_y, int total_combat_experience, bool is_victorious);

// combat.c
void update_combat_log(const char* text);
void draw_combat_screen();
void startup_combat(int current_world_pos_x, int current_world_pos_y, Enemy* enemy);
void update_combat();

// player.c
void draw_player(int view_width, int view_height);
void update_player(Map* current_map, Player* player);
void draw_player_stats(Player* player);

#endif

