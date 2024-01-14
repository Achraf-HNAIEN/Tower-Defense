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
        .wavesHead = NULL,
        .inventory_size = 0
    };

    Wave* headWave = NULL;  // Head of the waves linked list
    Wave* currentWave;      // Current wave pointer for iteration

    const int frameDelay = 1000 / 60;
    MLV_change_frame_rate(60);
    generatePath(game.grid, &game.path, &game.pathSize);

    int previousTime = MLV_get_time();
    int last_wave_time = previousTime;

    while (!game.quit) {
        int currentTime = MLV_get_time();
        float deltaTime = (currentTime - previousTime) / 1000.0f;
        previousTime = currentTime;
        MLV_actualise_window();

        // Update and handle each wave
        currentWave = headWave;
        while (currentWave != NULL) {
            moveMonsters(currentWave->monsters, game.path, game.pathSize, deltaTime, &game);
            currentWave = currentWave->next;
        }

        // Handle events
        MLV_Event event;
        int mouse_x, mouse_y;
        MLV_Button_state state;
        MLV_Keyboard_button key;
        event = MLV_get_event(&key, NULL, NULL, NULL, NULL, &mouse_x, &mouse_y, NULL, &state);

        if (event == MLV_MOUSE_BUTTON && state == MLV_RELEASED) {
            printf("Mouse clicked at: x=%d, y=%d\n", mouse_x, mouse_y);
            if (is_click_inside(mouse_x, mouse_y, WIDTH * CELL_SIZE + 5, 162, 92, 50)) {
                printf("Tower placement button clicked.\n");
                game.want_to_place_tower = !game.want_to_place_tower;
            } else if (game.want_to_place_tower) {
                Point gridPosition = {mouse_x / CELL_SIZE, mouse_y / CELL_SIZE};
                if (isWithinBounds(gridPosition.x, gridPosition.y)) {
                    Gemme *newGem = NULL; // This should be replaced with actual gem creation logic
                    placeTower(&game, gridPosition, newGem);
                    game.want_to_place_tower = !game.want_to_place_tower;
                }
            } else if (is_click_inside(mouse_x, mouse_y, WIDTH * CELL_SIZE + 181, 146, 17, 17) && game.level_gemme_in_shop < 12) {
                game.level_gemme_in_shop++;
            } else if (is_click_inside(mouse_x, mouse_y, WIDTH * CELL_SIZE + 181, 211, 17, 17) && game.level_gemme_in_shop > 0) {
                game.level_gemme_in_shop--;
            } else if (is_click_inside(mouse_x, mouse_y, WIDTH * CELL_SIZE + 15, 80, 170, 50)) {
                upgrade_mana_storage(&game);
            }
        }

        // Check for new wave trigger
        if ((event == MLV_KEY && key == MLV_KEYBOARD_SPACE && state == MLV_RELEASED) || currentTime - last_wave_time >= WAVE_INTERVAL * 1000) {
            Wave* newWave = initializeWave(game.wave, game.path, game.pathSize);

            // Append the new wave to the list
            if (headWave == NULL) {
                headWave = newWave;
            } else {
                Wave* temp = headWave;
                while (temp->next != NULL) {
                    temp = temp->next;
                }
                temp->next = newWave;
            }

            last_wave_time = MLV_get_time();
            add_mana(&game, (WAVE_INTERVAL - (currentTime - last_wave_time) / 1000.0f) * (game.mana_max / 100));
            game.wave++;
        }

        // Draw everything
        currentWave = headWave;
        while (currentWave != NULL) {
            drawAll(&game, headWave);
            currentWave = currentWave->next;
        }

        // Frame rate control
        int frameTime = MLV_get_time() - currentTime;
        if (frameTime < frameDelay) {
            MLV_delay_according_to_frame_rate(frameDelay, frameTime);
        }
    }

    // Clean up resources
    MLV_free_window();
    if (game.path != NULL) {
        free(game.path);
    }

    // Free all the waves and monsters
    currentWave = headWave;
    while (currentWave != NULL) {
        Wave* nextWave = currentWave->next;
        if (currentWave->monsters != NULL) {
            free(currentWave->monsters);
        }
        free(currentWave);
        currentWave = nextWave;
    }

    return 0;
}
