#include <stdio.h>
#include <string.h>
#include "citoyen.h"

// Fonction pour ajouter un citoyen dans le fichier
void ajouter_citoyen(Citoyen c) {
    FILE *f = fopen("citoyen.txt", "a"); // Ouvrir le fichier en mode ajout (append)
    if (f == NULL) {
        printf("Erreur : Impossible d'ouvrir le fichier citoyen.txt\n");
        return;
    }

    // Écrire les informations sur une seule ligne avec des barres obliques comme séparateurs
    fprintf(f, "%s/%s/%s/%s/%s/%02d/%02d/%04d/%s/%s\n",
            c.nom, c.id, c.email, c.telephone, c.mot_de_passe,
            c.jour, c.mois, c.annee, c.gouvernorat, c.sexe);

    fclose(f); // Fermer le fichier
}

int rechercher_citoyen_par_id(const char *id) {
    FILE *f = fopen("citoyen.txt", "r");
    if (f == NULL) {
        printf("Erreur : Impossible d'ouvrir le fichier citoyen.txt\n");
        return 0; // Erreur lors de l'ouverture du fichier
    }

    char ligne[1024]; // Tampon pour lire chaque ligne
    while (fgets(ligne, sizeof(ligne), f)) {
        // Supprimer les retours à la ligne ou espaces en fin de ligne
        ligne[strcspn(ligne, "\r\n")] = '\0';

        // Découper la ligne en tokens en utilisant "/" comme séparateur
        char *token = strtok(ligne, "/");
        int colonne = 0;

        while (token != NULL) {
            colonne++;
            if (colonne == 2) { // La colonne 2 correspond à l'ID
                if (strcmp(token, id) == 0) {
                    fclose(f);
                    return 1; // ID trouvé
                }
                break; // Pas besoin de continuer à parcourir les tokens
            }
            token = strtok(NULL, "/");
        }
    }

    fclose(f);
    return 0; // ID non trouvé
}





int modifier_citoyen(const char *id, Citoyen c_modifie) {
    FILE *f = fopen("citoyen.txt", "r");
    if (f == NULL) {
        printf("Erreur : Impossible d'ouvrir le fichier citoyen.txt\n");
        return 0;
    }

    FILE *f_temp = fopen("temp.txt", "w");
    if (f_temp == NULL) {
        printf("Erreur : Impossible de créer un fichier temporaire\n");
        fclose(f);
        return 0;
    }

    char ligne[1024];
    int id_found = 0;

    while (fgets(ligne, sizeof(ligne), f)) {
        char id_ligne[50] = "";
        sscanf(ligne, "%*[^/]/%49[^/]", id_ligne); // Extraire l'ID (2ème champ)

        if (strcmp(id_ligne, id) == 0) {
            // Si l'ID correspond, écrire la nouvelle ligne
            id_found = 1;
            fprintf(f_temp, "%s/%s/%s/%s/%s/%02d/%02d/%04d/%s/%s\n",
                    c_modifie.nom, c_modifie.id, c_modifie.email, c_modifie.telephone, c_modifie.mot_de_passe,
                    c_modifie.jour, c_modifie.mois, c_modifie.annee, c_modifie.gouvernorat, c_modifie.sexe);
        } else {
            // Sinon, réécrire la ligne d'origine
            fputs(ligne, f_temp);
        }
    }

    fclose(f);
    fclose(f_temp);

    if (id_found) {
        remove("citoyen.txt");
        rename("temp.txt", "citoyen.txt");
    } else {
        remove("temp.txt");
    }

    return id_found;
}

