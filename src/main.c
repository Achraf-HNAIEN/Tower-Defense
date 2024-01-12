#include <time.h>
#include <MLV/MLV_all.h>
#include "grid.h"
#include "monstre.h"
#include "graph.h"
#include "game.h"
#include "tower.h"

int main() {
    srand(time(NULL));
    MLV_create_window("Tower Defense Grid", "TD Grid", WIDTH * CELL_SIZE + 200, HEIGHT * CELL_SIZE);

    // Initialize game structure
    Game game = {
        .wave = 1,
        .mana = 150,
        .mana_max = 2000,
        .path = NULL,
        .pathSize = 0,
        .quit = 0,
        .nb_tower = 0,
        .level_mana = 0,
        .level_gemme_in_shop = 0,
        .want_to_place_tower = 0,
        .tower_count = 0,
        .last_wave_time = MLV_get_time(), // Add this to track the last wave trigger time
        .manual_trigger = 0, // Add this to handle manual wave triggering
    };

    const int frameDelay = 1000 / 60;
    MLV_change_frame_rate(60);

    generatePath(game.grid, &game.path, &game.pathSize);
    Monster *Monsters = initializeWave(game.wave, game.path, game.pathSize);
    int previousTime = MLV_get_time();

    while (!game.quit) {
        int currentTime = MLV_get_time();
        float deltaTime = (currentTime - previousTime) / 1000.0f;
        previousTime = currentTime;
        MLV_actualise_window();
       
        if ((currentTime - game.last_wave_time) >= WAVE_INTERVAL * 1000 || game.manual_trigger) {
            
            if (game.manual_trigger) {
                int bonus = (WAVE_INTERVAL - (currentTime - game.last_wave_time) / 1000.0f) * game.mana_max / 100;
                game.mana += bonus;
            }

            free(Monsters); 
            Monsters = initializeWave(++game.wave, game.path, game.pathSize);
            game.last_wave_time = currentTime;
            game.manual_trigger = 0; 
        }

        // Update monsters
        int monster_count = moveMonsters(Monsters, game.path, game.pathSize, deltaTime, &game);

        MLV_Event event;
        int mouse_x, mouse_y;
        MLV_Button_state state;
        MLV_Keyboard_button key;

        event = MLV_get_event(&key, NULL, NULL, NULL, NULL, &mouse_x, &mouse_y, NULL, &state);

        if (event == MLV_MOUSE_BUTTON && state == MLV_PRESSED) {
            printf("Mouse clicked at: x=%d, y=%d\n", mouse_x, mouse_y);
            if (is_click_inside(mouse_x, mouse_y, WIDTH * CELL_SIZE + 5, 162, 92, 50)) {
                printf("Tower placement button clicked.\n");
                game.want_to_place_tower = !game.want_to_place_tower;
            } else if (game.want_to_place_tower) {
                Point gridPosition = {mouse_x / CELL_SIZE, mouse_y / CELL_SIZE};
                if (isWithinBounds(gridPosition.x, gridPosition.y)) {
                    Gemme *newGem = NULL;
                    placeTower(&game, gridPosition, newGem);
                }
            } else if (event == MLV_KEY && state == MLV_PRESSED && key == MLV_KEYBOARD_SPACE) {
                game.manual_trigger = 1;
                game.last_wave_time = currentTime - WAVE_INTERVAL * 1000;
            }
        }

        drawAll(&game, Monsters, monster_count);

        // Frame rate control
        int frameTime = MLV_get_time() - currentTime;
        if (frameTime < frameDelay) {
            MLV_delay_according_to_frame_rate(frameDelay, frameTime);
        }
    }

    // Cleanup
    MLV_free_window();
    free(Monsters); 
    if (game.path != NULL) {
        free(game.path);
    }

    return 0;
}
