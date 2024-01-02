#include "tower.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void InitializeTower(Tower *tower, int x, int y, float range, float damage, 
                     float attack_speed, float mana_cost, int element_type) {
    if (tower == NULL) {
        fprintf(stderr, "InitializeTower: 'tower' is NULL\n");
        return;
    }
    
    //position
    tower->x = x;
    tower->y = y;
    
    //initial info 
    tower->range = range;
    tower->damage = damage;
    tower->attack_speed = attack_speed;
    tower->mana_cost = mana_cost;
    tower->level = 1; //1st level and when upgarde level go >>>>>> up
    tower->element_type = element_type;
    
    //autres
    tower->targetID = -1; // No target at the beging
    tower->cooldown = 0.0f; // 0 means tower is ready and cool
    tower->gem_level = 0; // No gemmes on initialisation
    tower->reaction_type = 0; // No reactions until gemmes is > 0
    tower->effect_duration = 0.0f; // No effect until start shoting
    tower->slow_effect = 0.0f; // No slow effect until a Hydro equipped
    tower->splash_radius = 0.0f; // No splash until a Pyro equipped
    tower->parasite_damage = 0.0f; // No parasite damage until a Dendro equipped
}


int CanBuildTower(Point position, int grid[HEIGHT][WIDTH]) {
    return grid[position.y][position.x] == !EMPTY;
}
/*
exemple of use in the future
Point desiredPosition = {x, y};

if (CanBuildTower(desiredPosition, grid)) {
    
    Tower newTower;
    InitializeTower(&newTower, desiredPosition.x, desiredPosition.y, ....);
    // Add tower to the game's list of towers, update the grid, etc.
} else {
    // Inform the player they can't build here
    printf("Cannot build a tower at (%d, %d). Position is not empty.\n", desiredPosition.x, desiredPosition.y);
}

*/