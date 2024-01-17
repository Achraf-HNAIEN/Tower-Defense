
#ifndef TOWER_H
#define TOWER_H

#include "grid.h"    
#include "gemme.h"


#define TOWER 2
#define MAX_TOWERS 30

typedef struct {
    Point position;    
    Gemme *gemme;      
} Tower;

void InitializeTower(Tower *tower, Point position);

int CanBuildTower(int grid[HEIGHT][WIDTH], Point position);

int PlaceGemmeInTower(Tower *tower, Gemme *gemme);

void RemoveGemmeFromTower(Tower *tower);

#endif
