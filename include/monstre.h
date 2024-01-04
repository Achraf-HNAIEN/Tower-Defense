#ifndef MONSTRE_H
#define MONSTRE_H

#include "grid.h"

#define MONSTER_COUNT 30

#define ELEMENTAL_TYPE_nb 3

typedef enum {
    ELEMENT_NONE,
    ELEMENT_PYRO,
    ELEMENT_DENDRO,
    ELEMENT_HYDRO
} ElementalType;

typedef struct Monster {
    float hp;          
    float max_hp;      
    float speed;       
    float x, y;        
    float damage;      
    int pathIndex;     
    int wave_number;   
    ElementalType elemental_type; 
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
void moveMonsters(Monster monsters[], int count, Point path[], int pathSize, float deltaTime);
#endif
