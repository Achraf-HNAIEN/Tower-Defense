// #include <string.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include "grid.h"
// #include "monstre.h"

// int main() {
//     int grid[HEIGHT][WIDTH];
//     Monster Monsters[MONSTER_COUNT];
//     Point *path = NULL;
//     int pathSize = 0;

//     // Generate the path
//     generatePath(grid, &path, &pathSize);
//     printGrid(grid);

//     //initializeMonsters(Monsters, MONSTER_COUNT, SPEED_NORMAL);
//     // moveMonsters(Monsters, path, pathSize);
//     // printMonsterPositions(Monsters, MONSTER_COUNT);

//     if (path != NULL) {
//         free(path);
//     }

//     return 0;
// }

#include <MLV/MLV_all.h>
#include "grid.h"
#include "monstre.h"
#include "graph.h"


int main()
{
    MLV_create_window("Tower Defense Grid", "TD Grid", WIDTH * CELL_SIZE, HEIGHT * CELL_SIZE);

    int grid[HEIGHT][WIDTH];
    Monster Monsters[MONSTER_COUNT];
    Point *path = NULL;
    int pathSize = 0;

    /* Generate the path */
    generatePath(grid, &path, &pathSize);

    /* Draw */
    MLV_clear_window(MLV_COLOR_BLACK);
    draw_grid_with_path(grid,path,pathSize);
    draw_start_and_finish(path[0], path[pathSize - 1]);
    MLV_actualise_window();


    MLV_wait_keyboard(NULL, NULL, NULL);


    MLV_free_window();
    if (path != NULL)
    {
        free(path);
    }

    return 0;
}
