#include "tower.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void InitializeTower(Tower *tower, Point position) {
    if (tower == NULL) {
        fprintf(stderr, "Error: Tower pointer is null initialize Tower function\n");
        return;
    }
    tower->position = position;

    tower->gemme = NULL;
}

int CanBuildTower(short grid[HEIGHT][WIDTH], Point position) {
    if (!isWithinBounds(position.x, position.y)) {
        return 0; //position is out of bounds
    }
    return grid[position.y][position.x] == EMPTY;
}

int PlaceGemmeInTower(Tower *tower, Gemme *gemme) {
    if (tower == NULL) {
        return 0;
    }
    if (gemme == NULL) {
        return 0;
    }

    if (tower->gemme != NULL) {
        return 0;
    }

    tower->gemme = gemme;
    if (tower->gemme != NULL) {
        tower->gemme->cooldown = GEMME_CHARGE_TIME;
        tower->gemme->isReadyToShoot = 0;
    }
    return 1;
}


