#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include "grid.h"

int isWithinBounds(int x, int y) {
    return x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT;
}


int manhattanDistance(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}


int tooCloseToPath(int x, int y, int startX, int startY, Point *path, int pathSize) {
    for (int i = 0; i < pathSize; i++) {
        int pathX = path[i].x;
        int pathY = path[i].y;

        if ((pathX == x && pathY == y) || (pathX == startX && pathY == startY)) {
            continue;
        }
        if (manhattanDistance(x, y, pathX, pathY) <= 1) {
            return 1;  
        }

    }
    return 0;  
}

void printGrid(int grid[HEIGHT][WIDTH]) {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            printf("%d ", grid[y][x]);
        }
        printf("\n");
    }
}

void initializeGrid(int grid[HEIGHT][WIDTH]) {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            grid[y][x] = EMPTY;
        }
    }
}
void chooseStartingPoint(int *x, int *y) {
    *x = rand() % (WIDTH - 6) + 3;  
    *y = rand() % (HEIGHT - 6) + 3; 
    printf("Starting Point Chosen: (%d, %d)\n", *x, *y); // Debug
}


int calculateExtend(int x, int y, int direction, int startX, int startY, Point *path, int pathSize) {
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

        if (!isWithinBounds(newX, newY) || tooCloseToPath(newX, newY, startX, startY, path, pathSize)) {
            return extend - 2;
        }
        if (newX < 2 || newX >= WIDTH - 2 || newY < 2 || newY >= HEIGHT - 2) {
            return extend - 2;
        }
    }
    return extend - 2; 
}


void addPathSegment(int *x, int *y, int direction, int steps, int grid[HEIGHT][WIDTH], int *length, Point **path, int *pathSize) {
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


// Adjust chooseSteps to follow the '3/4' probability logic
int chooseSteps(int extend) {
    int steps = 0;
    for (int i = 0; i < extend; i++) {
        if ((rand() % 4) < 3) {
            steps++;
        }
    }
    return MAX(steps, 3);
}

int validatePath(int grid[HEIGHT][WIDTH], Point *path, int pathSize) {
    // Check for minimum path length
    if (pathSize < MIN_LENGTH) {
        return 0;
    }

    // Check for minimum turns
    int turns = 0;
    for (int i = 2; i < pathSize; i++) {
        int dx1 = path[i - 1].x - path[i - 2].x;
        int dy1 = path[i - 1].y - path[i - 2].y;
        int dx2 = path[i].x - path[i - 1].x;
        int dy2 = path[i].y - path[i - 1].y;

        // Check if there's a change in direction
        if (dx1 != dx2 || dy1 != dy2) {
            turns++;
        }
    }
    if (turns < MIN_TURNS) {
        return 0;
    }

    // Check for minimum distance between path segments
    for (int i = 0; i < pathSize; i++) {
        for (int j = i + 2; j < pathSize; j++) {  // Start from i + 2 to avoid immediate adjacent points
            if (manhattanDistance(path[i].x, path[i].y, path[j].x, path[j].y) < 3) {
                return 0;
            }
        }
    }

    return 1;  // Path is valid
}
int chooseNewDirection(int x, int y, int currentDirection, Point *path, int pathSize) {
    int leftDirection = (currentDirection + 3) % 4;  // Turn left
    int rightDirection = (currentDirection + 1) % 4; // Turn right

    int leftExtend = calculateExtend(x, y, leftDirection, x, y, path, pathSize);
    int rightExtend = calculateExtend(x, y, rightDirection, x, y, path, pathSize);
    int currentExtend = calculateExtend(x, y, currentDirection, x, y, path, pathSize);

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

void generatePath(int grid[HEIGHT][WIDTH], Point **path, int *pathSize) {
    srand(time(NULL));
    int attempts = 0;

    while (attempts++ < MAX_ATTEMPTS) {
        initializeGrid(grid);
        int x, y, length = 0, turns = 0;
        chooseStartingPoint(&x, &y);
        
        // Other variables for path generation
        int currentDirection = -1;
        int lastDirection = -1;
        int extend = 0;
        int steps = 0;
        
        while (1) {
            extend = calculateExtend(x, y, currentDirection, x, y, *path, *pathSize);
            if (extend > 2) {
                steps = chooseSteps(extend);
                addPathSegment(&x, &y, currentDirection, steps, grid, &length, path, pathSize);
                
                // Count turns
                if (lastDirection != -1 && lastDirection != currentDirection) {
                    turns++;
                }
                lastDirection = currentDirection;

                // Calculate new direction
                currentDirection = chooseNewDirection(x, y, currentDirection, *path, *pathSize);
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