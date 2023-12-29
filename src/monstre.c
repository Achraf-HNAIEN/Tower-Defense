#include "monstre.h"

void initializeMonsters(Monster monsters[], int count, Point path[]) {
    for (int i = 0; i < count; i++) {
        monsters[i].hp = 100;  
        monsters[i].speed = 1; 
        monsters[i].x = path[0].x;
        monsters[i].y = path[0].y;
        monsters[i].damage = 10; 
        monsters[i].pathIndex = 0;
    }
}

void moveMonsters(Monster monsters[], int count, Point path[], int pathSize) {
    for (int i = 0; i < count; i++) {
        if (monsters[i].hp <= 0) continue; 

        monsters[i].pathIndex += monsters[i].speed;
        if (monsters[i].pathIndex < pathSize) {
            monsters[i].x = path[monsters[i].pathIndex].x;
            monsters[i].y = path[monsters[i].pathIndex].y;
        }
    }
}


