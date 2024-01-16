#ifndef PROJECTILE_H
#define PROJECTILE_H
#include "grid.h"
#include "MLV/MLV_all.h"
#define MAX_PROJECTILES 100

typedef struct {
    Point start;  
    Point end;    
    MLV_Color color; 
} VisualProjectile;

#endif
