#ifndef PROJECTILE_H
#define PROJECTILE_H
#include "grid.h"
#include "gemme.h"
#include "MLV/MLV_all.h"
#include <stdbool.h>

#define MAX_PROJECTILES 100
#define PROJECTILE_SPEED 2.5
#define PROJECTILE_SPEED_SLOW 0.75
#define PROJECTILE_SIZE 3



/**
 * @struct VisualProjectile
 * @brief Represents a visual projectile in the game.
 * 
 * This struct contains information about the start and end coordinates of the projectile,
 * its color, whether it is active or not, and the gem and target monster associated with it.
 */
typedef struct {
    float start_x;      
    float start_y;      
    float end_x;        
    float end_y;        
    MLV_Color color;
    int active;         
    Gemme *gem;
    Monster *targetMonster;
} VisualProjectile;




#endif
