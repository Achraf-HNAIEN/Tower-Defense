
#ifndef MONSTRE_H
#define MONSTRE_H

#define MONSTER_COUNT 30
#define SPEED_NORMAL 1.0f / 60.0f 
#define SPEED_AGILE (2.0f / 60.0f)

#include "grid.h"

// typedef struct {
//     float x, y; 
//     float speed; 
// } Monster;

typedef struct {
    float x, y;   
    float speed; 
    int pathIndex;
} Monster;


void moveMonsters(Monster monsters[], Point path[], int pathLength);
void initializeMonsters(Monster monsters[], int count, float speed);
void printMonsterPositions(Monster monsters[], int count);


#endif