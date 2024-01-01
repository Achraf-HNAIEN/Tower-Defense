#include "monstre.h"
#include <math.h>

void initializeMonsters(Monster monsters[], int count, Point path[]) {
    const float spawnDelay = 100.0f;
    for (int i = 0; i < count; i++) {
        monsters[i].hp = 100;  
        monsters[i].speed = 1; 
        monsters[i].x = path[0].x; 
        monsters[i].y = path[0].y; 
        monsters[i].damage = 10; 
        monsters[i].pathIndex = -i * spawnDelay;
    }
}


void moveMonsters(Monster monsters[], int count, Point path[], int pathSize, float deltaTime) {
     // One second between each monster spawn

    for (int i = 0; i < count; i++) {
        // Skip dead monsters
        if (monsters[i].hp <= 0) continue;

        // Handle spawning delay for each monster
        if (monsters[i].pathIndex < 0) {
            monsters[i].pathIndex += deltaTime;
            if (monsters[i].pathIndex >= 0) {
                monsters[i].pathIndex = 0; 
            } else {
                continue; 
            }
        }

        
        float distanceToMove = monsters[i].speed * deltaTime;

       
        while (distanceToMove > 0 && monsters[i].pathIndex < pathSize - 1) {
            
            float dx = path[monsters[i].pathIndex + 1].x - monsters[i].x;
            float dy = path[monsters[i].pathIndex + 1].y - monsters[i].y;
            float segmentLength = sqrt(dx * dx + dy * dy);

            if (segmentLength == 0) {
                monsters[i].pathIndex++;
                continue;
            }

            dx /= segmentLength;
            dy /= segmentLength;

            float step = fmin(distanceToMove, segmentLength);

            monsters[i].x += dx * step;
            monsters[i].y += dy * step;
            distanceToMove -= step;

            if (step == segmentLength) {
                monsters[i].pathIndex++;
            }
        }
    }
}



