#ifndef GRAPH_H
#define GRAPH_H

#include <MLV/MLV_all.h>
#include "grid.h"
#include "monstre.h"
#include "game.h"
#include "tower.h"

#define WIDTH 28     
#define HEIGHT 22    
#define PATH 1       
#define EMPTY 0
#define HEALTH_BAR_WIDTH 20
#define HEALTH_BAR_HEIGHT 3


int is_click_inside(int mouse_x, int mouse_y, int x, int y, int width, int height);
void drawAll(Game *game, Wave *headWave,float deltaTime);
void updateProjectilePosition(Game *game, float deltaTime);
void drawProjectiles(Game *game);
void cleanupProjectiles(Game *game);
    void debugPrintProjectiles(Game *game);

#endif
