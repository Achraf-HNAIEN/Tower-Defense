#ifndef MONSTRE_H
#define MONSTRE_H

#include "grid.h"


#define WAVE_INTERVAL 35  // Interval in seconds
#define PROB_NORMAL 0.50  // Proba normal wave
#define PROB_CROWD  0.20  // Proba crowd wave
#define PROB_AGILE  0.20  // Proba agile wave
#define PROB_BOSS   0.10  // Proba boss wave

#define COUNT_NORMAL 12   // normal wave
#define COUNT_CROWD  24   // crowd wave
#define COUNT_AGILE  12   // agile wave
#define COUNT_BOSS   2    // boss wave

typedef enum {
    ELEMENT_NONE,
    ELEMENT_PYRO,
    ELEMENT_DENDRO,
    ELEMENT_HYDRO
} ElementalType;

typedef enum {
    NORMAL,
    CROWD,
    AGILE,
    BOSS
} MonsterType;

typedef struct Monster {
    float hp;          
    float max_hp;      
    float speed;       
    float x, y;        
    float damage;      
    int pathIndex;     
    int wave_number;   
    ElementalType elemental_type; 
    MonsterType type;
    float elemental_residue;      
    float mana_reward;            
    float mana_penalty;           
    int status_effects;           
    float status_duration;        
    float effect_intensity;       
} Monster;


void initializeNormalMonsters(Monster monsters[], int wave, Point path[]);
void initializeCrowdMonsters(Monster monsters[], int wave, Point path[]);
void initializeQuickMonsters(Monster monsters[], int wave, Point path[]);
void initializeBosskMonsters(Monster monsters[], int wave, Point path[]);
MonsterType selectWaveType(int waveNumber);
void moveMonsters(Monster monsters[], int count, Point path[], int pathSize, float deltaTime);
#endif
