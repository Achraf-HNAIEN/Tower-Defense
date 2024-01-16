
#ifndef TOWER_H
#define TOWER_H

#include "grid.h"    
#include "gemme.h"


#define TOWER 2
#define MAX_TOWERS 10

typedef struct {
    Point position;    
    Gemme *gemme;      // Pointer to a Gemme, NULL if no gemme is placed
} Tower;

void InitializeTower(Tower *tower, Point position);

int CanBuildTower(const int grid[HEIGHT][WIDTH], Point position);

void PlaceGemmeInTower(Tower *tower, Gemme *gemme);

void RemoveGemmeFromTower(Tower *tower);

#endif
