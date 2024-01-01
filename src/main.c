#include <MLV/MLV_all.h>
#include "grid.h"
#include "monstre.h"
#include "graph.h"
#include <time.h>

int main() {
    MLV_create_window("Tower Defense Grid", "TD Grid", WIDTH * CELL_SIZE, HEIGHT * CELL_SIZE);

    int grid[HEIGHT][WIDTH];
    Point *path = NULL;
    Monster Monsters[MONSTER_COUNT];
    int pathSize = 0;
    int quit = 0;
    const int frameDelay = 1000 / 60;

    MLV_change_frame_rate(60);
    generatePath(grid, &path, &pathSize);
    initializeMonsters(Monsters, MONSTER_COUNT, path);

    int previousTime = MLV_get_time();

    while (!quit) {
        int currentTime = MLV_get_time();
        float deltaTime = (currentTime - previousTime) / 1000.0f; // Calculate deltaTime in seconds
        previousTime = currentTime; // Update previousTime for the next frame

        moveMonsters(Monsters, MONSTER_COUNT, path, pathSize, deltaTime); // Pass deltaTime to moveMonsters


        /* Draw */
        MLV_clear_window(MLV_COLOR_BLACK);
        draw_grid_with_path(grid, path, pathSize);
        drawMonsters(Monsters, MONSTER_COUNT);
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
