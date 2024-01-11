
#ifndef TOWER_H
#define TOWER_H

#include "grid.h"    
#include "gemme.h"





// Constants for tower construction
#define INITIAL_FREE_TOWERS 3   
#define INITIAL_TOWER_COST 100  
#define TOWER_COST_MULTIPLIER 2 
#define TOWER 2
#define MAX_TOWERS 10

typedef struct {
    Point position;    // Tower position on the grid
    Gemme *gemme;      // Pointer to a Gemme, NULL if no gemme is placed
} Tower;

void InitializeTower(Tower *tower, Point position);

int CanBuildTower(const int grid[HEIGHT][WIDTH], Point position);

void PlaceGemmeInTower(Tower *tower, Gemme *gemme);

void RemoveGemmeFromTower(Tower *tower);

#endif
