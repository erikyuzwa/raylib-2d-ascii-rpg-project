#include "project.h"

Player player;
GameState game_state;
int world_pos_x;
int world_pos_y;
bool draw_fps = false;

void game_startup() {

	InitAudioDevice();

    game_reset();
}

void game_reset() {

    player = (Player){ 2, 2 };

    game_state = STATE_MAP;

}

void game_update() {

    if (IsKeyPressed(KEY_F1)) {
        draw_fps = !draw_fps;
    }
    else if (IsKeyPressed(KEY_F2)) {
        game_reset();
    }
    else if (IsKeyPressed(KEY_F4)) {
        TakeScreenshot(generate_timestamp_filename());
    }

    char next_tile;

    switch (game_state) {
    case STATE_MAP:

        world_pos_x = player.x;
        world_pos_y = player.y;

        // player movement
        if (IsKeyPressed(KEY_RIGHT) && map[player.y][player.x + 1] != '#') player.x++;
        if (IsKeyPressed(KEY_LEFT) && map[player.y][player.x - 1] != '#') player.x--;
        if (IsKeyPressed(KEY_DOWN) && map[player.y + 1][player.x] != '#') player.y++;
        if (IsKeyPressed(KEY_UP) && map[player.y - 1][player.x] != '#') player.y--;

        next_tile = map[player.y][player.x];
        if (next_tile == 'M') {
            game_state = STATE_COMBAT;

            startup_combat();
        }


        break;
    case STATE_COMBAT:

        update_combat();

        
        break;
    }

    

}

void game_draw() {
	BeginDrawing();
	ClearBackground(BLACK);

    switch (game_state) {
    case STATE_MAP:
        Vector2 origin = { 0, 0 };
        draw_map(origin);
        draw_player(origin);
        break;
    case STATE_COMBAT:
        draw_combat_screen();
        break;
    }

    if (draw_fps) {
        float fps = GetFPS();
        DrawText(TextFormat("FPS: %.2f", fps), 10, 10, 20, YELLOW);
    }

	EndDrawing();
}

void game_shutdown() {

	CloseAudioDevice();
}


void draw_player(Vector2 origin) {
    Vector2 pos = {
        origin.x + player.x * TILE_SIZE + 8,
        origin.y + player.y * TILE_SIZE + 6
    };

    DrawTextEx(
        GetFontDefault(),
        "@",
        pos,
        24,
        1,
        YELLOW);

}

char* generate_timestamp_filename() {
    time_t now;
    struct tm* timenow;
    static char filename[64];

    time(&now);
    timenow = localtime(&now);
    strftime(filename, sizeof(filename), "screenshot_%Y%m%d_%H%M%S.png", timenow);
    return filename;
}