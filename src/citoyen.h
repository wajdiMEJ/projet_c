#ifndef CITOYEN_H
#define CITOYEN_H

typedef struct {
    char nom[50];
    char id[20];
    char email[50];
    char telephone[20];
    char mot_de_passe[50];
    int jour, mois, annee;
    char gouvernorat[50];
    char sexe[10];
} Citoyen;

// Prototypes des fonctions
void ajouter_citoyen(Citoyen c);
int rechercher_citoyen_par_id(const char *id);
int modifier_citoyen(const char *id, Citoyen c_modifie);

#endif

