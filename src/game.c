#include "project.h"

Player player;
GameState game_state;
//int world_pos_x;
//int world_pos_y;
bool draw_fps = false;

Map* game_current_map = NULL;
Map world_map;

void game_startup() {

	InitAudioDevice();

    load_map_from_file(&world_map, "assets/world.map", "world");

    game_reset();
}

void game_reset() {

    //player = (Player){ 2, 2 };

    game_state = STATE_MAP;

    game_current_map = &world_map;
    spawn_player_to_map(game_current_map, &player);

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

   //char next_tile;

    switch (game_state) {
    case STATE_MAP:

        update_player(game_current_map, &player);

        //world_pos_x = player.x;
        //world_pos_y = player.y;

        // player movement
        /*if (IsKeyPressed(KEY_RIGHT) && map[player.y][player.x + 1] != '#') player.x++;
        if (IsKeyPressed(KEY_LEFT) && map[player.y][player.x - 1] != '#') player.x--;
        if (IsKeyPressed(KEY_DOWN) && map[player.y + 1][player.x] != '#') player.y++;
        if (IsKeyPressed(KEY_UP) && map[player.y - 1][player.x] != '#') player.y--;

        next_tile = map[player.y][player.x];
        if (next_tile == 'M') {
            game_state = STATE_COMBAT;

            startup_combat();
        }*/


        break;
    case STATE_COMBAT:

        update_combat();

        
        break;
    }

    

}

void game_draw() {
	BeginDrawing();
	ClearBackground(BLACK);

    Vector2 origin = { 0, 0 };

    switch (game_state) {
    case STATE_MAP:
        
        draw_map(game_current_map, origin);
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

char* generate_timestamp_filename() {
    time_t now;
    struct tm* timenow;
    static char filename[64];

    time(&now);
    timenow = localtime(&now);
    strftime(filename, sizeof(filename), "screenshot_%Y%m%d_%H%M%S.png", timenow);
    return filename;
}