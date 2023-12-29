#ifndef GRAPH_H
#define GRAPH_H

#include <MLV/MLV_all.h>
#include "grid.h"

#define CELL_SIZE 25 
#define WIDTH 28     
#define HEIGHT 22    
#define PATH 1       
#define EMPTY 0      


void draw_cell(int x, int y, MLV_Color color);
void draw_grid_with_path(int grid[HEIGHT][WIDTH], Point *path, int pathSize);
void draw_start_and_finish(Point start, Point finish);
#endif