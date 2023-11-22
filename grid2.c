#include <MLV/MLV_all.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define GRID_WIDTH 28
#define GRID_HEIGHT 22
#define CELL_SIZE 40

void initialize_grid(int grid[GRID_WIDTH][GRID_HEIGHT]) {
    for (int i = 0; i < GRID_WIDTH; ++i) {
        for (int j = 0; j < GRID_HEIGHT; ++j) {
            grid[i][j] = 0;  // 0 represents an empty cell
        }
    }
}

void draw_grid(int grid[GRID_WIDTH][GRID_HEIGHT]) {
    MLV_clear_window(MLV_COLOR_GRAY);

    // Draw vertical lines
    for (int i = 0; i <= GRID_WIDTH; ++i) {
        MLV_draw_line(i * CELL_SIZE, 0, i * CELL_SIZE, GRID_HEIGHT * CELL_SIZE, MLV_COLOR_BLACK);
    }

    // Draw horizontal lines
    for (int j = 0; j <= GRID_HEIGHT; ++j) {
        MLV_draw_line(0, j * CELL_SIZE, GRID_WIDTH * CELL_SIZE, j * CELL_SIZE, MLV_COLOR_BLACK);
    }

    // Fill cells
    for (int i = 0; i < GRID_WIDTH; ++i) {
        for (int j = 0; j < GRID_HEIGHT; ++j) {
            if (grid[i][j] == 1) {
                MLV_draw_filled_rectangle(i * CELL_SIZE, j * CELL_SIZE, CELL_SIZE, CELL_SIZE, MLV_COLOR_WHITE);
            }
        }
    }
}

int is_valid_move(int x, int y, int grid[GRID_WIDTH][GRID_HEIGHT]) {
    return x >= 0 && x < GRID_WIDTH && y >= 0 && y < GRID_HEIGHT && grid[x][y] == 0;
}

void generate_path(int grid[GRID_WIDTH][GRID_HEIGHT], int length) {
    int startX, startY;

    // Start from a random point at least 3 cells away from the border
    do {
        startX = rand() % (GRID_WIDTH - 6) + 3;
        startY = rand() % (GRID_HEIGHT - 6) + 3;
    } while (!is_valid_move(startX, startY, grid));

    int x = startX;
    int y = startY;

    int last_direction = -1;  // -1 means no direction

    int turns = 0;  // Number of turns made by the path

    while (turns < 7 || length > 75) {
        grid[x][y] = 1;
        length--;

        // Move in a random direction, avoiding the last direction with a 50% chance
        int direction = rand() % 4;  // 0: North, 1: East, 2: South, 3: West

        if (direction == last_direction && rand() % 2 == 0) {
            // Reject the last direction and choose another one
            direction = (direction + 1 + rand() % 3) % 4;
        }

        int dx = 0, dy = 0;

        switch (direction) {
            case 0:  // North
                dy = -1;
                break;
            case 1:  // East
                dx = 1;
                break;
            case 2:  // South
                dy = 1;
                break;
            case 3:  // West
                dx = -1;
                break;
        }

        int extent = 0;

        while (is_valid_move(x + dx, y + dy, grid)) {
            extent++;
            x += dx;
            y += dy;
        }

        // Choose a random segment size between 3 and the extent
        int segment = rand() % (extent - 2) + 3;

        // Move in the chosen direction for the segment size
        for (int i = 0; i < segment; ++i) {
            grid[x][y] = 1;
            length--;

            x += dx;
            y += dy;
        }

        // Update the last direction and increment the number of turns
        last_direction = direction;
        turns++;
    }
}

int main() {
    srand((unsigned int)time(NULL));

    int grid[GRID_WIDTH][GRID_HEIGHT];
    initialize_grid(grid);

    int path_length = 75;  // Adjust the path length for the whole grid

    MLV_create_window("Random Path Generation", "Random Path", GRID_WIDTH * CELL_SIZE, GRID_HEIGHT * CELL_SIZE);

    generate_path(grid, path_length);

    // Color the starting point (green)
    MLV_draw_filled_circle(CELL_SIZE * 3 / 2, CELL_SIZE * 3 / 2, CELL_SIZE / 4, MLV_COLOR_GREEN);

    // Color the finishing point (red)
    MLV_draw_filled_circle((GRID_WIDTH - 3.5) * CELL_SIZE, (GRID_HEIGHT - 3.5) * CELL_SIZE, CELL_SIZE / 4, MLV_COLOR_RED);

    draw_grid(grid);

    MLV_actualise_window();
    MLV_wait_seconds(5);
    MLV_free_window();

    return 0;
}
