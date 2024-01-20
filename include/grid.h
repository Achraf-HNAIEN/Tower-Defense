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


/**
 * Checks if the given coordinates (x, y) are within the bounds of the grid.
 *
 * @param x The x-coordinate.
 * @param y The y-coordinate.
 * @return 1 if the coordinates are within bounds, 0 otherwise.
 */
int isWithinBounds(int x, int y);

/**
 * Calculates the distance between two points in the grid.
 *
 * @param p1 The first point.
 * @param p2 The second point.
 * @return The distance between the two points.
 */
float distanceBetweenPoints(Point p1, Point p2);

/**
 * Generates a path through the grid using a given grid, starting point, and ending point.
 *
 * @param grid      The grid.
 * @param path      A pointer to the generated path.
 * @param pathSize  A pointer to the size of the generated path.
 */
void generatePath(short grid[HEIGHT][WIDTH], Point **path, short *pathSize);


#endif 