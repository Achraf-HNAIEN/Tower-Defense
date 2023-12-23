#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "grid.h"
#include "monstre.h"


int main() {
    int grid[HEIGHT][WIDTH];
    Monster Monsters[MONSTER_COUNT];
    Point *path = NULL;
    int pathSize = 0;

    // Generate the path
    initializeMonsters(Monsters, MONSTER_COUNT, SPEED_NORMAL);
    generatePath(grid, &path, &pathSize);
    printGrid(grid);

    moveMonsters(Monsters, path, pathSize);
    printMonsterPositions(Monsters, MONSTER_COUNT);

    if (path != NULL) {
        free(path);
    }

    return 0;
}