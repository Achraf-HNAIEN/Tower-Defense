
#ifndef TOWER_H
#define TOWER_H

#include "grid.h"    
#include "gemme.h"


#define TOWER 2
#define MAX_TOWERS 30
/**
 * @struct Tower
 * Represents a tower in the game.
 * A tower has a position and can hold a gemme.
 */
typedef struct {
    Point position;    
    Gemme *gemme;      
} Tower;

/**
 * Initializes a tower with the given position.
 * 
 * @param tower The tower to initialize.
 * @param position The position of the tower.
 */
void InitializeTower(Tower *tower, Point position);

/**
 * Checks if a tower can be built at the given position on the grid.
 * 
 * @param grid The game grid.
 * @param position The position to check.
 * @return 1 if a tower can be built, 0 otherwise.
 */
int CanBuildTower(short grid[HEIGHT][WIDTH], Point position);

/**
 * Places a gemme in the tower.
 * 
 * @param tower The tower to place the gemme in.
 * @param gemme The gemme to place.
 * @return 1 if the gemme was successfully placed, 0 otherwise.
 */
int PlaceGemmeInTower(Tower *tower, Gemme *gemme);


#endif
