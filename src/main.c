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

    /* loop */
    while (!quit) {
        int start_time = MLV_get_time(); 

        moveMonsters(Monsters, MONSTER_COUNT, path, pathSize);
        //printf("1 turn");

        /* Draw */
        MLV_clear_window(MLV_COLOR_BLACK);
        draw_grid_with_path(grid, path, pathSize);
        draw_start_and_finish(path[0], path[pathSize - 1]);
        drawMonsters(Monsters, MONSTER_COUNT); 
        MLV_actualise_window();

        /* Frame rate */
        int frame_time = MLV_get_time() - start_time;
        if (frame_time < frameDelay) {
            MLV_delay_according_to_frame_rate(frameDelay, frame_time);
        }
    }


    MLV_free_window();
    if (path != NULL) {
        free(path);
    }

    return 0;
}
