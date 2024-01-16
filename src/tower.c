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

int CanBuildTower(const int grid[HEIGHT][WIDTH], Point position) {
    if (!isWithinBounds(position.x, position.y)) {
        return 0; //position is out of bounds
    }
    return grid[position.y][position.x] == EMPTY;
}

void PlaceGemmeInTower(Tower *tower, Gemme *gemme) {
    if (tower == NULL) {
        printf("Error ------> Tower is NULL in place gemme in tower\n");
        return;
    }
    if (gemme == NULL) {
        printf("Error ------> Gemme NULL in place Gemme in tower function\n");
        return;
    }

    if (tower->gemme != NULL) {
        printf("Warning ------> there is all ready a gemme on this tower \n");

    }

    tower->gemme = gemme;
    if (tower->gemme != NULL) {
        tower->gemme->chargeTime = GEMME_CHARGE_TIME;
        tower->gemme->isReadyToShoot = 0;
    }

    printf("Gem placed in tower. It will be ready to shoot after %f seconds.\n", GEMME_CHARGE_TIME);
}

void RemoveGemmeFromTower(Tower *tower) {
    if (tower == NULL) {
        printf("Error -------> Tower is NULL in Remove gemme from Tower\n");
        return;
    }
    if (tower->gemme == NULL) {
        printf("Warning ------> No gemme to remove from this tower.\n");
        return;
    }
    tower->gemme = NULL;
    printf("Gemme removed from tower with success\n");
}


