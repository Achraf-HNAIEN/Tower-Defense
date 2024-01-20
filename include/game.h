#ifndef __GAME__
#define __GAME__


#include "grid.h"
#include "tower.h"
#include "monstre.h"
#include "projectile.h"


#define CELL_SIZE 25 
#define GEMME_RANGE 3

typedef struct _Wave{
    Monster *monsters;
    int Nb_Monsters;
    struct _Wave *next;
}Wave;

typedef struct{
  int wave;
  int mana;
  int mana_max;
  short pathSize;
  short quit;
  short grid[HEIGHT][WIDTH];
  short nb_tower;
  short level_mana;
  short level_gemme_in_shop;
  short want_to_place_tower; //if the player is in tower placement mode 0 by default and 1 if he is in tower placement mode
  short tower_count;
  short next_wave_time;
  short inventory_size;
  short has_start;
  short numVisualProjectiles;
  short gemme_selected; //-1 mean no gemme selected, [0-5] inventory index / 6 mean slot fusion 1 / 7 mean slot fusion 2
  Gemme * fusion_slot1; // NULL mean no gemme in fusion slot 1
  Gemme * fusion_slot2; // NULL mean no gemme in fusion slot 2
  Gemme * inventaire[6];
  Wave *wavesHead;
  Point *path;
  Tower towers[MAX_TOWERS]; 
  VisualProjectile visualProjectiles[MAX_PROJECTILES];
  }Game;


/**
 * Checks if a given point (mouse_x, mouse_y) is inside a rectangle defined by its top-left corner (x, y),
 * width, and height.
 *
 * @param mouse_x The x-coordinate of the point to check.
 * @param mouse_y The y-coordinate of the point to check.
 * @param x The x-coordinate of the top-left corner of the rectangle.
 * @param y The y-coordinate of the top-left corner of the rectangle.
 * @param width The width of the rectangle.
 * @param height The height of the rectangle.
 * @return 1 if the point is inside the rectangle, 0 otherwise.
 */
extern int is_click_inside(int mouse_x, int mouse_y, int x, int y, int width, int height);

/**
 * Handles the click event on the inventory.
 * 
 * @param m_x The x-coordinate of the mouse click.
 * @param m_y The y-coordinate of the mouse click.
 * @param game A pointer to the Game struct.
 */
void handle_inventory_click(int m_x, int m_y, Game * game);

/**
 * Handles the left click event.
 * 
 * @param mouse_x The x-coordinate of the mouse click.
 * @param mouse_y The y-coordinate of the mouse click.
 * @param game A pointer to the Game struct.
 */
void handle_left_click(int mouse_x, int mouse_y, Game *game);

/**
 * Updates the game state based on the elapsed time, mouse position, and other parameters.
 *
 * @param game The game object to update.
 * @param deltaTime The time elapsed since the last update.
 * @param mouse_x The x-coordinate of the mouse position.
 * @param mouse_y The y-coordinate of the mouse position.
 */
void update_game(Game *game, float deltaTime, int mouse_x, int mouse_y);

/**
 * Handles the logic for starting a new wave in the game.
 *
 * @param game The game object.
 * @param currentTime The current time in milliseconds.
 * @param last_wave_time A pointer to the variable storing the time of the last wave.
 */
void handle_new_wave(Game *game, int currentTime, int *last_wave_time);

/**
 * Moves the monsters along the specified path based on the elapsed time and game state.
 *
 * @param monsters An array of Monster objects.
 * @param path An array of Point objects representing the path for the monsters.
 * @param pathSize The size of the path array.
 * @param deltaTime The time elapsed since the last update.
 * @param game The game object.
 * @return The number of monsters that have reached the end of the path.
 */
int moveMonsters(Monster monsters[], Point path[], int pathSize, float deltaTime, Game *game);

/**
 * Initializes a new wave with the specified wave number and path.
 *
 * @param waveNumber The number of the wave.
 * @param path An array of Point objects representing the path for the monsters.
 * @param pathSize The size of the path array.
 * @return A pointer to the initialized Wave object.
 */
Wave* initializeWave(int waveNumber, Point path[], int pathSize);

/**
 * Buys a gemme in the game.
 *
 * @param game The game object.
 */
void buy_gemme(Game * game);

/**
 * Places a tower at the specified position using the specified gemme.
 *
 * @param game The game object.
 * @param position The position where the tower should be placed.
 * @param gemme The gemme to use for the tower.
 */
void placeTower(Game *game, Point position, Gemme *gemme);

/*Add 'mana' to the player mana storage.*/
void add_mana(Game * game, int mana);

/*Try to upgrade mana storage of the player*/
/**
 * Upgrades the mana storage of the game.
 *
 * @param game The game object.
 */
void upgrade_mana_storage(Game *);

/**
 * Adds a visual projectile to the game.
 *
 * @param game The game object.
 * @param vp The visual projectile to add.
 */
void addVisualProjectile(Game *game, VisualProjectile vp);

/**
 * Tries to perform a fusion in the game.
 *
 * @param game The game object.
 */
void try_fusion(Game * game);

/**
 * Tries to place a gemme on a tower in the game.
 *
 * @param game The game object.
 * @param mouse_x The x-coordinate of the mouse.
 * @param mouse_y The y-coordinate of the mouse.
 */
void try_place_gemme_on_tower(Game *game, int mouse_x, int mouse_y);

/**
 * Tries to remove a gemme from a tower in the game.
 *
 * @param game The game object.
 * @param x The x-coordinate of the tower.
 * @param y The y-coordinate of the tower.
 */
void try_remove_gemme_on_tower(Game *, int x, int y);

/**
 * Checks if any wave in the game is dead and updates the game accordingly.
 *
 * @param game The game object.
 */
void check_wave_dead(Game * game);

/**
 * Updates the gemmes and shoots projectiles in the game.
 *
 * @param game The game object.
 * @param deltaTime The time elapsed since the last update.
 */
void UpdateGemmesAndShoot(Game *game, float deltaTime);

/**
 * @brief Frees the resources used by the game.
 * 
 * This function releases any dynamically allocated memory and resources used by the game.
 * It should be called when the game is no longer needed to prevent memory leaks.
 * 
 * @param game The game object to free resources for.
 */
void free_game_resources(Game * game);
#endif