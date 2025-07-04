#include "project.h"

Player player;
GameState game_state;
bool draw_fps = false;

Map* game_current_map = NULL;
Map world_map;

void game_startup() {

	InitAudioDevice();

    load_map_from_file(&world_map, "assets/world.map", "world");

    game_reset();
}

void game_reset() {

    // reset player xp
    player.xp = 0;

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

    switch (game_state) {
    case STATE_MAP:

        update_player(game_current_map, &player);

        update_view_data(game_current_map, &player, VIEW_WIDTH, VIEW_HEIGHT);

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
        
        draw_view_data(VIEW_TOTAL_WIDTH, VIEW_TOTAL_HEIGHT);
        draw_player(VIEW_WIDTH, VIEW_HEIGHT);

        draw_player_stats(&player);

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