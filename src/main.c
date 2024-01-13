#include <time.h>
#include <MLV/MLV_all.h>
#include "grid.h"
#include "monstre.h"
#include "graph.h"
#include "game.h"
#include "tower.h"

int main()
{
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
        // .tower_placement_cost = INITIAL_TOWER_COST,
        .tower_count = 0,
        .wavesHead = NULL,
        .inventory_size = 0
        };

    
    const int frameDelay = 1000 / 60;
    MLV_change_frame_rate(60);
    generatePath(game.grid, &game.path, &game.pathSize);
    // Monster *Monsters = NULL ;
    Monster *Monsters =  initialize_Monster(game.wave, game.path, game.pathSize) ; // A FAIRE : attendre que le joueur déclenche la première wave.
    int previousTime = MLV_get_time();
    int last_wave_time = previousTime;
    while (!game.quit)
    {

        int currentTime = MLV_get_time();
        float deltaTime = (currentTime - previousTime) / 1000.0f;
        previousTime = currentTime;
        MLV_actualise_window();
        int monster_count = moveMonsters(Monsters, game.path, game.pathSize, deltaTime, &game);
        MLV_Event event;
        int mouse_x, mouse_y;
        MLV_Button_state state;
        MLV_Keyboard_button key;
        event = MLV_get_event(&key, NULL, NULL, NULL, NULL, &mouse_x, &mouse_y, NULL, &state);

        if (event == MLV_MOUSE_BUTTON && state == MLV_RELEASED)
        {
            printf("Mouse clicked at: x=%d, y=%d\n", mouse_x, mouse_y);
            if (is_click_inside(mouse_x, mouse_y, WIDTH * CELL_SIZE + 5, 162, 92, 50))
            {
                printf("Tower placement button clicked.\n");
                game.want_to_place_tower = !game.want_to_place_tower;
            }
            else if (game.want_to_place_tower)
            {
                Point gridPosition = {mouse_x / CELL_SIZE, mouse_y / CELL_SIZE};
                if (isWithinBounds(gridPosition.x, gridPosition.y))
                {
                    //attemptPlaceTower(&game, gridPosition);
                    Gemme *newGem = NULL;
                    placeTower(&game, gridPosition, newGem);
                    game.want_to_place_tower = !game.want_to_place_tower;
                }
            }
            else if (is_click_inside(mouse_x, mouse_y, WIDTH * CELL_SIZE + 181, 146,17,17) && game.level_gemme_in_shop < 12){
                game.level_gemme_in_shop++;
            }
            else if (is_click_inside(mouse_x, mouse_y, WIDTH * CELL_SIZE + 181, 211,17,17) && game.level_gemme_in_shop > 0 ){
                game.level_gemme_in_shop--;
            }
            else if (is_click_inside(mouse_x, mouse_y, WIDTH * CELL_SIZE + 15, 80, 170, 50)){
                upgrade_mana_storage(&game);
            }
        }
        
        else if ( (event == MLV_KEY  && key == MLV_KEYBOARD_SPACE && state == MLV_RELEASED) || currentTime - last_wave_time >= WAVE_INTERVAL * 1000){
            // nouvelle vague de monstres
            Monsters = initialize_Monster(game.wave, game.path, game.pathSize);
            last_wave_time = MLV_get_time();
            add_mana(&game, (WAVE_INTERVAL - (currentTime - last_wave_time) / 1000.0f) * (game.mana_max / 100)) ;
            game.wave++;
        }

        drawAll(&game, Monsters, monster_count);

        int frameTime = MLV_get_time() - currentTime;
        if (frameTime < frameDelay)
        {
            MLV_delay_according_to_frame_rate(frameDelay, frameTime);
        }
    }
    MLV_free_window();
    if (game.path != NULL)
    {
        free(game.path);
    }

    return 0;
}
