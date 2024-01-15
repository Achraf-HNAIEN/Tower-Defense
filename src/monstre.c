#include "monstre.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>



MonsterType selectWaveType(int waveNumber) {
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


Monster * initialize_Monster(int waveNumber, Point path[], int pathSize) {
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
        monsters[i].tint = rand() % 360;

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

void applay_Damage(Monster * monster, float damage){
    monster->hp -= damage;
}
