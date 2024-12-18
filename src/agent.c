#include <stdio.h>
#include <string.h>
#include "agent.h"

#define FILENAME "agent.txt"

// Fonction pour ajouter un agent dans le fichier
int ajouter_agent(Agent agent) {
    FILE *file = fopen(FILENAME, "a");
    if (file == NULL) {
        fprintf(stderr, "Erreur : Impossible d'ouvrir le fichier %s\n", FILENAME);
        return 0;  // Retourne 0 en cas d'erreur
    }

    fprintf(file, "%s;%s;%s;%d/%d/%d;%s;%s;%d;%s;%s\n",
            agent.id, agent.nom, agent.cin, agent.jour, agent.mois, agent.annee,
            agent.telephone, agent.mail, agent.salaire, agent.gouvernorat, agent.sexe);

    fclose(file);
    return 1;  // Retourne 1 si l'ajout a réussi
}



// Fonction pour rechercher un agent par ID
int rechercher_agent_par_id(const char *id_recherche, Agent *result) {
    FILE *file = fopen(FILENAME, "r");
    if (file == NULL) {
        fprintf(stderr, "Erreur : Impossible d'ouvrir le fichier %s\n", FILENAME);
        return 0;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        Agent agent;

        // Découper les informations de chaque ligne
        if (sscanf(line, "%49[^;];%49[^;];%49[^;];%49[^;];%49[^;];%49[^;];%d;%49[^;];%9s",
                   agent.id, agent.nom, agent.cin, agent.jour, agent.mois, agent.annee,
                   agent.telephone, agent.mail, &agent.salaire,
                   agent.gouvernorat, agent.sexe) == 9) {

            // Vérifier si l'ID correspond
            if (strcmp(agent.id, id_recherche) == 0) {
                *result = agent;
                fclose(file);
                return 1; // Agent trouvé
            }
        }
    }

    fclose(file);
    return 0; // Agent non trouvé
}

/////////
