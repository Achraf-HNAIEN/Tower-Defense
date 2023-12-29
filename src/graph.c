#include "graph.h"



void draw_cell(int x, int y, MLV_Color color) {
    MLV_draw_filled_rectangle(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE, color);
}
void draw_grid_with_path(int grid[HEIGHT][WIDTH], Point *path, int pathSize) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            draw_cell(j, i, MLV_COLOR_GRAY); 
        }
    }

    for (int i = 0; i < pathSize; i++) {
        draw_cell(path[i].x, path[i].y, MLV_COLOR_WHITE); 
    }

    for (int i = 0; i <= HEIGHT; i++) {
        MLV_draw_line(0, i * CELL_SIZE, WIDTH * CELL_SIZE, i * CELL_SIZE, MLV_COLOR_BLACK);
    }
    for (int j = 0; j <= WIDTH; j++) {
        MLV_draw_line(j * CELL_SIZE, 0, j * CELL_SIZE, HEIGHT * CELL_SIZE, MLV_COLOR_BLACK);
    }
}

void draw_start_and_finish(Point start, Point finish) {
    draw_cell(start.x, start.y, MLV_COLOR_RED);   
    draw_cell(finish.x, finish.y, MLV_COLOR_GREEN);
}

void drawMonsters(Monster monsters[], int count) {
    int monsterSize = CELL_SIZE / 4; 

    for (int i = 0; i < count; i++) {
        if (monsters[i].hp <= 0) continue;

        int pixelX = monsters[i].x * CELL_SIZE + CELL_SIZE / 2;
        int pixelY = monsters[i].y * CELL_SIZE + CELL_SIZE / 2;

        MLV_draw_filled_circle(pixelX, pixelY, monsterSize, MLV_COLOR_RED);
    }
}
