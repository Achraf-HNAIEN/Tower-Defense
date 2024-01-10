#include "tower.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// // Constants for default tower attributes
// #define DEFAULT_RANGE 3.0
// #define DEFAULT_DAMAGE 50.0
// #define DEFAULT_ATTACK_SPEED 1.0
// #define DEFAULT_MANA_COST 100
// #define DEFAULT_LEVEL 1
// #define DEFAULT_COOLDOWN 0.0

// void Initialize_Tower(Tower *tower, Point position, int element_type) {
//     if (tower == NULL) {
//         // Error handling if the tower pointer is null
//         fprintf(stderr, "Error: Tower pointer is null in InitializeTower\n");
//         return;
//     }

//     tower->position = position;

//     tower->range = DEFAULT_RANGE;
//     tower->damage = DEFAULT_DAMAGE;
//     tower->attack_speed = DEFAULT_ATTACK_SPEED;
//     tower->mana_cost = DEFAULT_MANA_COST;
//     tower->level = DEFAULT_LEVEL;
//     tower->element_type = element_type; 

//     tower->targetID = -1;                
//     tower->cooldown = DEFAULT_COOLDOWN;
//     tower->gem_level = 0;                
//     tower->reaction_type = 0;            
//     tower->effect_duration = 0.0;
//     tower->slow_effect = 0.0;
//     tower->splash_radius = 0.0;
//     tower->parasite_damage = 0.0;

// }

// int Can_Build_Tower(Point position, int grid[HEIGHT][WIDTH]) {
//     // verfie si la position est dans la grille
//     if (position.x < 0 || position.x >= WIDTH || position.y < 0 || position.y >= HEIGHT) {
//         return 0;  // position est out of bounds
//     }

//     // Check if the cell is empty (empty = 0)
//     if (grid[position.y][position.x] == EMPTY) {
//         return 1;  // 
//     }

//     return 0;
// }


// void Upgrade_Tower(Tower *tower) {
//     if (tower == NULL) {
//         // si le tower pointer est null
//         fprintf(stderr, "Error: Tower pointer is null in UpgradeTower\n");
//         return;
//     }
//     // Augmente le niveau de la tour
//     tower->level += 1;
//     // augmente les dommages de la tour
//     tower->damage *= DAMAGE_INCREASE_FACTOR;
//     // augmente la portée de la tour
//     tower->range *= RANGE_INCREASE_FACTOR;
//     // augmente la vitesse d'attaque de la tour
//     tower->attack_speed *= ATTACK_SPEED_INCREASE_FACTOR;
//     // augmente le cout de la tour
//     tower->mana_cost *= UPGRADE_COST_MULTIPLIER;


//     // le reste des upgrade ici !!!!!!
// }

// // verifier si le monstre est rentre dans la zone de fonctionnement de la tour
// int isMonsterInRange(Tower *tower, Monster *monster) {
//     float dx = tower->position.x - monster->x;
//     float dy = tower->position.y - monster->y;
//     float distance = sqrt(dx * dx + dy * dy);
//     return distance <= tower->range;
// }

// void SetTarget(Tower *tower, Monster *monsters, int monster_count) {
//     if (tower == NULL || monsters == NULL) {
//         fprintf(stderr, "Error -------> null pointer setTarget function\n");
//         return;
//     }

//     tower->targetID = -1;  // change the target to default (-1)
//     float maxHealth = 0;

//     for (int i = 0; i < monster_count; i++) {
//         if (isMonsterInRange(tower, &monsters[i]) && monsters[i].hp > maxHealth) {
//             maxHealth = monsters[i].hp;
//             tower->targetID = i;  // Set the new target
//         }
//     }
// }


// void ApplyDamage(Monster *target, float damage) {
//     if (target->hp > 0) {
//         target->hp -= damage;
//         if (target->hp <= 0) {
//             // others
//         }
//     }
// }

// void ShootTarget(Tower *tower, Monster *target) {
//     if (tower == NULL || target == NULL) {
//         fprintf(stderr, "Error: Null pointer passed to ShootTarget\n");
//         return;
//     }

//     if (tower->cooldown <= 0) {
//         float damage = tower->damage;

//         if (tower->gem_level > 0) {
//             damage *= (1 + 0.1 * tower->gem_level); 
//         }

//         ApplyDamage(target, damage);

//         tower->cooldown = 1.0 / tower->attack_speed;

//         // Apply any special effects based on the tower's element type or gem
//         // For example:
//         // if (tower->element_type == ELEMENT_TYPE_PYRO) { /* Apply Pyro effects */ }

//     }
// }



void InitializeTower(Tower *tower, Point position) {
    if (tower == NULL) {
        fprintf(stderr, "Error: Tower pointer is null initialize Tower function\n");
        return;
    }
    tower->position = position;

    tower->gemme = NULL;
}

int CanBuildTower(const int grid[HEIGHT][WIDTH], Point position) {
    if (!isWithinBounds(position.x, position.y)) {
        return 0; //position is out of bounds
    }
    return grid[position.y][position.x] == EMPTY;
}

void PlaceGemmeInTower(Tower *tower, Gemme *gemme) {
    if (tower == NULL) {
        printf("Error ------> Tower is NULL in place gemme in tower\n");
        return;
    }
    if (gemme == NULL) {
        printf("Error ------> Gemme NULL in place Gemme in tower function\n");
        return;
    }

    if (tower->gemme != NULL) {
        printf("Warning ------> \n");

    }

    tower->gemme = gemme;
}

void RemoveGemmeFromTower(Tower *tower) {
    if (tower == NULL) {
        printf("Error -------> Tower is NULL in Remove gemme from Tower\n");
        return;
    }
    if (tower->gemme == NULL) {
        printf("Warning ------> No gemme to remove from this tower.\n");
        return;
    }
    tower->gemme = NULL;
    printf("Gemme removed from tower with success\n");
}
