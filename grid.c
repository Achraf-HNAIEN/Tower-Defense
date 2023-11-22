#include <MLV/MLV_all.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define GRID_WIDTH 28
#define GRID_HEIGHT 22
#define CELL_SIZE 30

void initialize_grid(int grid[GRID_WIDTH][GRID_HEIGHT]) {
    for (int i = 0; i < GRID_WIDTH; ++i) {
        for (int j = 0; j < GRID_HEIGHT; ++j) {
            grid[i][j] = 0;  // 0 represents an empty cell
        }
    }
}

void draw_grid(int grid[GRID_WIDTH][GRID_HEIGHT]) {
    MLV_clear_window(MLV_COLOR_BLACK);

    // Draw vertical lines
    for (int i = 0; i <= GRID_WIDTH; ++i) {
        MLV_draw_line(i * CELL_SIZE, 0, i * CELL_SIZE, GRID_HEIGHT * CELL_SIZE, MLV_COLOR_WHITE);
    }

    // Draw horizontal lines
    for (int j = 0; j <= GRID_HEIGHT; ++j) {
        MLV_draw_line(0, j * CELL_SIZE, GRID_WIDTH * CELL_SIZE, j * CELL_SIZE, MLV_COLOR_WHITE);
    }

    // Fill cells
    for (int i = 0; i < GRID_WIDTH; ++i) {
        for (int j = 0; j < GRID_HEIGHT; ++j) {
            if (grid[i][j] == 1) {
                MLV_draw_filled_rectangle(i * CELL_SIZE, j * CELL_SIZE, CELL_SIZE, CELL_SIZE, MLV_COLOR_WHITE);
            }
        }
    }

    MLV_actualise_window();
}

int is_valid_move(int x, int y) {
    return x >= 0 && x < GRID_WIDTH && y >= 0 && y < GRID_HEIGHT;
}

void generate_path(int grid[GRID_WIDTH][GRID_HEIGHT], int length) {
    int x = GRID_WIDTH / 2;
    int y = GRID_HEIGHT / 2;

    for (int i = 0; i < length; ++i) {
        grid[x][y] = 1;  // 1 represents a part of the path

        // Move in a random direction
        int direction = rand() % 4;  // 0: north, 1: east, 2: south, 3: west

        switch (direction) {
            case 0:  // North
                if (is_valid_move(x, y - 1)) {
                    y -= 1;
                }
                break;
            case 1:  // East
                if (is_valid_move(x + 1, y)) {
                    x += 1;
                }
                break;
            case 2:  // South
                if (is_valid_move(x, y + 1)) {
                    y += 1;
                }
                break;
            case 3:  // West
                if (is_valid_move(x - 1, y)) {
                    x -= 1;
                }
                break;
        }
    }
}

int main() {
    srand((unsigned int)time(NULL));

    int grid[GRID_WIDTH][GRID_HEIGHT];
    initialize_grid(grid);

    int path_length = 100;  // Adjust the path length as needed
    generate_path(grid, path_length);

    MLV_create_window("Random Path Generation", "Random Path", GRID_WIDTH * CELL_SIZE, GRID_HEIGHT * CELL_SIZE);

    draw_grid(grid);

    MLV_wait_seconds(5);
    MLV_free_window();

    return 0;
}
