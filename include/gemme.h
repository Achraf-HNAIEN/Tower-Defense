#ifndef __GEMME__
#define __GEMME__ 

typedef enum {
    ROUGE,
    VERTE,
    BLEU
}type;

typedef struct {
    type type;
    short teinte;
    short niveau;
}Gemme_pure;

typedef struct {
    short teinte;
    short niveau;
}Gemme;

Gemme generate_new_random_gemme();



Gemme_pure generate_new_random_gemme_pure(Gemme);


#endif