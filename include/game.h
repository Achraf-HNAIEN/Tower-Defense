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
  int pathSize;
  int quit;
  int grid[HEIGHT][WIDTH];
  int nb_tower;
  int level_mana;
  int level_gemme_in_shop;
  Point *path;
  int want_to_place_tower; //if the player is in tower placement mode 0 by default and 1 if he is in tower placement mode
  Tower towers[MAX_TOWERS]; 
  int tower_count;
  Wave *waves;
  Wave *wavesHead;
  int wave_count;
  Gemme * inventaire[6];
  short inventory_size;
  short has_start;
  int next_wave_time;
  short gemme_selected; //-1 mean no gemme selected, [0-5] inventory index / 6 mean slot fusion 1 / 7 mean slot fusion 2
  VisualProjectile visualProjectiles[MAX_PROJECTILES];
  short numVisualProjectiles;
  Gemme * fusion_slot1; // NULL mean no gemme in fusion slot 1
  Gemme * fusion_slot2; // NULL mean no gemme in fusion slot 2
  }Game;


extern int is_click_inside(int mouse_x, int mouse_y, int x, int y, int width, int height);

void handle_inventory_click(int m_x, int m_y, Game * game);

int moveMonsters(Monster monsters[], Point path[], int pathSize, float deltaTime, Game *game);
Wave* initializeWave(int waveNumber, Point path[], int pathSize);

void buy_gemme(Game * game);

void placeTower(Game *game, Point position, Gemme *gemme);

/*Add 'mana' to the player mana storage.*/
void add_mana(Game * game, int mana);

/*Try to upgrade mana storage of the player*/
void upgrade_mana_storage(Game *);
void addVisualProjectile(Game *game, VisualProjectile vp);
void try_fusion(Game * game);
void try_place_gemme_on_tower(Game *game, int mouse_x, int mouse_y);
void try_remove_gemme_on_tower(Game *, int, int);
void check_wave_dead(Game * game);
void UpdateGemmesAndShoot(Game *game, float deltaTime);
#endif