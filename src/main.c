#include <MLV/MLV_all.h>
#include "grid.h"
#include "monstre.h"
#include "graph.h"
#include <time.h>
#include "game.h"

int main() {
    srand(time(NULL));
    MLV_create_window("Tower Defense Grid", "TD Grid", WIDTH * CELL_SIZE + 200, HEIGHT * CELL_SIZE);
    Game game = {.wave = 1, .mana = 150 , .mana_max = 2000, 
        .path = NULL, .pathSize = 0, .quit = 0 , .nb_tour = 0,
        .level_mana = 0};
    //Monster Monsters[monster_count]; 

    const int frameDelay = 1000 / 60;
    MLV_change_frame_rate(60);

    generatePath(game.grid, &game.path, &game.pathSize);
    Monster * Monsters = initializeWave(game.wave, game.path, game.pathSize);
    int previousTime = MLV_get_time();

    while (!game.quit) {
        int currentTime = MLV_get_time();
        float deltaTime = (currentTime - previousTime) / 1000.0f; 
        previousTime = currentTime; 

        int monster_count = moveMonsters(Monsters, game.path, game.pathSize, deltaTime); // Pass deltaTime to moveMonsters

        /* Draw */
        drawAll(&game, Monsters, monster_count);

        /* Frame rate control */
        int frameTime = MLV_get_time() - currentTime;
        if (frameTime < frameDelay) {
            MLV_delay_according_to_frame_rate(frameDelay, frameTime);
        }

        // Check for user input to potentially set 'quit' to 1 and exit the loop
    }

    MLV_free_window();
    if (game.path != NULL) {
        free(game.path);
    }

    return 0;
}