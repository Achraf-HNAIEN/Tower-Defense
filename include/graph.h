#ifndef GRAPH_H
#define GRAPH_H

#include <MLV/MLV_all.h>
#include "grid.h"
#include "monstre.h"
#include "game.h"
#include "tower.h"

#define WIDTH 28     
#define HEIGHT 22    
#define PATH 1       
#define EMPTY 0
#define HEALTH_BAR_WIDTH 20
#define HEALTH_BAR_HEIGHT 3
#define MONSTRE_SIZE (CELL_SIZE / 4)


/**
 * Checks if the given mouse coordinates are inside a specified rectangle.
 *
 * @param mouse_x The x-coordinate of the mouse.
 * @param mouse_y The y-coordinate of the mouse.
 * @param x The x-coordinate of the top-left corner of the rectangle.
 * @param y The y-coordinate of the top-left corner of the rectangle.
 * @param width The width of the rectangle.
 * @param height The height of the rectangle.
 * @return 1 if the mouse is inside the rectangle, 0 otherwise.
 */
int is_click_inside(int mouse_x, int mouse_y, int x, int y, int width, int height);

/**
 * Draws all the game elements on the screen.
 *
 * @param game The game object.
 * @param headWave The head of the wave linked list.
 * @param deltaTime The time elapsed since the last frame.
 */
void drawAll(Game *game, Wave *headWave, float deltaTime);

/**
 * Updates the position of the projectiles in the game.
 *
 * @param game The game object.
 * @param deltaTime The time elapsed since the last frame.
 */
void updateProjectilePosition(Game *game, float deltaTime);

/**
 * Draws the projectiles on the screen.
 *
 * @param game The game object.
 */
void drawProjectiles(Game *game);

/**
 * Cleans up the memory allocated for the projectiles.
 *
 * @param game The game object.
 */
void cleanupProjectiles(Game *game);

/**
 * Prints debug information about the projectiles.
 *
 * @param game The game object.
 */
void debugPrintProjectiles(Game *game);

/**
 * Converts a hue value to an RGB color.
 *
 * @param hue The hue value.
 * @return The corresponding RGB color.
 */
MLV_Color hueToRGB(int hue);

#endif
