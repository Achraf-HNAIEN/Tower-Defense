#ifndef MONSTRE_H
#define MONSTRE_H

#include "grid.h"


#define WAVE_INTERVAL 35  // Interval in seconds
#define PROB_NORMAL 0.50  // Proba normal wave
#define PROB_CROWD  0.20  // Proba crowd wave
#define PROB_AGILE  0.20  // Proba agile wave
#define PROB_BOSS   0.10  // P60roba boss wave

#define COUNT_NORMAL 12   // normal wave
#define COUNT_CROWD  24   // crowd wave
#define COUNT_AGILE  12   // agile wave
#define COUNT_BOSS   2    // boss wave
#define SPAWN_DELAY 60

/**
 * @brief Enumeration of elemental types for monsters.
 */
typedef enum {
    ELEMENT_NONE,   /**< No elemental type */
    ELEMENT_PYRO,   /**< Pyro elemental type */
    ELEMENT_DENDRO, /**< Dendro elemental type */
    ELEMENT_HYDRO   /**< Hydro elemental type */
} ElementalType;

/**
 * @brief Enumeration of monster types.
 */
typedef enum {
    NORMAL, /**< Normal monster type */
    CROWD,  /**< Crowd monster type */
    AGILE,  /**< Agile monster type */
    BOSS    /**< Boss monster type */
} MonsterType;

/**
 * @brief Structure representing a monster.
 */
typedef struct Monster {
    float hp;                       /**< Current health points of the monster */
    float max_hp;                   /**< Maximum health points of the monster */
    float speed;                    /**< Speed of the monster */
    float x, y;                     /**< Position of the monster */
    float damage;                   /**< Damage inflicted by the monster */
    int pathIndex;                  /**< Index of the current path for the monster */
    int wave_number;                /**< Wave number of the monster */
    ElementalType elemental_type;   /**< Elemental type of the monster */
    MonsterType type;               /**< Type of the monster */
    int tint;                       /**< Tint of the monster */
    float elemental_residue;        /**< Elemental residue of the monster */
    float mana_reward;              /**< Mana reward for defeating the monster */
    float mana_penalty;             /**< Mana penalty for letting the monster reach the end of the path */
    int status_effects;             /**< Status effects applied to the monster */
    float status_duration;          /**< Duration of the status effects */
    float effect_intensity;         /**< Intensity of the status effects */
} Monster;

/**
 * @brief Initializes a monster with the given wave number and path.
 * 
 * @param waveNumber The wave number of the monster.
 * @param path The path for the monster to follow.
 * @param pathSize The size of the path array.
 * @return A pointer to the initialized monster.
 */
Monster * initialize_Monster(int waveNumber, Point path[], int pathSize);

/**
 * @brief Selects the monster type for the given wave number.
 * 
 * @param waveNumber The wave number.
 * @return The monster type for the wave.
 */
MonsterType selectWaveType(int waveNumber);

/**
 * @brief Applies damage to the monster.
 * 
 * @param monster A pointer to the monster.
 * @param damage The amount of damage to apply.
 */
void applay_Damage(Monster * monster, float damage);


#endif
