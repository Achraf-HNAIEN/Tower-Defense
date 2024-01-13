#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>

#include "game.h"


void placeTower(Game *game, Point gridPosition, Gemme *gemme) {
    if (!CanBuildTower(game->grid, gridPosition) || game->nb_tower >= MAX_TOWERS) {
        printf("Cannot build tower here or maximum number of towers reached.\n");
        return;
    }

    int towerCost = game->nb_tower < 3? 0 : 100 * pow(2,(game->nb_tower+1)-4);

    if (game->mana < towerCost) {
        printf("Not enough mana to build a tower.\n");
        return;
    }

    game->mana -= towerCost;
    game->nb_tower++;

    game->grid[gridPosition.y][gridPosition.x] = TOWER;

    Tower newTower;
    InitializeTower(&newTower, gridPosition);
    if (gemme) {
        PlaceGemmeInTower(&newTower, gemme);
    }
    game->towers[game->tower_count] = newTower;
    game->tower_count++;

    printf("Tower placed at (%d, %d).\n", gridPosition.x, gridPosition.y);

 
}
