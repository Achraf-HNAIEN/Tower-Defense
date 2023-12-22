#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define WIDTH 28
#define HEIGHT 22
#define PATH 1
#define EMPTY 0
#define MIN_TURNS 7
#define MIN_LENGTH 75
#define MAX_STEPS 4 

#define NORTH 0
#define EAST 1
#define SOUTH 2
#define WEST 3
#define MAX_STEPS 4

typedef struct {
    int x;
    int y;
} Point;


int isWithinBounds(int x, int y);
int manhattanDistance(int x1, int y1, int x2, int y2);
int tooCloseToPath(int x, int y, int startX, int startY, Point *path, int pathSize);
void printGrid(int grid[HEIGHT][WIDTH]);
void initializeGrid(int grid[HEIGHT][WIDTH]);
void chooseStartingPoint(int *x, int *y);
int calculateExtend(int x, int y, int direction, int startX, int startY, Point *path, int pathSize);
void addPathSegment(int *x, int *y, int direction, int steps, int grid[HEIGHT][WIDTH], int *length, Point **path, int *pathSize);
int chooseDirection(int x, int y, int startX, int startY, Point *path, int pathSize, int grid[HEIGHT][WIDTH]);

int isWithinBounds(int x, int y) {
    return x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT;
}


int manhattanDistance(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

// int tooCloseToPath(int x, int y, int startX, int startY, int grid[HEIGHT][WIDTH]) {
//     for (int dy = -2; dy <= 2; dy++) {
//         for (int dx = -2; dx <= 2; dx++) {
//             int newX = x + dx;
//             int newY = y + dy;

//             // Skip the starting point
//             if (newX == startX && newY == startY) {
//                 continue;
//             }

//             if (isWithinBounds(newX, newY) && grid[newY][newX] == PATH) {
//                 if (manhattanDistance(x, y, newX, newY) <= 2) {
//                     return 1;
//                 }
//             }
//         }
//     }
//     return 0;
// }

int tooCloseToPath(int x, int y, int startX, int startY, Point *path, int pathSize) {
    for (int i = 0; i < pathSize; i++) {
        int pathX = path[i].x;
        int pathY = path[i].y;

        // Skip the current position and the starting point
        if ((pathX == x && pathY == y) || (pathX == startX && pathY == startY)) {
            continue;
        }

        if (manhattanDistance(x, y, pathX, pathY) <= 1) {
            return 1;  // Too close to the path
        }
    }
    return 0;  // Not close to any path points
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
    }
    printf("Direction %d, Extend: %d\n", direction, extend - 1); // Debug
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

// int chooseDirection(int x, int y,int startX,int startY, int grid[HEIGHT][WIDTH]) {
//     int totalExtend = 0;
//     int directionExtend[4] = {0};

//     for (int dir = 0; dir < 4; dir++) {
//     directionExtend[dir] = calculateExtend(x, y, dir, startX, startY, path, pathSize);
//         totalExtend += directionExtend[dir];
//     }

//     if (totalExtend == 0) {
//         return -1; // No valid direction
//     }

//     int randomPick = rand() % totalExtend;
//     for (int dir = 0; dir < 4; dir++) {
//         if (randomPick < directionExtend[dir]) {
//             return dir;
//         }
//         randomPick -= directionExtend[dir];
//     }

//     return -1;
// }
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
    return (steps < 3) ? 3 : steps; 
}


int main() {
    int grid[HEIGHT][WIDTH];
    Point *path = NULL;
    int pathSize = 0;
    int x, y, length = 0, turns = 0;
    srand(time(NULL));

    do {
        initializeGrid(grid);
        length = turns = 0;

        chooseStartingPoint(&x, &y);
        printf("Starting Point: x=%d, y=%d\n", x, y);  // Debug
        grid[y][x] = PATH;
        length++;

        int currentDirection, lastDirection = -1, extend, steps;
        while (length < MIN_LENGTH || turns < MIN_TURNS) {
            currentDirection = chooseDirection(x, y, x, y, path, pathSize, grid);
            printf("Chosen Direction: %d\n", currentDirection);  // Debug
            if (currentDirection == -1) {
                printf("No valid direction found. Breaking out of loop.\n");  // Debug
                break;
            }

            extend = calculateExtend(x, y, currentDirection, x, y, path, pathSize);
            printf("Extend in chosen direction: %d\n", extend);  // Debug
            if (extend < 3) {
                printf("Extend too short. Breaking out of loop.\n");  // Debug
                break;
            }

            steps = chooseSteps(extend);
            printf("Steps to move: %d\n", steps);  // Debug
            addPathSegment(&x, &y, currentDirection, steps, grid, &length, &path, &pathSize);
            if (lastDirection != -1 && lastDirection != currentDirection) {
                turns++;
            }
            printf("Current Length: %d, Current Turns: %d\n", length, turns);  // Debug
            lastDirection = currentDirection;
        }

    } while (length < MIN_LENGTH || turns < MIN_TURNS);
    for (int i = 0; i < pathSize; i++) {
        printf("Path Point %d: (%d, %d)\n", i, path[i].x, path[i].y);
    }
    printGrid(grid);
    free(path);  

    return 0;
}
