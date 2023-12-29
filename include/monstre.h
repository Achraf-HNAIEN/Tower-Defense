#ifndef MONSTRE_H
#define MONSTRE_H

#include "grid.h"

#define MONSTER_COUNT 30

typedef struct Monster {
    int hp;         
    double speed;      
    int x, y;       
    int damage;     
    int pathIndex;  
} Monster;

void initializeMonsters(Monster monsters[], int count, Point path[]);
void moveMonsters(Monster monsters[], int count, Point path[], int pathSize);
void drawMonsters(Monster monsters[], int count);

#endif
