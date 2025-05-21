#include "project.h"

Combatant player_combatant;
Combatant monster_combatant;
CombatTurnState combat_turn = COMBAT_TURN_PLAYER;

char combat_log[64] = "A monster appears!";

void startup_combat() {

	player_combatant = (Combatant){ 20, 20, 5 };
	monster_combatant = (Combatant){ 10, 10, 3 };

	// reset combat values
	player_combatant.hp = player_combatant.max_hp;

	monster_combatant.hp = 10;
	monster_combatant.attack = GetRandomValue(3, 6);
	combat_turn = COMBAT_TURN_PLAYER;
	update_combat_log("A monster appears!");

}

void update_combat() {

	switch(combat_turn) {
	case COMBAT_TURN_PLAYER:
		if (IsKeyPressed(KEY_A)) {
			monster_combatant.hp -= player_combatant.attack;
			snprintf(combat_log, sizeof(combat_log), "You hit for %d!", player_combatant.attack);
			
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

		if (monster_combatant.hp > 0) {
			player_combatant.hp -= monster_combatant.attack;
			snprintf(combat_log, sizeof(combat_log), "Monster hits for %d!", monster_combatant.attack);
			combat_turn = COMBAT_TURN_PLAYER;
		}
		else {
			update_combat_log("You defeated the monster!");
			game_state = STATE_MAP;
			map[player.y][player.x] = '.';
		}

		break;
	}

	if (player_combatant.hp <= 0) {
		update_combat_log("You were defeated...");
		// exit? quit?
	}
	
}

void update_combat_log(const char* text) {
	snprintf(combat_log, sizeof(combat_log), text);
}

void draw_combat_screen() {
	DrawRectangle(40, 40, 400, 200, DARKGRAY);
	DrawRectangleLines(40, 40, 400, 200, WHITE);
	DrawText("Combat!", 60, 60, 24, RED);

	DrawText(TextFormat("Player HP: %d/%d", player_combatant.hp, player_combatant.max_hp),
		60, 100, 20, RAYWHITE);
	DrawText(TextFormat("Monster HP: %d/%d", monster_combatant.hp, monster_combatant.max_hp),
		60, 120, 20, RAYWHITE);

	DrawText(combat_log, 60, 150, 20, LIGHTGRAY);

	if (combat_turn == COMBAT_TURN_PLAYER) {
		DrawText("[A] Attack  [D] Defend  [F] Flee", 60, 200, 20, LIGHTGRAY);
	}
}
