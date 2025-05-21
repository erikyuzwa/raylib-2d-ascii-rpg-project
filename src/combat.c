#include "project.h"

void draw_combat_screen() {
	DrawRectangle(40, 40, 400, 200, DARKGRAY);
	DrawRectangleLines(40, 40, 400, 200, WHITE);
	DrawText("Combat!", 60, 60, 24, RED);
	DrawText("You encountered a monster!", 60, 100, 20, RAYWHITE);
	DrawText("[A] Attack  [D] Defend  [F] Flee", 60, 140, 20, LIGHTGRAY);
}