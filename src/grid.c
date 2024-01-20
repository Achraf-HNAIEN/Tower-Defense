#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include "grid.h"

int isWithinBounds(int x, int y) {
    return x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT;
}


static int manhattan_Distance(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}


static int too_Close_To_Path(int x, int y, int startX, int startY, Point *path, int pathSize) {
    for (int i = 0; i < pathSize; i++) {
        int pathX = path[i].x;
        int pathY = path[i].y;

        if ((pathX == x && pathY == y) || (pathX == startX && pathY == startY)) {
            continue;
        }
        if (manhattan_Distance(x, y, pathX, pathY) <= 1) {
            return 1;  
        }

    }
    return 0;  
}

static void initialize_Grid(short grid[HEIGHT][WIDTH]) {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            grid[y][x] = EMPTY;
        }
    }
}
static void choose_Starting_Point(int *x, int *y) {
    *x = rand() % (WIDTH - 6) + 3;  
    *y = rand() % (HEIGHT - 6) + 3; 
}


static int calculate_Extend(int x, int y, int direction, int startX, int startY, Point *path, int pathSize) {
    int extend = 0;
    int dx = 0, dy = 0;

    switch(direction) {
        case NORTH: dy = -1; break;
        case EAST: dx = 1; break;
        case SOUTH: dy = 1; break;
        case WEST: dx = -1; break;
    }

    for (extend = 1; extend <= MAX_STEPS; extend++) {
        int newX = x + dx * extend;
        int newY = y + dy * extend;

        if (!isWithinBounds(newX, newY) || too_Close_To_Path(newX, newY, startX, startY, path, pathSize)) {
            return extend - 2;
        }
        if (newX < 2 || newX >= WIDTH - 2 || newY < 2 || newY >= HEIGHT - 2) {
            return extend - 2;
        }
    }
    return extend - 2; 
}


static void add_Path_Segment(int *x, int *y, int direction, int steps, short grid[HEIGHT][WIDTH], int *length, Point **path, short *pathSize) {
    int dx = 0, dy = 0;

    switch(direction) {
        case NORTH: dy = -1; break;
        case EAST: dx = 1; break;
        case SOUTH: dy = 1; break;
        case WEST: dx = -1; break;
    }

     for (int i = 0; i < steps; i++) {
        *x += dx;
        *y += dy;

        if (isWithinBounds(*x, *y)) {
            grid[*y][*x] = PATH;
            (*length)++;

            *path = realloc(*path, (*pathSize + 1) * sizeof(Point));
            (*path)[*pathSize].x = *x;
            (*path)[*pathSize].y = *y;
            (*pathSize)++;
        } else {
            break;
        }
    }
}


// Adjust choose_Steps to follow the '3/4' probability logic
static int choose_Steps(int extend) {
    int steps = 0;
    for (int i = 0; i < extend; i++) {
        if ((rand() % 4) < 3) {
            steps++;
        }
    }
    return MAX(steps, 3);
}

static int choose_New_Direction(int x, int y, int currentDirection, Point *path, int pathSize) {
    int leftDirection = (currentDirection + 3) % 4;  // Turn left
    int rightDirection = (currentDirection + 1) % 4; // Turn right

    int leftExtend = calculate_Extend(x, y, leftDirection, x, y, path, pathSize);
    int rightExtend = calculate_Extend(x, y, rightDirection, x, y, path, pathSize);
    int currentExtend = calculate_Extend(x, y, currentDirection, x, y, path, pathSize);

    // Calculate total extend for probability calculation
    int totalExtend = leftExtend + rightExtend + currentExtend;

    if (totalExtend == 0) {
        return -1; // No valid direction
    }

    // Choose direction based on extend
    int randomPick = rand() % totalExtend;
    if (randomPick < leftExtend) {
        return leftDirection;
    } else if (randomPick < leftExtend + rightExtend) {
        return rightDirection;
    } else {
        return currentDirection;
    }
}

float distanceBetweenPoints(Point p1, Point p2) {
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

void generatePath(short grid[HEIGHT][WIDTH], Point **path, short *pathSize) {
    srand(time(NULL));
    int attempts = 0;

    while (attempts++ < MAX_ATTEMPTS) {
        initialize_Grid(grid);
        int x, y, length = 0, turns = 0;
        choose_Starting_Point(&x, &y);
        
        // Other variables for path generation
        int currentDirection = -1;
        int lastDirection = -1;
        int extend = 0;
        int steps = 0;
        
        while (1) {
            extend = calculate_Extend(x, y, currentDirection, x, y, *path, *pathSize);
            if (extend > 2) {
                steps = choose_Steps(extend);
                add_Path_Segment(&x, &y, currentDirection, steps, grid, &length, path, pathSize);
                
                // Count turns
                if (lastDirection != -1 && lastDirection != currentDirection) {
                    turns++;
                }
                lastDirection = currentDirection;

                // Calculate new direction
                currentDirection = choose_New_Direction(x, y, currentDirection, *path, *pathSize);
            } else {
                break;
            }

            if (length >= MIN_LENGTH && turns >= MIN_TURNS) {
                return;  // Valid path found
            }
        }

        free(*path);
        *path = NULL;
        *pathSize = 0;
    }

    printf("Failed to generate a valid path after %d attempts\n", MAX_ATTEMPTS);
}