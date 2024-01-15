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


//void draw_cell(int x, int y, MLV_Color color);
//void draw_grid_with_path(int grid[HEIGHT][WIDTH], Point *path, int pathSize);
//void draw_start_and_finish(Point start, Point finish);
//void drawMonsters(Monster monsters[], int count);
int is_click_inside(int mouse_x, int mouse_y, int x, int y, int width, int height);
void drawAll(Game *game, Wave *headWave);
//void draw_side_information(Game *);
#endif
