#include "project.h"

Combatant player_combatant;
CombatTurnState combat_turn = COMBAT_TURN_PLAYER;
char combat_log[64] = "A monster appears!";

Enemy combat_enemies[MAX_ENEMIES];
int enemy_count = 0;
int current_enemy_index = 0;

int overworld_player_x = 0;
int overworld_player_y = 0;
int total_combat_experience = 0;


void startup_combat(int current_world_pos_x, int current_world_pos_y, Enemy* enemy) {

	overworld_player_x = current_world_pos_x;
	overworld_player_y = current_world_pos_y;

	// reset player combatant values
	player_combatant = (Combatant){ 30, 30, 5 };

	// clear out existing combat_enemies
	for (int i = 0; i < MAX_COMBAT_ENEMIES; i++) {
		combat_enemies[i].alive = false;
	}

	total_combat_experience = 0;


	// setup the monsters
	enemy_count = GetRandomValue(1, MAX_COMBAT_ENEMIES);
	for (int i = 0; i < enemy_count; i++) {
		combat_enemies[i].alive = true;
		combat_enemies[i].attack = enemy->attack;
		combat_enemies[i].hp = enemy->hp;
		combat_enemies[i].max_hp = enemy->max_hp;
		combat_enemies[i].tile.data = enemy->tile.data;
		combat_enemies[i].tile.id = enemy->tile.id;
		combat_enemies[i].name = enemy->name;
		combat_enemies[i].xp = GetRandomValue(10, 50);
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
			} while (!combat_enemies[current_enemy_index].alive && current_enemy_index != start);
		}

		// cycle downwards through enemies list
		if (IsKeyPressed(KEY_RIGHT)) {
			int start = current_enemy_index;
			do {
				current_enemy_index = (current_enemy_index + 1) % enemy_count;
			} while (!combat_enemies[current_enemy_index].alive && current_enemy_index != start);
		}


		if (IsKeyPressed(KEY_A)) {

			Enemy* current_target = &combat_enemies[current_enemy_index];
			if (current_target->alive) {
				current_target->hp -= player_combatant.attack;
				snprintf(combat_log, sizeof(combat_log), "You hit %s for %d!", current_target->name, player_combatant.attack);
				if (current_target->hp <= 0) {
					current_target->alive = false;
					total_combat_experience += current_target->xp;
					snprintf(combat_log, sizeof(combat_log), "You defeated %s!", current_target->name);
				}
			}

			combat_turn = COMBAT_TURN_MONSTER;
		}

		if (IsKeyPressed(KEY_F)) {
			update_combat_log("You flee!");
			game_state = STATE_MAP;
			
			restore_player_to_overworld(overworld_player_x, overworld_player_y, total_combat_experience, false);
		}
		break;
	case COMBAT_TURN_MONSTER:

		for (int i = 0; i < enemy_count; i++) {
			if (combat_enemies[i].alive) {
				player_combatant.hp -= combat_enemies[i].attack;
				snprintf(combat_log, sizeof(combat_log), "%s hits for %d!", combat_enemies[i].name, combat_enemies[i].attack);
			}
		}

		// victory state
		bool all_enemies_dead = true;
		for (int i = 0; i < enemy_count; i++) {
			if (combat_enemies[i].alive) {
				all_enemies_dead = false;

				if (!combat_enemies[current_enemy_index].alive) {
					current_enemy_index = i;
				}

				break;
			}
		}

		if (all_enemies_dead) {
			snprintf(combat_log, sizeof(combat_log), "You defeated all the enemies!");

			game_state = STATE_MAP;
			restore_player_to_overworld(overworld_player_x, overworld_player_y, total_combat_experience, true);
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
		if (combat_enemies[i].alive) {
			Color c = (i == current_enemy_index) ? YELLOW : RAYWHITE;
			DrawText(TextFormat("%s HP: %d/%d", combat_enemies[i].name, combat_enemies[i].hp, combat_enemies[i].max_hp), 60, text_offset_y + i * 20, 20, c);
		}
	}

	text_offset_y += (i * 20) + 20;
	
	DrawText(combat_log, 60, text_offset_y, 20, LIGHTGRAY);

	text_offset_y += 30;

	if (combat_turn == COMBAT_TURN_PLAYER) {
		DrawText("[A] Attack  [F] Flee", 60, text_offset_y, 20, YELLOW);
	}
}
