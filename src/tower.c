#include "tower.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Constants for default tower attributes
#define DEFAULT_RANGE 3.0
#define DEFAULT_DAMAGE 50.0
#define DEFAULT_ATTACK_SPEED 1.0
#define DEFAULT_MANA_COST 100
#define DEFAULT_LEVEL 1
#define DEFAULT_COOLDOWN 0.0

void InitializeTower(Tower *tower, Point position, int element_type) {
    if (tower == NULL) {
        // Error handling if the tower pointer is null
        fprintf(stderr, "Error: Tower pointer is null in InitializeTower\n");
        return;
    }

    tower->position = position;

    tower->range = DEFAULT_RANGE;
    tower->damage = DEFAULT_DAMAGE;
    tower->attack_speed = DEFAULT_ATTACK_SPEED;
    tower->mana_cost = DEFAULT_MANA_COST;
    tower->level = DEFAULT_LEVEL;
    tower->element_type = element_type; 

    tower->targetID = -1;                
    tower->cooldown = DEFAULT_COOLDOWN;
    tower->gem_level = 0;                
    tower->reaction_type = 0;            
    tower->effect_duration = 0.0;
    tower->slow_effect = 0.0;
    tower->splash_radius = 0.0;
    tower->parasite_damage = 0.0;

}

