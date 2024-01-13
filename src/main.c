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
        .last_wave_time = MLV_get_time(),
        .manual_trigger = 0,
        .max_active_waves = 6,
        .num_active_waves = 0,
        .active_waves = (Wave **)malloc(game.max_active_waves * sizeof(Wave *)),
    };

    // Ensure all wave pointers are initialized to NULL
    for (int i = 0; i < game.max_active_waves; i++) {
        game.active_waves[i] = NULL;
    }

    const int frameDelay = 1000 / 60;
    MLV_change_frame_rate(60);

    generatePath(game.grid, &game.path, &game.pathSize);

    // Initialize the first wave
    Wave *firstWave = initializeWave(game.wave, game.path, game.pathSize);
    game.active_waves[0] = firstWave;
    game.num_active_waves = 1;

    int previousTime = MLV_get_time();

    while (!game.quit) {
        int currentTime = MLV_get_time();
        float deltaTime = (currentTime - previousTime) / 1000.0f;
        previousTime = currentTime;

        // Check for wave creation and update
        if ((currentTime - game.last_wave_time) >= WAVE_INTERVAL * 1000 || game.manual_trigger) {
            if (game.manual_trigger) {
                // Bonus mana for manual trigger
                int bonus = (WAVE_INTERVAL - (currentTime - game.last_wave_time) / 1000.0f) * game.mana_max / 100;
                game.mana += bonus;
            }

            // Only add a new wave if we haven't hit the max
            if (game.num_active_waves < game.max_active_waves) {
                Wave *newWave = initializeWave(++game.wave, game.path, game.pathSize);
                game.active_waves[game.num_active_waves++] = newWave;
            }
            game.last_wave_time = currentTime;
            game.manual_trigger = 0;
        }

        // Update all active waves
        for (int i = 0; i < game.num_active_waves; i++) {
            if (game.active_waves[i] != NULL) {
                updateWave(game.active_waves[i], deltaTime, &game);
            }
        }

        // Handle MLV events
        MLV_Event event = MLV_get_event(
            NULL, NULL, NULL,
            NULL, NULL,
            NULL, NULL, NULL,
            NULL
        );

        if (event == MLV_MOUSE_BUTTON)        {
            int mouse_x, mouse_y;
            MLV_get_mouse_position(&mouse_x, &mouse_y);
            if (is_click_inside(mouse_x, mouse_y, WIDTH * CELL_SIZE + 5, 162, 92, 50)) {
                printf("Tower placement button clicked.\n");
                game.want_to_place_tower = !game.want_to_place_tower;
            } else if (game.want_to_place_tower) {
                Point gridPosition = {mouse_x / CELL_SIZE, mouse_y / CELL_SIZE};
                if (isWithinBounds(gridPosition.x, gridPosition.y)) {
                    Gemme *newGem = NULL; // Here you would create a new Gemme based on user input
                    placeTower(&game, gridPosition, newGem);
                }
            }
        } else if (event == MLV_KEY) {
            MLV_Keyboard_button key;
            MLV_get_keyboard_state(&key);
            if (key == MLV_KEYBOARD_SPACE) {
                game.manual_trigger = 1;
                game.last_wave_time = currentTime - WAVE_INTERVAL * 1000;
            }
        }

        // Draw everything
        drawAll(&game);

        // Frame rate control
        int frameTime = MLV_get_time() - currentTime;
        if (frameTime < frameDelay) {
            MLV_delay_according_to_frame_rate(frameDelay, frameTime);
        }
        
        // Check for game quit condition
        if (game.quit) {
            break;
        }
        
        // Actualise the window
        MLV_actualise_window();
    }

    // Cleanup
    MLV_free_window();
    for (int i = 0; i < game.num_active_waves; i++) {
        if (game.active_waves[i] != NULL) {
            freeWave(game.active_waves[i]);
        }
    }
    free(game.active_waves);
    if (game.path != NULL) {
        free(game.path);
    }

    return 0;
}

