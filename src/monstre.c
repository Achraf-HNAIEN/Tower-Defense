#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

#include "monstre.h"

// void moveMonsters(Monster monsters[], Point path[], int pathLength) {
//     for (int i = 0; i < MONSTER_COUNT; i++) {
//         // Get the current and the next path points
//         Point currentPoint = path[(int)monsters[i].x];
//         Point nextPoint = path[(int)monsters[i].x + 1];

//         // Calculate the direction towards the next point
//         float dirX = nextPoint.x - currentPoint.x;
//         float dirY = nextPoint.y - currentPoint.y;
//         float length = sqrtf(dirX * dirX + dirY * dirY);

//         // Avoid division by zero
//         if (length != 0) {
//             dirX /= length;
//             dirY /= length;
//         }

//         // Move the monster towards the next point
//         monsters[i].x += dirX * monsters[i].speed;
//         monsters[i].y += dirY * monsters[i].speed;

//         // If the monster reaches or passes the next point, snap it to the next point
//         if ((dirX > 0 && monsters[i].x >= nextPoint.x) ||
//             (dirY > 0 && monsters[i].y >= nextPoint.y) ||
//             (dirX < 0 && monsters[i].x <= nextPoint.x) ||
//             (dirY < 0 && monsters[i].y <= nextPoint.y)) {
//             monsters[i].x = nextPoint.x;
//             monsters[i].y = nextPoint.y;
//         }

//         // Check if the monster has reached the end of the path
//         if ((int)monsters[i].x >= pathLength - 1) {
//             // The monster has reached the end of the path
//             // Handle it here, for example, by deducting the player's life
//             printf("Monster %d has reached the end of the path!\n", i);
//         }
//     }
// }

void moveMonsters(Monster monsters[], Point path[], int pathLength) {
    for (int i = 0; i < MONSTER_COUNT; i++) {
        // Ensure the path index is within bounds
        if (monsters[i].pathIndex >= pathLength - 1) continue;

        Point currentPoint = path[monsters[i].pathIndex];
        Point nextPoint = path[monsters[i].pathIndex + 1];

        // Calculate the direction towards the next point
        float dirX = nextPoint.x - currentPoint.x;
        float dirY = nextPoint.y - currentPoint.y;
        float length = sqrtf(dirX * dirX + dirY * dirY);

        // Normalize the direction vector
        if (length != 0) {
            dirX /= length;
            dirY /= length;
        }

        // Move the monster
        monsters[i].x += dirX * monsters[i].speed;
        monsters[i].y += dirY * monsters[i].speed;

        // Check monster is in next point
        float distanceToNext = sqrtf((monsters[i].x - nextPoint.x) * (monsters[i].x - nextPoint.x) +
                                     (monsters[i].y - nextPoint.y) * (monsters[i].y - nextPoint.y));
        if (distanceToNext < monsters[i].speed) {
            monsters[i].x = nextPoint.x;
            monsters[i].y = nextPoint.y;
            monsters[i].pathIndex++;
        }

        // Check reached the end of the path
        if (monsters[i].pathIndex >= pathLength - 1) {
            printf("Monster %d has reached the end of the path!\n", i);
            //reach the end
        }
    }
        printMonsterPositions(monsters,MONSTER_COUNT);
}


void initializeMonsters(Monster monsters[], int count, float speed) {
    for (int i = 0; i < count; i++) {
        monsters[i].x = 0.0f; 
        monsters[i].y = 0.0f;
        monsters[i].pathIndex = 0; 
        monsters[i].speed = speed;
    }
}

void printMonsterPositions(Monster monsters[], int count) {
    printf("Monster Positions:\n");
    for (int i = 0; i < count; i++) {
        printf("Monster %d: X: %.2f, Y: %.2f, PathIndex: %d\n", 
               i, monsters[i].x, monsters[i].y, monsters[i].pathIndex);
    }
}