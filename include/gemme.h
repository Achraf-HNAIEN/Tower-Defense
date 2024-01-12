#ifndef GEMME_H
#define GEMME_H

//#include "monstre.h"
typedef struct Monster Monster;
typedef enum {
    PURE,
    MIXTE
} TypeGemme;

typedef enum {
    PYRO,   //red
    DENDRO, //green
    HYDRO   //blue
} ElementType;

typedef struct {
    TypeGemme type;          //PURE | MIXTE
    int niveau;
    int teinte;           
    ElementType elementType; // The elemental type, only relevant for PURE
    float cooldown;          //in seconds pls
    int range;               // Range of fire for the gemme

} Gemme;


Gemme generate_new_pure_gemme(int niveau);

Gemme createGemme(TypeGemme type, int niveau, int teinte, ElementType elementType, int range);

void upgradeGemme(Gemme *gemme);

double calculateGemmeDamage(const Gemme *gemme, int targetHue);

// a reajuster
void applyElementalEffect(const Gemme *gemme,Monster *monster);

Gemme fuseGemmes(const Gemme *gemme1, const Gemme *gemme2);

int determineGemmeCost(int niveau);

#endif // GEMME_H
