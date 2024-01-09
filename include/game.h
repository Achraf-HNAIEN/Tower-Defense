#ifndef __GAME__
#define __GAME__
#include "grid.h"
//#include "monstre.h"

typedef struct{
  int wave;
  int mana;
  int mana_max;
  int pathSize;
  int quit;
  int grid[HEIGHT][WIDTH];
  Point *path;
  }Game;

  #endif