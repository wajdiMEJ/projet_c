
#ifndef AGENT_H
#define AGENT_H

typedef struct {
    char id[20];
    char nom[50];
    char prenom[50];
    char cin[20];
    int jour;
    int mois;
    int annee;
    char telephone[20];
    char mail[50];
    int salaire;
    char gouvernorat[50];
    char sexe[10];
} Agent;

// Declare the function ajouter_agent
int ajouter_agent(Agent agent);
int rechercher_agent_par_id(const char *id_recherche, Agent *result);
#endif

