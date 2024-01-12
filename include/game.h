#ifndef __GAME__
#define __GAME__


#include "grid.h"
#include "tower.h"





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
  // int tower_placement_cost;
  Tower towers[MAX_TOWERS]; 
  int tower_count;
  int last_wave_time;
  int manual_trigger_bonus;
  int manual_trigger;
  }Game;

void placeTower(Game *game, Point position, Gemme *gemme);
#endif