#ifndef GEMME_H
#define GEMME_H
#include "monstre.h"


#define M_PI 3.14159265358979323846

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
} Gemme;

Gemme createGem(int niveau);

Gemme new_gemme_pure(int level);

void upgradeGem(Gemme *gem);

float calculateGemDamage(const Gemme *gem, const Monster *target);

void applyElementalEffect(Gemme *gem, Monster *target);

int fuseGems(const Gemme *gem1, const Gemme *gem2, Gemme *);

ElementType determineElementType(int teinte);

void initializeGem(Gemme *gem, int towerIndex);

#endif
