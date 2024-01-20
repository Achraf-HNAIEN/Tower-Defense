#include "gemme.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

Gemme * createGem(int niveau){
    Gemme *newGem = (Gemme *) malloc(sizeof(Gemme));
    if(!newGem) {fprintf(stderr, "Fail to allocate memory.\n"); return newGem;}
    int teinte =-1;
    int randomType = rand() % 3;
    switch (randomType)
    {
    case 0:
        // red
        teinte = (rand() % 60) + 330;
        teinte %= 360;
        newGem->elementType = PYRO;
        break;
    case 1: // Green
        teinte = (rand() % 60) + 90;
        newGem->elementType = DENDRO;
        break;
    case 2: // Blue
        teinte = (rand() % 60) + 210;
        newGem->elementType = HYDRO;
        break;
    }

    newGem->type = PURE;
    newGem->niveau = niveau;
    newGem->teinte = teinte;
    newGem->cooldown = 2.0f;

    return newGem;
}

float calculateGemDamage(const Gemme *gem, const Monster *target)
{
    float baseDamage = BASE_DAMAGE; // to test

    int hueDifference = abs(gem->teinte - target->tint);
    if (hueDifference > 180)
    {
        hueDifference = 360 - hueDifference;
    }

    float colorEffectiveness = (1.0f - cos(M_PI * hueDifference / 180.0f)) / 2.0f;
    float finalDamage = baseDamage * pow(2, gem->niveau) * (1 - colorEffectiveness);

    if (gem->type == MIXTE)
    {
        finalDamage *= 1.3; // to review with Mathieu
        if (1 == rand() % 10)
            finalDamage *= 1.5;
    }

    return finalDamage;
}

void applyElementalEffect(Gemme *gem, Monster *target) {
    if (gem == NULL || target == NULL) {
        return;
    }
    
    switch (gem->elementType) {
        case PYRO:
            target->elemental_type = ELEMENT_PYRO;
            target->status_duration = 2.0f;
            break;
        case DENDRO:
            target->elemental_type = ELEMENT_DENDRO;
            target->status_duration = 10.0f;
            break;
        case HYDRO:
            target->elemental_type = ELEMENT_HYDRO;
            target->status_duration = 5.0f;
            break;
        default:
            break;
}
}

int fuseGems(const Gemme *gem1, const Gemme *gem2, Gemme *fusedGem)
{
    if (gem1 == NULL || gem2 == NULL || gem1->niveau != gem2->niveau){
        return 1;
    }
    if (fusedGem == NULL)
    {
        return 1;
    }
    fusedGem->niveau = gem1->niveau + 1;
    fusedGem->teinte = (gem1->teinte + gem2->teinte) / 2;

    if (gem1->type == PURE && gem2->type == PURE && gem1->elementType == gem2->elementType)
    {
        fusedGem->type = PURE;
        fusedGem->elementType = gem1->elementType;
    }
    else
    {
        fusedGem->type = MIXTE;
        fusedGem->elementType = ELEMENT_NONE;
    }
    fusedGem->cooldown = 2.0f;
    return 0;
}

void destroyGem(Gemme *gem)
{
    if (gem == NULL)
    {
        return;
    }
    free(gem);
}

