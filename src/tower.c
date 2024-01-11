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
        printf("Warning ------> \n");

    }

    tower->gemme = gemme;
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


