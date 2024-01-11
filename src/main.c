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
        .nb_tour = 0,
        .level_mana = 0,
        .level_gemme_in_shop = 0,
        .built_tower_count = INITIAL_FREE_TOWERS,
        .want_to_place_tower = 0,
        .tower_placement_cost = INITIAL_TOWER_COST,
        .tower_count = 0};

    const int frameDelay = 1000 / 60;
    MLV_change_frame_rate(60);

    generatePath(game.grid, &game.path, &game.pathSize);
    Monster *Monsters = initializeWave(game.wave, game.path, game.pathSize);
    int previousTime = MLV_get_time();

    while (!game.quit)
    {
        int currentTime = MLV_get_time();
        float deltaTime = (currentTime - previousTime) / 1000.0f;
        previousTime = currentTime;

        int monster_count = moveMonsters(Monsters, game.path, game.pathSize, deltaTime, &game);
        MLV_Event event;
        int mouse_x, mouse_y;
        MLV_Button_state state;
        event = MLV_get_event(NULL, NULL, NULL, NULL, NULL, &mouse_x, &mouse_y, NULL, &state);

        if (event == MLV_MOUSE_BUTTON && state == MLV_PRESSED)
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
                }
            }
        }

        drawAll(&game, Monsters, monster_count);
        MLV_actualise_window();

        int frameTime = MLV_get_time() - currentTime;
        if (frameTime < frameDelay)
        {
            MLV_delay_according_to_frame_rate(frameDelay, frameTime);
        }
    }
    MLV_actualise_window();

    MLV_free_window();
    if (game.path != NULL)
    {
        free(game.path);
    }

    return 0;
}
