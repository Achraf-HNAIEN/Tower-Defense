#include "gemme.h"
#include <stdlib.h>

Gemme generate_new_random_gemme(){
    Gemme gemme = {rand() % 360, 1};
    return gemme;
}

Gemme_pure generate_new_random_gemme_pure(Gemme gemme){
    Gemme_pure gemme_pure = {rand() % 3, rand() % 60, gemme.niveau};
    return gemme_pure;
}