#include "monstre.h"
#include <math.h>
#include <stdlib.h>
/*
void initializeMonsters(Monster monsters[], int count, Point path[]) {
    const float spawnDelay = 60.0f;
    for (int i = 0; i < count; i++) {
        monsters[i].hp = 100;  
        monsters[i].speed = 1; 
        monsters[i].x = path[0].x; 
        monsters[i].y = path[0].y; 
        monsters[i].damage = 10; 
        monsters[i].pathIndex = -i * spawnDelay;
    }
}
*/

int moveMonsters(Monster monsters[], Point path[], int pathSize, float deltaTime) {
     // One second between each monster spawn
    int count = 0;
    if(monsters[0].type == BOSS) {count = 2;}
    else if(monsters[0].type == CROWD) {count = 24;}
    else {count = 12;}
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
    return count;
}


static MonsterType selectWaveType(int waveNumber) {
    float randomValue = (float)rand() / RAND_MAX;

    // For first 5 waves, exclude the BOSS wave type
    if (waveNumber <= 5) {
        if (randomValue < PROB_NORMAL) {
            return NORMAL;
        } else if (randomValue < PROB_NORMAL + PROB_CROWD) {
            return CROWD;
        } else {
            return AGILE;
        }
    } else {
        // Include BOSS wave type for wave numbers above 5
        if (randomValue < PROB_NORMAL) {
            return NORMAL;
        } else if (randomValue < PROB_NORMAL + PROB_CROWD) {
            return CROWD;
        } else if (randomValue < PROB_NORMAL + PROB_CROWD + PROB_AGILE) {
            return AGILE;
        } else {
            return BOSS;
        }
    }
}


Monster * initializeWave(int waveNumber, Point path[], int pathSize) {
    MonsterType waveType = selectWaveType(waveNumber);
    Monster * monsters;
    if(waveType == BOSS){
        monsters = (Monster *) malloc(sizeof(Monster) * 2);
    }else if(waveType == CROWD){
        monsters = (Monster *) malloc(sizeof(Monster) * 24);
    }else{
        monsters = (Monster *) malloc(sizeof(Monster) * 12);
    }

    int count = 0;
    float baseHP = 100.0f;

    // Determine the number of monsters and other properties based on wave type
    switch (waveType) {
        case NORMAL:
            count = COUNT_NORMAL;
            break;
        case CROWD:
            count = COUNT_CROWD;
            break;
        case AGILE:
            count = COUNT_AGILE;
            break;
        case BOSS:
            count = COUNT_BOSS;
            break;
    }

    for (int i = 0; i < count; i++) {
        monsters[i].type = waveType;
        monsters[i].wave_number = waveNumber;
        monsters[i].x = path[0].x;
        monsters[i].y = path[0].y;
        monsters[i].elemental_type = ELEMENT_NONE;
        (waveType == AGILE) ? (monsters[i].pathIndex = -i * SPAWN_DELAY/2) : (monsters[i].pathIndex = -i * SPAWN_DELAY);

        if (waveType == BOSS) {
            monsters[i].hp = 12 * baseHP * pow(1.2, waveNumber);
            monsters[i].speed = 1.0f;
        } else {
            monsters[i].hp = baseHP * pow(1.2, waveNumber);
            monsters[i].speed = (waveType == AGILE) ? 2.0f : 1.0f;
        }

        monsters[i].max_hp = monsters[i].hp;
        monsters[i].damage = 10.0f; // Default damage, adjust as needed
        // monsters[i].elemental_residue = 0.0f; // No elemental residue initially
        // monsters[i].mana_reward = monsters[i].hp * 0.1f * pow(1.3, /* current mana level */);
        // monsters[i].mana_penalty = monsters[i].hp * 0.15f * pow(1.3, /* current mana level */);
        // monsters[i].status_effects = 0; // No status effects initially
        // monsters[i].status_duration = 0.0f;
        // monsters[i].effect_intensity = 0.0f;
        
    }
    return monsters;
}
