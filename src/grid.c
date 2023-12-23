#include <stdio.h>
#include <stdlib.h>
#include <time.h>
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
            return extend - 1;
        }
        if (newX < 2 || newX >= WIDTH - 2 || newY < 2 || newY >= HEIGHT - 2) {
            return extend - 1;
        }
    }
    return extend - 1; 
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


int chooseDirection(int x, int y, int startX, int startY, Point *path, int pathSize, int grid[HEIGHT][WIDTH]) {
    int totalExtend = 0;
    int directionExtend[4] = {0};

    for (int dir = 0; dir < 4; dir++) {
        directionExtend[dir] = calculateExtend(x, y, dir, startX, startY, path, pathSize);
        totalExtend += directionExtend[dir];
    }

    if (totalExtend == 0) {
        return -1; // No valid direction
    }

    int randomPick = rand() % totalExtend;
    for (int dir = 0; dir < 4; dir++) {
        if (randomPick < directionExtend[dir]) {
            return dir;
        }
        randomPick -= directionExtend[dir];
    }

    return -1;
}

int chooseSteps(int extend) {
    int steps = 0;
    for (int i = 0; i < extend; i++) {
        if ((rand() % 4) < 3) { // 3/4 probability
            steps++;
        }
    }
    return steps >= 3 ? steps : 3; // Ensure at least 3 steps
}


// int main() {
//     int grid[HEIGHT][WIDTH];
//     Point *path = NULL;
//     int pathSize = 0;
//     int x, y, length = 0, turns = 0;
//     srand(time(NULL));

//     do {
//         initializeGrid(grid);
//         length = turns = 0;

//         chooseStartingPoint(&x, &y);
//         grid[y][x] = PATH;
//         length++;
//         path = realloc(path, sizeof(Point));
//         path[0] = (Point){x, y};
//         pathSize = 1;

//         int currentDirection, lastDirection = -1, extend, steps;
//         while (length < MIN_LENGTH || turns < MIN_TURNS) {
//             currentDirection = chooseDirection(x, y, x, y, path, pathSize, grid);
//             if (currentDirection == -1) break;

//             extend = calculateExtend(x, y, currentDirection, x, y, path, pathSize);
//             if (extend < 3) break;

//             steps = chooseSteps(extend);
//             addPathSegment(&x, &y, currentDirection, steps, grid, &length, &path, &pathSize);

//             if (lastDirection != -1 && lastDirection != currentDirection) turns++;
//             lastDirection = currentDirection;

//             // Recalculate the extend for the next move (90-degree turns)
//             int newDirections[2];
//             newDirections[0] = (currentDirection + 1) % 4; // Turn right 90 degrees
//             newDirections[1] = (currentDirection + 3) % 4; // Turn left 90 degrees

//             int bestDirection = -1, maxExtend = 0;
//             for (int i = 0; i < 2; i++) {
//                 int dirExtend = calculateExtend(x, y, newDirections[i], x, y, path, pathSize);
//                 if (dirExtend > maxExtend) {
//                     maxExtend = dirExtend;
//                     bestDirection = newDirections[i];
//                 }
//             }

//             if (maxExtend < 3 || bestDirection == -1) break;
//             currentDirection = bestDirection;
//         }

//     } while (length < MIN_LENGTH || turns < MIN_TURNS);

//     printGrid(grid);

//     // Clean up
//     if (path != NULL) {
//         free(path);
//     }

//     return 0;
// }

void generatePath(int grid[HEIGHT][WIDTH], Point **path, int *pathSize) {
    int x, y, length = 0, turns = 0;
    srand(time(NULL));

    do {
        initializeGrid(grid);
        length = turns = 0;

        chooseStartingPoint(&x, &y);
        grid[y][x] = PATH;
        length++;
        *path = realloc(*path, sizeof(Point));
        (*path)[0] = (Point){x, y};
        *pathSize = 1;

        int currentDirection, lastDirection = -1, extend, steps;
        while (length < MIN_LENGTH || turns < MIN_TURNS) {
            currentDirection = chooseDirection(x, y, x, y, *path, *pathSize, grid);
            if (currentDirection == -1) break;

            extend = calculateExtend(x, y, currentDirection, x, y, *path, *pathSize);
            if (extend < 3) break;

            steps = chooseSteps(extend);
            addPathSegment(&x, &y, currentDirection, steps, grid, &length, path, pathSize);

            if (lastDirection != -1 && lastDirection != currentDirection) {
                turns++;
            }
            lastDirection = currentDirection;

            int newDirections[2];
            newDirections[0] = (currentDirection + 1) % 4; // Turn right
            newDirections[1] = (currentDirection + 3) % 4; // Turn left

            int bestDirection = -1, maxExtend = 0;
            for (int i = 0; i < 2; i++) {
                int dirExtend = calculateExtend(x, y, newDirections[i], x, y, *path, *pathSize);
                if (dirExtend > maxExtend) {
                    maxExtend = dirExtend;
                    bestDirection = newDirections[i];
                }
            }

            if (maxExtend < 3 || bestDirection == -1) break;
            currentDirection = bestDirection;
        }

    } while (length < MIN_LENGTH || turns < MIN_TURNS);
}
