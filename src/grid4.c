#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "MLV/MLV_all.h"

#define GRID_WIDTH 28
#define GRID_HEIGHT 22
#define CELL_SIZE 30
#define MIN_DISTANCE_TO_EDGE 3
#define MIN_PATH_LENGTH 75
#define MIN_TURNS 7
#define PROBABILITY 3 // 3/4 probability

int calculate_manhattan_distance(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

int is_valid_cell(int x, int y) {
    return x >= 0 && x < GRID_WIDTH && y >= 0 && y < GRID_HEIGHT;
}
int calculate_extent(int grid[GRID_WIDTH][GRID_HEIGHT], int x, int y, int dx, int dy) {
    int extent = 0;
    int nx = x + dx;
    int ny = y + dy;
    while (is_valid_cell(nx, ny) && grid[nx][ny] == 0) {
        int too_close = 0;
        for (int i = 0; i < GRID_WIDTH; i++) {
            for (int j = 0; j < GRID_HEIGHT; j++) {
                if (grid[i][j] == 1 && calculate_manhattan_distance(nx, ny, i, j) <= 2) {
                    too_close = 1;
                    break;
                }
            }
            if (too_close) break;
        }
        if (too_close) break;
        extent++;
        nx += dx;
        ny += dy;
    }
    return extent;
}
void initialize_grid(int grid[GRID_WIDTH][GRID_HEIGHT]) {
    for (int i = 0; i < GRID_WIDTH; i++) {
        for (int j = 0; j < GRID_HEIGHT; j++) {
            grid[i][j] = 0;
        }
    }
}


// int calculate_extent(int grid[GRID_WIDTH][GRID_HEIGHT], int x, int y, int dx, int dy) {
//     int extent = 0;
//     x += dx;
//     y += dy;
//     while (is_valid_cell(x, y) && grid[x][y] == 0) {
//         extent++;
//         x += dx;
//         y += dy;
//     }
//     return extent;
// }

int choose_direction(int extents[4]) {
    int total_extent = 0;
    for (int i = 0; i < 4; i++) {
        total_extent += extents[i];
    }

    if (total_extent == 0) {
        return -1; // No valid direction
    }

    int random_value = rand() % total_extent;
    for (int i = 0; i < 4; i++) {
        if (random_value < extents[i]) {
            return i;
        }
        random_value -= extents[i];
    }
    return -1;
}

int generate_segment_length(int extent) {
    int sum = 0;
    for (int i = 0; i < extent; i++) {
        if (rand() % 4 < PROBABILITY) { // 3/4 probability
            sum += 1;
        }
    }
    return sum >= 3 ? sum : 3;
}


// void add_path_segment(int grid[GRID_WIDTH][GRID_HEIGHT], int path[][2], int *path_length, int *x, int *y, int direction, int length) {
//     int dx = 0, dy = 0;
//     switch (direction) {
//         case 0: dy = -1; break; // North
//         case 1: dx = 1; break;  // East
//         case 2: dy = 1; break;  // South
//         case 3: dx = -1; break; // West
//     }

//     for (int i = 0; i < length; i++) {
//         if (is_valid_cell(*x + dx, *y + dy)) {
//             *x += dx;
//             *y += dy;
//             grid[*x][*y] = 1;
//             if (*path_length < MIN_PATH_LENGTH) {
//                 path[*path_length][0] = *x;
//                 path[*path_length][1] = *y;
//                 (*path_length)++;
//             }
//         } else {
//             break;
//         }
//     }
// }

void add_path_segment(int grid[GRID_WIDTH][GRID_HEIGHT], int *x, int *y, int direction, int length) {
    int dx = 0, dy = 0;
    switch (direction) {
        case 0: dy = -1; break; // North
        case 1: dx = 1; break;  // East
        case 2: dy = 1; break;  // South
        case 3: dx = -1; break; // West
    }

    for (int i = 0; i < length; i++) {
        if (is_valid_cell(*x + dx, *y + dy)) {
            *x += dx;
            *y += dy;
            grid[*x][*y] = 1;
        } else {
            break;
        }
    }
}


int count_turns(int path[MIN_PATH_LENGTH][2], int path_length) {
    int turns = 0;
    for (int i = 2; i < path_length; i++) {
        int dx1 = path[i - 1][0] - path[i - 2][0];
        int dy1 = path[i - 1][1] - path[i - 2][1];
        int dx2 = path[i][0] - path[i - 1][0];
        int dy2 = path[i][1] - path[i - 1][1];
        if (dx1 != dx2 || dy1 != dy2) {
            turns++;
        }
    }
    return turns;
}
void draw_grid() {
    MLV_clear_window(MLV_COLOR_BLACK);

    // Draw vertical lines
    for (int i = 0; i <= GRID_WIDTH; i++) {
        MLV_draw_line(i * CELL_SIZE, 0, i * CELL_SIZE, GRID_HEIGHT * CELL_SIZE, MLV_COLOR_WHITE);
    }

    // Draw horizontal lines
    for (int j = 0; j <= GRID_HEIGHT; j++) {
        MLV_draw_line(0, j * CELL_SIZE, GRID_WIDTH * CELL_SIZE, j * CELL_SIZE, MLV_COLOR_WHITE);
    }

    MLV_actualise_window();
}
void draw_path(int grid[GRID_WIDTH][GRID_HEIGHT]) {
    for (int i = 0; i < GRID_WIDTH; i++) {
        for (int j = 0; j < GRID_HEIGHT; j++) {
            if (grid[i][j] == 1) { // Assuming 1 represents a cell that is part of the path
                MLV_draw_filled_rectangle(i * CELL_SIZE, j * CELL_SIZE, CELL_SIZE, CELL_SIZE, MLV_COLOR_GREEN);
            }
        }
    }

    MLV_actualise_window();
}

void print_grid(int grid[GRID_WIDTH][GRID_HEIGHT]) {
    for (int i = 0; i < GRID_WIDTH; i++) {
        for (int j = 0; j < GRID_HEIGHT; j++) {
            printf("%d ", grid[i][j]);
        }
        printf("\n");
    }
}

void print_path(int path[][2], int path_length) {
    printf("Path: ");
    for (int i = 0; i < path_length; i++) {
        printf("(%d,%d) ", path[i][0], path[i][1]);
    }
    printf("\n");
}

int main() {
    srand(time(NULL));

    int grid[GRID_WIDTH][GRID_HEIGHT];
    int path[MIN_PATH_LENGTH][2];
    int path_length = 0;
    int turns = 0;
    int successful_path = 0;

    do {
        initialize_grid(grid);
        // Choose a random starting point
        int x = rand() % (GRID_WIDTH - 2 * MIN_DISTANCE_TO_EDGE) + MIN_DISTANCE_TO_EDGE;
        int y = rand() % (GRID_HEIGHT - 2 * MIN_DISTANCE_TO_EDGE) + MIN_DISTANCE_TO_EDGE;
        grid[x][y] = 1;
        path[path_length][0] = x;
        path[path_length][1] = y;
        path_length++;
        turns = 0;

        int last_dx = 0, last_dy = 0;
        while (turns < MIN_TURNS || path_length < MIN_PATH_LENGTH) {
            int extents[4] = {
                calculate_extent(grid, x, y, 0, -1), // North
                calculate_extent(grid, x, y, 1, 0),  // East
                calculate_extent(grid, x, y, 0, 1),  // South
                calculate_extent(grid, x, y, -1, 0)  // West
            };

            int direction = choose_direction(extents);
            if (direction == -1) break;

            int dx = 0, dy = 0;
            switch (direction) {
                case 0: dy = -1; break; // North
                case 1: dx = 1; break;  // East
                case 2: dy = 1; break;  // South
                case 3: dx = -1; break; // West
            }
            if (dx != last_dx || dy != last_dy) {
                turns++;
                last_dx = dx;
                last_dy = dy;
            }

            int segment_length = generate_segment_length(extents[direction]);
            add_path_segment(grid, &x, &y, direction, segment_length);
            path_length += segment_length;
        }

        if (turns >= MIN_TURNS && path_length >= MIN_PATH_LENGTH) {
            successful_path = 1;
        }

    } while (!successful_path);

    // draw_grid();
    // draw_path(grid);
    print_grid(grid);
    print_path(path, path_length);


    return 0;
}
// int main() {
//     srand(time(NULL));

//     int grid[GRID_WIDTH][GRID_HEIGHT];
//     int path[MIN_PATH_LENGTH][2];
//     int path_length, path_valid;

//     do {
//         initialize_grid(grid);
//         print_grid(grid);  
//         path_length = 0;
//         path_valid = 0;

//         // Choose a random starting point
//         int x = rand() % (GRID_WIDTH - 2 * MIN_DISTANCE_TO_EDGE) + MIN_DISTANCE_TO_EDGE;
//         int y = rand() % (GRID_HEIGHT - 2 * MIN_DISTANCE_TO_EDGE) + MIN_DISTANCE_TO_EDGE;
//         grid[x][y] = 1;
//         path[path_length][0] = x;
//         path[path_length][1] = y;
//         path_length++;

//         while (path_length < MIN_PATH_LENGTH) {
//             int extents[4] = {
//                 calculate_extent(grid, x, y, 0, -1), // North
//                 calculate_extent(grid, x, y, 1, 0),  // East
//                 calculate_extent(grid, x, y, 0, 1),  // South
//                 calculate_extent(grid, x, y, -1, 0)  // West
//             };

//             int direction = choose_direction(extents);
//             printf("Chosen direction: %d\n", direction);

//             if (direction == -1 || extents[direction] <= 2) break;

//             int segment_length = generate_segment_length(extents[direction]);
//             add_path_segment(grid, path, &path_length, &x, &y, direction, segment_length);
//             print_grid(grid);  // Print the grid after adding a path segment
//             print_path(path, path_length);  // Print the current path
//             for (int i = 0; i < segment_length && path_length < MIN_PATH_LENGTH; i++) {
//                 path[path_length][0] = x;
//                 path[path_length][1] = y;
//                 path_length++;
//             }
//             printf("Extents: North=%d, East=%d, South=%d, West=%d\n", extents[0], extents[1], extents[2], extents[3]);


//             if (count_turns(path, path_length) >= MIN_TURNS) {
//                 path_valid = 1;
//                 break;
//             }
//         }
//     } while (!path_valid);



//     return 0;

// }
// void initialize_grid(int grid[GRID_WIDTH][GRID_HEIGHT]) {
//     for (int i = 0; i < GRID_WIDTH; i++) {
//         for (int j = 0; j < GRID_HEIGHT; j++) {
//             grid[i][j] = 0;
//         }
//     }
// }

// int is_valid_cell(int x, int y) {
//     return x >= 0 && x < GRID_WIDTH && y >= 0 && y < GRID_HEIGHT;
// }

// int calculate_extent(int grid[GRID_WIDTH][GRID_HEIGHT], int x, int y, int dx, int dy) {
//     int extent = 0;
//     x += dx;
//     y += dy;
//     while (is_valid_cell(x, y) && grid[x][y] == 0) {
//         // Check surrounding cells to ensure we don't come within 2 cells of the path
//         for (int i = -2; i <= 2; i++) {
//             for (int j = -2; j <= 2; j++) {
//                 if (is_valid_cell(x + i, y + j) && grid[x + i][y + j] == 1) {
//                     return extent;
//                 }
//             }
//         }
//         extent++;
//         x += dx;
//         y += dy;
//     }
//     return extent;
// }

// int choose_direction(int extents[4]) {
//     int total_extent = 0;
//     for (int i = 0; i < 4; i++) {
//         total_extent += extents[i];
//     }

//     if (total_extent == 0) {
//         return -1; // No valid direction
//     }

//     int random_value = rand() % total_extent;
//     for (int i = 0; i < 4; i++) {
//         if (random_value < extents[i]) {
//             return i;
//         }
//         random_value -= extents[i];
//     }
//     return -1;
// }

// void add_path_segment(int grid[GRID_WIDTH][GRID_HEIGHT], int *x, int *y, int direction, int length) {
//     int dx = 0, dy = 0;
//     switch (direction) {
//         case 0: dy = -1; break; // North
//         case 1: dx = 1; break;  // East
//         case 2: dy = 1; break;  // South
//         case 3: dx = -1; break; // West
//     }

//     for (int i = 0; i < length; i++) {
//         if (is_valid_cell(*x + dx, *y + dy)) {
//             *x += dx;
//             *y += dy;
//             grid[*x][*y] = 1;
//         } else {
//             break;
//         }
//     }
// }

// int count_turns(int path[MIN_PATH_LENGTH][2], int path_length) {
//     int turns = 0;
//     for (int i = 2; i < path_length; i++) {
//         int dx1 = path[i - 1][0] - path[i - 2][0];
//         int dy1 = path[i - 1][1] - path[i - 2][1];
//         int dx2 = path[i][0] - path[i - 1][0];
//         int dy2 = path[i][1] - path[i - 1][1];
//         if (dx1 != dx2 || dy1 != dy2) {
//             turns++;
//         }
//     }
//     return turns;
// }

// void draw_grid() {
//     MLV_clear_window(MLV_COLOR_BLACK);

//     // Draw vertical lines
//     for (int i = 0; i <= GRID_WIDTH; i++) {
//         MLV_draw_line(i * CELL_SIZE, 0, i * CELL_SIZE, GRID_HEIGHT * CELL_SIZE, MLV_COLOR_WHITE);
//     }

//     // Draw horizontal lines
//     for (int j = 0; j <= GRID_HEIGHT; j++) {
//         MLV_draw_line(0, j * CELL_SIZE, GRID_WIDTH * CELL_SIZE, j * CELL_SIZE, MLV_COLOR_WHITE);
//     }

//     MLV_actualise_window();
// }
// void draw_path(int grid[GRID_WIDTH][GRID_HEIGHT]) {
//     for (int i = 0; i < GRID_WIDTH; i++) {
//         for (int j = 0; j < GRID_HEIGHT; j++) {
//             if (grid[i][j] == 1) { // Assuming 1 represents a cell that is part of the path
//                 MLV_draw_filled_rectangle(i * CELL_SIZE, j * CELL_SIZE, CELL_SIZE, CELL_SIZE, MLV_COLOR_GREEN);
//             }
//         }
//     }

//     MLV_actualise_window();
// }



