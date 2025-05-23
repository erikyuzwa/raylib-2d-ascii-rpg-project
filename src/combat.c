#include "project.h"

Combatant player_combatant;
CombatTurnState combat_turn = COMBAT_TURN_PLAYER;
char combat_log[64] = "A monster appears!";

Enemy enemies[MAX_ENEMIES];
int enemy_count = 0;
int current_enemy_index = 0;

MonsterTemplate monster_types[] = {
	{"Goblin", 8, 2},
	{"Orc", 12, 4},
	{"Slime", 6, 1},
	{"Troll", 16, 5},
	{"Wraith", 10, 3}
};

#define MONSTER_TYPE_COUNT (sizeof(monster_types) / sizeof(monster_types[0]))

void startup_combat() {

	// reset player combatant values
	player_combatant = (Combatant){ 30, 30, 5 };

	// setup the monsters
	enemy_count = GetRandomValue(1, MAX_ENEMIES);
	for (int i = 0; i < enemy_count; i++) {
		// pick a random monster
		int m = GetRandomValue(0, MONSTER_TYPE_COUNT - 1);
		MonsterTemplate mt = monster_types[m];
		enemies[i].name = mt.name;
		enemies[i].hp = mt.max_hp;
		enemies[i].max_hp = mt.max_hp;
		enemies[i].attack = mt.attack;
		enemies[i].alive = true;
	}

	current_enemy_index = 0;

	combat_turn = COMBAT_TURN_PLAYER;

	snprintf(combat_log, sizeof(combat_log), "You are ambushed by %d monsters!", enemy_count);

}

void update_combat() {

	switch(combat_turn) {
	case COMBAT_TURN_PLAYER:

		// cycle upwards through enemies list
		if (IsKeyPressed(KEY_LEFT)) {
			int start = current_enemy_index;
			do {
				current_enemy_index = (current_enemy_index - 1 + enemy_count) % enemy_count;
			} while (!enemies[current_enemy_index].alive && current_enemy_index != start);
		}

		// cycle downwards through enemies list
		if (IsKeyPressed(KEY_RIGHT)) {
			int start = current_enemy_index;
			do {
				current_enemy_index = (current_enemy_index + 1) % enemy_count;
			} while (!enemies[current_enemy_index].alive && current_enemy_index != start);
		}


		if (IsKeyPressed(KEY_A)) {

			Enemy* current_target = &enemies[current_enemy_index];
			if (current_target->alive) {
				current_target->hp -= player_combatant.attack;
				snprintf(combat_log, sizeof(combat_log), "You hit %s for %d!", current_target->name, player_combatant.attack);
				if (current_target->hp <= 0) {
					current_target->alive = false;
					snprintf(combat_log, sizeof(combat_log), "You defeated %s!", current_target->name);
				}
			}

			combat_turn = COMBAT_TURN_MONSTER;
		}

		if (IsKeyPressed(KEY_D)) {
			update_combat_log("You defend.");
			combat_turn = COMBAT_TURN_MONSTER;
		}

		if (IsKeyPressed(KEY_F)) {
			update_combat_log("You flee!");
			game_state = STATE_MAP;
			player.x = world_pos_x;
			player.y = world_pos_y;
		}
		break;
	case COMBAT_TURN_MONSTER:

		for (int i = 0; i < enemy_count; i++) {
			if (enemies[i].alive) {
				player_combatant.hp -= enemies[i].attack;
				snprintf(combat_log, sizeof(combat_log), "%s hits for %d!", enemies[i].name, enemies[i].attack);
			}
		}

		// victory state
		bool all_enemies_dead = true;
		for (int i = 0; i < enemy_count; i++) {
			if (enemies[i].alive) {
				all_enemies_dead = false;

				if (!enemies[current_enemy_index].alive) {
					current_enemy_index = i;
				}

				break;
			}
		}

		if (all_enemies_dead) {
			snprintf(combat_log, sizeof(combat_log), "You defeated all the enemies!");

			game_state = STATE_MAP;
			map[player.y][player.x] = '.';
			player_combatant.hp = player_combatant.max_hp;
		}

		if (player_combatant.hp <= 0) {
			update_combat_log("You were defeated...");
			// exit? quit?
		}

		combat_turn = COMBAT_TURN_PLAYER;

		break;
	}

}

void update_combat_log(const char* text) {
	snprintf(combat_log, sizeof(combat_log), text);
}

void draw_combat_screen() {
	DrawRectangle(40, 40, 400, 240, DARKGRAY);
	DrawRectangleLines(40, 40, 400, 240, WHITE);
	DrawText("Combat!", 60, 60, 24, RED);

	DrawText(TextFormat("Player HP: %d/%d", player_combatant.hp, player_combatant.max_hp),
		60, 100, 20, RAYWHITE);

	int text_offset_y = 140;
	int i = 0;
	for (i = 0; i < enemy_count; i++) {
		if (enemies[i].alive) {
			Color c = (i == current_enemy_index) ? YELLOW : RAYWHITE;
			DrawText(TextFormat("%s HP: %d/%d", enemies[i].name, enemies[i].hp, enemies[i].max_hp), 60, text_offset_y + i * 20, 20, c);
		}
	}

	text_offset_y += (i * 20) + 20;
	
	DrawText(combat_log, 60, text_offset_y, 20, LIGHTGRAY);

	text_offset_y += 30;

	if (combat_turn == COMBAT_TURN_PLAYER) {
		DrawText("[A] Attack  [D] Defend  [F] Flee", 60, text_offset_y, 20, YELLOW);
	}
}
