#ifndef GEMME_H
#define GEMME_H

// typedef enum {
//     ROUGE,
//     VERTE,
//     BLEU
// }type;

// typedef struct {
//     type type;
//     short teinte;
//     short niveau;
// }Gemme_pure;

// typedef struct {
//     short teinte;
//     short niveau;
// }Gemme;



typedef enum {
    PURE,
    MIXTE
} TypeGemme;

// Elemental types for pure gemmes
typedef enum {
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

Gemme generate_new_random_gemme();
//Gemme_pure generate_new_random_gemme_pure(Gemme);
Gemme createGemme(TypeGemme type, int niveau, int teinte, ElementType elementType);
void upgradeGemme(Gemme *gemme);

#endif // GEMME_H
