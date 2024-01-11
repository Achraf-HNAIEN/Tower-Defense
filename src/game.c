#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "game.h"

void placeTower(Game *game, Point gridPosition, Gemme *gemme) {
    // Check if we can build a tower here and if the maximum number of towers hasn't been reached
    if (!CanBuildTower(game->grid, gridPosition) || game->tower_count >= MAX_TOWERS) {
        printf("Cannot build tower here or maximum number of towers reached.\n");
        return;
    }

    // Calculate the cost for the next tower
    int towerCost = (game->built_tower_count < INITIAL_FREE_TOWERS) ? 0 : 
                    INITIAL_TOWER_COST * (1 << (game->built_tower_count - INITIAL_FREE_TOWERS));

    // Check if we have enough mana to build the tower
    if (game->mana < towerCost) {
        printf("Not enough mana to build a tower.\n");
        return;
    }

    // Deduct the mana cost and increment the built tower count
    game->mana -= towerCost;
    game->built_tower_count++;

    // Update the grid to place the tower
    game->grid[gridPosition.y][gridPosition.x] = TOWER;

    // Initialize the new tower and place it in the array of towers
    Tower newTower;
    InitializeTower(&newTower, gridPosition);
    if (gemme) {
        PlaceGemmeInTower(&newTower, gemme);
    }
    game->towers[game->tower_count] = newTower;
    game->tower_count++;

    // Log the placement for debugging
    printf("Tower placed at (%d, %d).\n", gridPosition.x, gridPosition.y);

    // Call the function to update the graphics
    // RedrawGame(game); // Uncomment or implement this as needed
}
