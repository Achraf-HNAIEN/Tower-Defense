#include "gemme.h"
#include <stdlib.h>

static ElementType getElementTypeFromHue(int hue) {
    if ((hue >= 0 && hue <= 30) || (hue >= 330 && hue <= 359)) {
        return PYRO;
    } else if (hue >= 90 && hue <= 150) {
        return DENDRO;
    } else if (hue >= 210 && hue <= 270) {
        return HYDRO;
    }
    return -1; // Error case
}

static int generateHueForElementType(ElementType elementType) {
    switch (elementType) {
        case PYRO:
            if (rand() % 2 == 0) {
                return rand() % 31; //0 -30
            } else {
                return (rand() % 30) + 330; //330- 359
            }
        case DENDRO:
            return (rand() % 61) + 90; //90 -150
        case HYDRO:
            return (rand() % 61) + 210; //210-270
        default:
            return -1;
    }
}

Gemme generate_new_pure_gemme(int niveau) {
    ElementType elementType = (ElementType)(rand() % 3);
    
    int hue = generateHueForElementType(elementType);
    
    Gemme newGemme = {
        .type = PURE,
        .niveau = niveau,
        .teinte = hue,
        .elementType = elementType,
        .cooldown = 2.0, //default val to change later
        .range = 3       //default val to change later
    };
    
    return newGemme;
}
