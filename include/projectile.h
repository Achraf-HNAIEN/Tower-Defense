#ifndef PROJECTILE_H
#define PROJECTILE_H
#include "grid.h"
#include "gemme.h"
#include "MLV/MLV_all.h"
#include <stdbool.h>

#define MAX_PROJECTILES 100
#define PROJECTILE_SPEED 1.25
#define PROJECTILE_SPEED_SLOW 0.75
#define PROJECTILE_SIZE 4
#define PROJECTILE_SPEED2 (3 * CELL_SIZE * 60) // since you're running at 60 frames per second



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
