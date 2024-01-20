#ifndef GEMME_H
#define GEMME_H
#include "monstre.h"
#include <MLV/MLV_all.h>


#define M_PI 3.14159265358979323846
#define GEMME_CHARGE_TIME 2.0 // Seconde
#define BASE_DAMAGE 80.0f

typedef enum {
    PURE,
    MIXTE
} TypeGemme;

typedef enum {
    NONE,
    PYRO,   // Red
    DENDRO, // Green
    HYDRO   // Blue
} ElementType;

typedef struct {
    TypeGemme type;          
    int niveau;              
    int teinte;              
    ElementType elementType; 
    float cooldown;    
    int isReadyToShoot;         
} Gemme;


/**
 * @brief Creates a new Gemme object with the specified level.
 *
 * @param niveau The level of the gem.
 * @return A pointer to the created Gemme object.
 */
Gemme * createGem(int niveau);

/**
 * @brief Creates a new pure gem with the specified level.
 *
 * @param level The level of the gem.
 * @return The new pure gem.
 */
Gemme new_gemme_pure(int level);

/**
 * @brief Upgrades the specified gem.
 *
 * @param gem The gem to be upgraded.
 */
void upgradeGem(Gemme *gem);

/**
 * @brief Calculates the damage caused by the gem to the target monster.
 *
 * @param gem The gem used to calculate the damage.
 * @param target The target monster.
 * @return The calculated damage.
 */
float calculateGemDamage(const Gemme *gem, const Monster *target);

/**
 * @brief Applies the elemental effect of the gem to the target monster.
 *
 * @param gem The gem that applies the effect.
 * @param target The target monster.
 */
void applyElementalEffect(Gemme *gem, Monster *target);

/**
 * @brief Fuses two gems together to create a new gem.
 *
 * @param gem1 The first gem to be fused.
 * @param gem2 The second gem to be fused.
 * @param resultGem The resulting fused gem.
 * @return The success status of the fusion operation.
 */
int fuseGems(const Gemme *gem1, const Gemme *gem2, Gemme *resultGem);

/**
 * @brief Determines the element type based on the specified teinte.
 *
 * @param teinte The teinte value.
 * @return The determined element type.
 */
ElementType determineElementType(int teinte);

/**
 * @brief Initializes the specified gem with the given tower index.
 *
 * @param gem The gem to be initialized.
 * @param towerIndex The tower index to be assigned.
 */
void initializeGem(Gemme *gem, int towerIndex);

#endif
