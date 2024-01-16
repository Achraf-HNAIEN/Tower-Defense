#ifndef GRID_H
#define GRID_H

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
#define MAX_ATTEMPTS 100000
#define MAX(a, b)       ((a) < (b) ? (b) : (a))


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
float distanceBetweenPoints(Point p1, Point p2);
void addPathSegment(int *x, int *y, int direction, int steps, int grid[HEIGHT][WIDTH], int *length, Point **path, int *pathSize);
int validatePath(int grid[HEIGHT][WIDTH], Point *path, int pathSize);
int chooseNewDirection(int x, int y, int currentDirection, Point *path, int pathSize);
void generatePath(int grid[HEIGHT][WIDTH], Point **path, int *pathSize);


#endif 