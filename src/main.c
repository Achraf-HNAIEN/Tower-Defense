#include <MLV/MLV_all.h>
#include "grid.h"
#include "monstre.h"
#include "graph.h"
#include <time.h>

int main() {
    srand(time(NULL));
    MLV_create_window("Tower Defense Grid", "TD Grid", WIDTH * CELL_SIZE + 200, HEIGHT * CELL_SIZE);
    int wave = 1;
    int monster_count = 12; // LE 12 DEPEND DE LA VAGUE
    int grid[HEIGHT][WIDTH];
    Point *path = NULL;
    Monster Monsters[monster_count]; 
    int pathSize = 0;
    int quit = 0;
    const int frameDelay = 1000 / 60;

    MLV_change_frame_rate(60);
    generatePath(grid, &path, &pathSize);
    initializeWave(Monsters, wave, path, pathSize);
    int previousTime = MLV_get_time();

    while (!quit) {
        int currentTime = MLV_get_time();
        float deltaTime = (currentTime - previousTime) / 1000.0f; // Calculate deltaTime in seconds
        previousTime = currentTime; // Update previousTime for the next frame

        moveMonsters(Monsters, monster_count, path, pathSize, deltaTime); // Pass deltaTime to moveMonsters

        /* Draw */
        MLV_clear_window(MLV_COLOR_BLACK);
        draw_grid_with_path(grid, path, pathSize);
        drawMonsters(Monsters, monster_count);
        draw_start_and_finish(path[0], path[pathSize - 1]);
        MLV_actualise_window();

        /* Frame rate control */
        int frameTime = MLV_get_time() - currentTime;
        if (frameTime < frameDelay) {
            MLV_delay_according_to_frame_rate(frameDelay, frameTime);
        }

        // Check for user input to potentially set 'quit' to 1 and exit the loop
    }

    MLV_free_window();
    if (path != NULL) {
        free(path);
    }

    return 0;
}
