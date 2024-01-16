#include <time.h>
#include <MLV/MLV_all.h>
#include "grid.h"
#include "monstre.h"
#include "graph.h"
#include "game.h"
#include "tower.h"
// https://elearning.univ-eiffel.fr/pluginfile.php/477685/mod_resource/content/5/projet-tower-defense-compressed.pdf



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
        .inventory_size = 0,
        .has_start = 0,
        .gemme_selected = -1, // -1 mean no gemme selected
        .numVisualProjectiles = 0
    };
    memset(game.visualProjectiles, 0, sizeof(game.visualProjectiles));
    Wave* headWave = NULL;  // Head of the waves linked list
    Wave* currentWave;      // Current wave pointer for iteration

    const int frameDelay = 1000 / 60;
    MLV_change_frame_rate(60);
    generatePath(game.grid, &game.path, &game.pathSize);

    int previousTime = MLV_get_time();
    int last_wave_time = previousTime;
    drawAll(&game, NULL);
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
            else if(is_click_inside(mouse_x, mouse_y, WIDTH * CELL_SIZE + 105, 162, 93, 50)){
               buy_gemme(&game);
            }
            else if(is_click_inside(mouse_x,mouse_y, WIDTH * CELL_SIZE + 25, 315,150,210)){
                handle_inventory_click(mouse_x, mouse_y, &game);
            }
        }
        // Check for new wave trigger
        else if (game.has_start && ((event == MLV_KEY && key == MLV_KEYBOARD_SPACE && state == MLV_RELEASED) || currentTime - last_wave_time >= WAVE_INTERVAL * 1000)) {
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
            
            add_mana(&game, (WAVE_INTERVAL - (currentTime - last_wave_time) / 1000) * (game.mana_max / 100));
            last_wave_time = MLV_get_time();
            game.wave++;
        }
        else if(!game.has_start && (event == MLV_KEY && key == MLV_KEYBOARD_SPACE && state == MLV_RELEASED)){
            game.has_start = !game.has_start;
            Wave* newWave = initializeWave(game.wave, game.path, game.pathSize);
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
        }

        game.next_wave_time = (WAVE_INTERVAL - (currentTime - last_wave_time) / 1000);

        // Draw everything
        currentWave = headWave;
        if(NULL == currentWave){
            drawAll(&game, NULL);
        }else{
            while (currentWave != NULL) {
                drawAll(&game, headWave);
                currentWave = currentWave->next;
            }
        }
        if(game.want_to_place_tower && mouse_x >= 0 && mouse_y >= 0 && mouse_x <= WIDTH*CELL_SIZE && mouse_y <= HEIGHT*CELL_SIZE){
            MLV_draw_filled_circle(mouse_x, mouse_y, CELL_SIZE*3, MLV_rgba(0,255,0,120));
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
