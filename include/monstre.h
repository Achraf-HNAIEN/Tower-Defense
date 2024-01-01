#ifndef MONSTRE_H
#define MONSTRE_H

#include "grid.h"

#define MONSTER_COUNT 30

typedef struct Monster {
    float hp;         
    float speed;       
    float x, y;        
    float damage;      
    int pathIndex;   
} Monster;


void initializeMonsters(Monster monsters[], int count, Point path[]);
void moveMonsters(Monster monsters[], int count, Point path[], int pathSize, float deltaTime);
#endif
