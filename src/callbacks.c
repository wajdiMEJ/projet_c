#include <gtk/gtk.h>
#include <stdio.h>
#include <string.h>

#include "callbacks.h"
#include "interface.h"
#include "support.h"



#define FILENAME "agent.txt"

// Fonction pour ajouter un agent



void on_button_ajouter_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *entry_id = lookup_widget(GTK_WIDGET(button), "entry_id");
    GtkWidget *entry_nom = lookup_widget(GTK_WIDGET(button), "entry_nom");
    GtkWidget *entry_prenom = lookup_widget(GTK_WIDGET(button), "entry_prenom");
    GtkWidget *entry_cin = lookup_widget(GTK_WIDGET(button), "entry_cin");
    GtkWidget *spinbutton_jours = lookup_widget(GTK_WIDGET(button), "spinbutton_jours");
    GtkWidget *spinbutton_mois = lookup_widget(GTK_WIDGET(button), "spinbutton_mois");
    GtkWidget *spinbutton_annee = lookup_widget(GTK_WIDGET(button), "spinbutton_annee");
    GtkWidget *entry_telephone = lookup_widget(GTK_WIDGET(button), "entry_telephone");
    GtkWidget *entry_mail = lookup_widget(GTK_WIDGET(button), "entry_mail");
    GtkWidget *spinbutton_salaire = lookup_widget(GTK_WIDGET(button), "spinbutton_salaire");
    GtkWidget *comboboxentry_gouvernorat = lookup_widget(GTK_WIDGET(button), "comboboxentry_gouvernorat");
    GtkWidget *radiobutton_femme = lookup_widget(GTK_WIDGET(button), "radiobutton_femme");

    GtkWidget *checkbutton_maintenence = lookup_widget(GTK_WIDGET(button), "checkbutton_maintenence");
    GtkWidget *checkbutton_securite = lookup_widget(GTK_WIDGET(button), "checkbutton_securite");
    GtkWidget *checkbutton_agent = lookup_widget(GTK_WIDGET(button), "checkbutton_agent");

    const char *id = gtk_entry_get_text(GTK_ENTRY(entry_id));
    const char *nom = gtk_entry_get_text(GTK_ENTRY(entry_nom));
    const char *prenom = gtk_entry_get_text(GTK_ENTRY(entry_prenom));
    const char *cin = gtk_entry_get_text(GTK_ENTRY(entry_cin));
    int jour = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinbutton_jours));
    int mois = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinbutton_mois));
    int annee = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinbutton_annee));
    const char *telephone = gtk_entry_get_text(GTK_ENTRY(entry_telephone));
    const char *mail = gtk_entry_get_text(GTK_ENTRY(entry_mail));
    int salaire = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinbutton_salaire));
    const char *gouvernorat = gtk_combo_box_get_active_text(GTK_COMBO_BOX(comboboxentry_gouvernorat));

    const char *sexe = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radiobutton_femme)) ? "Femme" : "Homme";

    const char *poste_maintenance = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkbutton_maintenence)) ? "Maintenance" : "";
    const char *poste_securite = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkbutton_securite)) ? "Sécurité" : "";
    const char *poste_agent = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkbutton_agent)) ? "Agent" : "";

    // Validation de base
    if (strlen(id) == 0 || strlen(nom) == 0 || strlen(prenom) == 0 || strlen(cin) == 0) {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button))),
                                                   GTK_DIALOG_DESTROY_WITH_PARENT,
                                                   GTK_MESSAGE_WARNING,
                                                   GTK_BUTTONS_OK,
                                                   "Veuillez remplir tous les champs obligatoires.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }

    // Vérification de l'adresse e-mail
    if (strchr(mail, '@') == NULL) {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button))),
                                                   GTK_DIALOG_DESTROY_WITH_PARENT,
                                                   GTK_MESSAGE_WARNING,
                                                   GTK_BUTTONS_OK,
                                                   "Veuillez entrer une adresse e-mail valide (doit contenir '@').");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }

    // Vérification de l'unicité de l'ID
    FILE *file = fopen("agent.txt", "r");
    if (file == NULL) {
        fprintf(stderr, "Erreur : Impossible d'ouvrir le fichier %s\n", "agent.txt");
        return;
    }

    char line[512];
    int id_exists = 0; // Variable pour vérifier si l'ID existe déjà
    while (fgets(line, sizeof(line), file) != NULL) {
        char existing_id[50];
        sscanf(line, "%49[^;]", existing_id);
        if (strcmp(existing_id, id) == 0) {
            id_exists = 1; // L'ID existe déjà
            break;
        }
    }
    fclose(file);

    // Si l'ID existe déjà, afficher un message d'erreur
    if (id_exists) {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button))),
                                                   GTK_DIALOG_DESTROY_WITH_PARENT,
                                                   GTK_MESSAGE_WARNING,
                                                   GTK_BUTTONS_OK,
                                                   "Impossible d'ajouter : L'ID existe déjà.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }

    // Écrire les informations dans le fichier
    file = fopen("agent.txt", "a");
    if (file == NULL) {
        fprintf(stderr, "Erreur : Impossible d'ouvrir le fichier %s\n", "agent.txt");
        return;
    }

    fprintf(file, "%s;%s;%s;%s;%d/%d/%d;%s;%s;%d;%s;%s;%s;%s;%s\n",
            id, nom, prenom, cin, jour, mois, annee, telephone, mail, salaire, gouvernorat, sexe,
            poste_maintenance, poste_securite, poste_agent);
    fclose(file);

    // Afficher un message de confirmation
    GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button))),
                                               GTK_DIALOG_DESTROY_WITH_PARENT,
                                               GTK_MESSAGE_INFO,
                                               GTK_BUTTONS_OK,
                                               "Agent ajouté avec succès.");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}




// Déclaration globale des variables si nécessaire
int x = 0, y = 0;

// Fonction de rappel pour radiobutton_homme

void
on_radiobutton_homme_toggled           (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
if (gtk_toggle_button_get_active(togglebutton)) {
        y = 1;
        g_print("Homme sélectionné, y = %d\n", y);
    }

}

// Fonction de rappel pour radiobutton_femme

void
on_radiobutton_femme_toggled           (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
if (gtk_toggle_button_get_active(togglebutton)) {
        y = 2;
        g_print("Femme sélectionnée, y = %d\n", y);
    }
}


// Fonction de rappel pour checkbutton_maintenence
void on_checkbutton_maintenence_toggled(GtkToggleButton *togglebutton, gpointer user_data) {
    if (gtk_toggle_button_get_active(togglebutton)) {
        x = 1;
        g_print("Maintenance activée, x = %d\n", x);
    } else {
        x = 0;
        g_print("Maintenance désactivée, x = %d\n", x);
    }
}

// Fonction de rappel pour checkbutton_securite
void on_checkbutton_securite_toggled(GtkToggleButton *togglebutton, gpointer user_data) {
    if (gtk_toggle_button_get_active(togglebutton)) {
        x = 2;
        g_print("Sécurité activée, x = %d\n", x);
    } else {
        x = 0;
        g_print("Sécurité désactivée, x = %d\n", x);
    }
}

// Fonction de rappel pour checkbutton_agent
void on_checkbutton_agent_toggled(GtkToggleButton *togglebutton, gpointer user_data) {
    if (gtk_toggle_button_get_active(togglebutton)) {
        x = 3;
        g_print("Agent activé, x = %d\n", x);
    } else {
        x = 0;
        g_print("Agent désactivé, x = %d\n", x);
    }
}













//////////////////////

void remplir_treeview(GtkWidget *treeview, const char *line);

void on_button___rechercher_clicked(GtkButton *button, gpointer user_data) {
    // Récupération de l'entrée associée pour la recherche
    GtkWidget *entry__rechercher = lookup_widget(GTK_WIDGET(button), "entry__rechercher");
    GtkWidget *treeview = lookup_widget(GTK_WIDGET(button), "treeview1"); // Récupération du TreeView
    
    // Récupération de l'ID entré par l'utilisateur
    const char *id_recherche = gtk_entry_get_text(GTK_ENTRY(entry__rechercher));

    // Validation de l'entrée
    if (strlen(id_recherche) == 0) {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button))),
                                                   GTK_DIALOG_DESTROY_WITH_PARENT,
                                                   GTK_MESSAGE_WARNING,
                                                   GTK_BUTTONS_OK,
                                                   "Veuillez entrer un ID à rechercher.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }

    // Ouverture du fichier agent.txt en lecture
    FILE *file = fopen("agent.txt", "r");
    if (file == NULL) {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button))),
                                                   GTK_DIALOG_DESTROY_WITH_PARENT,
                                                   GTK_MESSAGE_ERROR,
                                                   GTK_BUTTONS_OK,
                                                   "Erreur : Impossible d'ouvrir le fichier agent.txt.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }

    char line[512]; // Tampon pour stocker chaque ligne lue
    int found = 0;  // Variable pour indiquer si l'agent a été trouvé

    GtkListStore *store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(treeview)));
    if (store == NULL) {
        // Si le modèle n'existe pas, le créer
        store = gtk_list_store_new(11, // Nombre de colonnes
            G_TYPE_STRING, // ID
            G_TYPE_STRING, // Nom
            G_TYPE_STRING, // Prénom
            G_TYPE_STRING, // CIN
            G_TYPE_STRING, // Date de naissance
            G_TYPE_STRING, // Téléphone
            G_TYPE_STRING, // Mail
            G_TYPE_INT,    // Salaire
            G_TYPE_STRING, // Gouvernorat
            G_TYPE_STRING, // Sexe
            G_TYPE_STRING  // Poste
        );
        gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), GTK_TREE_MODEL(store));
        g_object_unref(store);
    } else {
        // Si un modèle existe déjà, on le vide avant d'ajouter de nouvelles données
        gtk_list_store_clear(store);
    }

    // Lecture ligne par ligne pour trouver l'agent correspondant à l'ID
    while (fgets(line, sizeof(line), file) != NULL) {
        char id[50]; // Tampon pour extraire l'ID de chaque ligne
        sscanf(line, "%49[^;]", id); // Extraction de l'ID de la ligne

        if (strcmp(id, id_recherche) == 0) { // Comparaison avec l'ID recherché
            found = 1; // Marquer que l'agent a été trouvé
            remplir_treeview(treeview, line); // Remplir le TreeView avec les données
            break;
        }
    }

    fclose(file); // Fermeture du fichier après la lecture

    // Affichage du résultat
    if (!found) {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button))),
                                                   GTK_DIALOG_DESTROY_WITH_PARENT,
                                                   GTK_MESSAGE_WARNING,
                                                   GTK_BUTTONS_OK,
                                                   "Aucun agent trouvé avec l'ID : %s.", id_recherche);
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    }
}

// Fonction pour remplir le TreeView avec les informations de l'agent
void remplir_treeview(GtkWidget *treeview, const char *line) {
    // Créer un modèle de liste pour le TreeView
    GtkListStore *store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(treeview)));

    // Extraire les informations de l'agent de la ligne
    char id[50], nom[50], prenom[50], cin[50], date_naissance[20], telephone[50], mail[50];
    int salaire;
    char gouvernorat[50], sexe[10], poste[20];

    sscanf(line, "%49[^;];%49[^;];%49[^;];%49[^;];%19[^;];%49[^;];%49[^;];%d;%49[^;];%9[^;];%19[^\n]",
           id, nom, prenom, cin, date_naissance, telephone, mail, &salaire, gouvernorat, sexe, poste);

    // Ajouter une nouvelle ligne au modèle avec les informations de l'agent
    GtkTreeIter iter;
    gtk_list_store_append(store, &iter);
    gtk_list_store_set(store, &iter,
                       0, id,
                       1, nom,
                       2, prenom,
                       3, cin,
                       4, date_naissance,
                       5, telephone,
                       6, mail,
                       7, salaire,
                       8, gouvernorat,
                       9, sexe,
                       10, poste, // Utilisation de la variable poste ici
                       -1); // Fin des arguments

    // Vérifier si les colonnes sont déjà configurées
    GList *columns = gtk_tree_view_get_columns(GTK_TREE_VIEW(treeview));
    if (g_list_length(columns) == 0) {
        GtkCellRenderer *renderer;
        GtkTreeViewColumn *column;

        const char *titles[] = {"ID", "Nom", "Prénom", "CIN", "Date de naissance", "Téléphone", "Mail",
                                "Salaire", "Gouvernorat", "Sexe", "Poste"};

        for (int i = 0; i < 11; i++) {
            renderer = gtk_cell_renderer_text_new();
            column = gtk_tree_view_column_new_with_attributes(titles[i], renderer, "text", i, NULL);
            gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
        }
    }
}



//////////////////








void
on_button__modifier_clicked            (GtkButton       *button,
                                        gpointer         user_data)
{


GtkWidget *MODIFIER;
    MODIFIER=create_MODIFIER();
    gtk_widget_show(MODIFIER);

}


void
on_button__supprimer__clicked          (GtkButton       *button,
                                        gpointer         user_data)
{
GtkWidget *SUPPRIMER;
    SUPPRIMER=create_SUPPRIMER();
    gtk_widget_show(SUPPRIMER);

}


void
on_button__affrev_clicked              (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_button2__rechercher_clicked         (GtkButton       *button,
                                        gpointer         user_data)
{



    GtkWidget *window1;
    window1=create_window1();
    gtk_widget_show(window1);



}


void
on_button__ajouter_clicked             (GtkButton       *button,
                                        gpointer         user_data)
{

    GtkWidget *AJOUTER;
    AJOUTER=create_AJOUTER();
    gtk_widget_show(AJOUTER);
	

}


void on_button_retour_clicked(GtkButton *button, gpointer user_data) {
    // Obtenir la fenêtre actuelle
    GtkWidget *current_window = gtk_widget_get_toplevel(GTK_WIDGET(button));
    
    // Créer la fenêtre d'accueil
    GtkWidget *HOME = create_HOME();
    
    // Afficher la fenêtre d'accueil
    gtk_widget_show(HOME);
    
    // Fermer la fenêtre actuelle
    gtk_widget_destroy(current_window);
}


void
on_button2__retour__rech_clicked       (GtkButton       *button,
                                        gpointer         user_data)
{
    GtkWidget *current_window = gtk_widget_get_toplevel(GTK_WIDGET(button));
 GtkWidget *HOME =create_HOME();
    gtk_widget_show(HOME);
gtk_widget_destroy(current_window);

}






////////////////////////////////////////////////////////
void on_button____RECHERCHER__clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *entry____rechercher = lookup_widget(GTK_WIDGET(button), "entry____rechercher");
    const char *id_rechercher = gtk_entry_get_text(GTK_ENTRY(entry____rechercher));

    // Vérifier si l'ID à rechercher est vide
    if (strlen(id_rechercher) == 0) {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button))),
                                                   GTK_DIALOG_DESTROY_WITH_PARENT,
                                                   GTK_MESSAGE_WARNING,
                                                   GTK_BUTTONS_OK,
                                                   "Veuillez saisir un ID à rechercher.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }

    // Ouvrir le fichier pour la lecture
    FILE *file = fopen("agent.txt", "r");
    if (file == NULL) {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button))),
                                                   GTK_DIALOG_DESTROY_WITH_PARENT,
                                                   GTK_MESSAGE_ERROR,
                                                   GTK_BUTTONS_OK,
                                                   "Erreur : Impossible d'ouvrir le fichier %s.", "agent.txt");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }

    // Variables pour lire les données
    char line[512];
    int found = 0;

    // Lire chaque ligne et rechercher l'ID
    while (fgets(line, sizeof(line), file)) {
        char *id, *nom, *prenom, *cin, *date_naissance, *telephone, *mail, *salaire_str, *gouvernorat, *sexe;
        char *poste_maintenance, *poste_securite, *poste_agent;

        // Utilisation de strtok pour séparer les champs par le délimiteur ';'
        id = strtok(line, ";");
        nom = strtok(NULL, ";");
        prenom = strtok(NULL, ";");
        cin = strtok(NULL, ";");
        date_naissance = strtok(NULL, ";");
        telephone = strtok(NULL, ";");
        mail = strtok(NULL, ";");
        salaire_str = strtok(NULL, ";");
        gouvernorat = strtok(NULL, ";");
        sexe = strtok(NULL, ";");
        poste_maintenance = strtok(NULL, ";");
        poste_securite = strtok(NULL, ";");
        poste_agent = strtok(NULL, "\n"); // Dernier champ avant le saut de ligne

        // Vérifier si l'ID correspond
        if (id != NULL && strcmp(id, id_rechercher) == 0) {
            found = 1;

            // Convertir le salaire en entier
            int salaire = (salaire_str != NULL) ? atoi(salaire_str) : 0;

            // Préparer le message à afficher
            char message[512];
            snprintf(message, sizeof(message),
                     "Agent trouvé :\n\n"
                     "ID : %s\nNom : %s\nPrénom : %s\nCIN : %s\nDate de naissance : %s\nTéléphone : %s\nMail : %s\nSalaire : %d\n"
                     "Gouvernorat : %s\nSexe : %s\nPostes : %s %s %s",
                     id, nom ? nom : "N/A", prenom ? prenom : "N/A", cin ? cin : "N/A", date_naissance ? date_naissance : "N/A",
                     telephone ? telephone : "N/A", mail ? mail : "N/A", salaire,
                     gouvernorat ? gouvernorat : "N/A", sexe ? sexe : "N/A",
                     poste_maintenance ? poste_maintenance : "", poste_securite ? poste_securite : "", poste_agent ? poste_agent : "");

            // Afficher les détails de l'agent trouvé
            GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button))),
                                                       GTK_DIALOG_DESTROY_WITH_PARENT,
                                                       GTK_MESSAGE_INFO,
                                                       GTK_BUTTONS_OK,
                                                       "%s", message);
            gtk_dialog_run(GTK_DIALOG(dialog));
            gtk_widget_destroy(dialog);
            break;
        }
    }

    fclose(file);

    // Si l'ID n'a pas été trouvé
    if (!found) {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button))),
                                                   GTK_DIALOG_DESTROY_WITH_PARENT,
                                                   GTK_MESSAGE_WARNING,
                                                   GTK_BUTTONS_OK,
                                                   "Aucun agent trouvé avec l'ID %s.", id_rechercher);
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    }
}






///////////////////////////////////////////////////






void on_button__MODIFIER__clicked(GtkButton *button, gpointer user_data) {
    // Widgets pour récupérer les données
    GtkWidget *entry_rechercher = lookup_widget(GTK_WIDGET(button), "entry____rechercher");
    GtkWidget *entry_nom = lookup_widget(GTK_WIDGET(button), "entry____nom");
    GtkWidget *entry_prenom = lookup_widget(GTK_WIDGET(button), "entry____prenom");
    GtkWidget *entry_cin = lookup_widget(GTK_WIDGET(button), "entry____cin");
    GtkWidget *spinbutton_jour = lookup_widget(GTK_WIDGET(button), "spinbutton____jour");
    GtkWidget *spinbutton_mois = lookup_widget(GTK_WIDGET(button), "spinbutton__mois");
    GtkWidget *spinbutton_annee = lookup_widget(GTK_WIDGET(button), "spinbutton__annee");
    GtkWidget *entry_telephone = lookup_widget(GTK_WIDGET(button), "entry____numtelephone");
    GtkWidget *entry_mail = lookup_widget(GTK_WIDGET(button), "entry____addmail");
    GtkWidget *spinbutton_salaire = lookup_widget(GTK_WIDGET(button), "spinbutton____sal");
    GtkWidget *combobox_gouvernorat = lookup_widget(GTK_WIDGET(button), "comboboxentry____cov");
    GtkWidget *checkbutton_maintenance = lookup_widget(GTK_WIDGET(button), "checkbutton____maintenence");
    GtkWidget *checkbutton_securite = lookup_widget(GTK_WIDGET(button), "checkbutton____securite");
    GtkWidget *checkbutton_agent = lookup_widget(GTK_WIDGET(button), "checkbutton____agent");
    GtkWidget *radiobutton_homme = lookup_widget(GTK_WIDGET(button), "radiobutton____homme");

    const char *id_rechercher = gtk_entry_get_text(GTK_ENTRY(entry_rechercher));

    // Validation de l'ID saisi
    if (strlen(id_rechercher) == 0) {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button))),
                                                   GTK_DIALOG_DESTROY_WITH_PARENT,
                                                   GTK_MESSAGE_WARNING,
                                                   GTK_BUTTONS_OK,
                                                   "Veuillez entrer un ID pour rechercher.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }

    // Variables pour lire et modifier le fichier
    FILE *file = fopen("agent.txt", "r");
    FILE *temp_file = fopen("temp.txt", "w");
    if (file == NULL || temp_file == NULL) {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button))),
                                                   GTK_DIALOG_DESTROY_WITH_PARENT,
                                                   GTK_MESSAGE_ERROR,
                                                   GTK_BUTTONS_OK,
                                                   "Erreur lors de l'ouverture du fichier.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }

    char line[512];
    int found = 0;

    // Parcourir les lignes du fichier
    while (fgets(line, sizeof(line), file)) {
        char id[50], nom[50], cin[50], date_naissance[50], telephone[50], mail[50], gouvernorat[ 50];
        char sexe[10], poste_1[20], poste_2[20], poste_3[20];
        int salaire;

        // Lire les données de chaque ligne
        sscanf(line, "%49[^;];%49[^;];%49[^;];%49[^;];%49[^;];%49[^;];%d;%49[^;];%9[^;];%19[^;];%19[^;];%19[^\n]",
               id, nom, cin, date_naissance, telephone, mail, &salaire, gouvernorat, sexe,
               poste_1, poste_2, poste_3);

        if (strcmp(id, id_rechercher) == 0) {
            found = 1;

            // Récupérer les nouvelles données depuis l'interface
            const char *new_nom = gtk_entry_get_text(GTK_ENTRY(entry_nom));
            const char *new_prenom = gtk_entry_get_text(GTK_ENTRY(entry_prenom));
            const char *new_cin = gtk_entry_get_text(GTK_ENTRY(entry_cin));
            int jour = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinbutton_jour));
            int mois = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinbutton_mois));
            int annee = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinbutton_annee));
            char date_naissance[11];
            snprintf(date_naissance, sizeof(date_naissance), "%02d/%02d/%04d", jour, mois, annee);
            const char *new_telephone = gtk_entry_get_text(GTK_ENTRY(entry_telephone));
            const char *new_mail = gtk_entry_get_text(GTK_ENTRY(entry_mail));

            // Vérification de l'adresse e-mail
            if (strchr(new_mail, '@') == NULL) {
                GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button))),
                                                           GTK_DIALOG_DESTROY_WITH_PARENT,
                                                           GTK_MESSAGE_WARNING,
                                                           GTK_BUTTONS_OK,
                                                           "Veuillez entrer une adresse e-mail valide contenant '@'.");
                gtk_dialog_run(GTK_DIALOG(dialog));
                gtk_widget_destroy(dialog);
                return;
            }

            int new_salaire = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinbutton_salaire));
            const char *new_gouvernorat = gtk_combo_box_get_active_text(GTK_COMBO_BOX(combobox_gouvernorat));
            const char *new_sexe = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radiobutton_homme)) ? "Homme" : "Femme";
            const char *new_poste_1 = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkbutton_agent)) ? "Agent" : "";
            const char *new_poste_2 = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkbutton_securite)) ? "Sécurité" : "";
            const char *new_poste_3 = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkbutton_maintenance)) ? "Maintenance" : "";

            // Écrire les nouvelles données dans le fichier temporaire
            fprintf(temp_file,"%s;%s;%s;%s;%s;%s;%s;%d;%s;%s;%s;%s;%s\n",
                    id, new_nom, new_prenom, new_cin, date_naissance, new_telephone, new_mail, new_salaire, new_gouvernorat, new_sexe, new_poste_1, new_poste_2, new_poste_3);
        } else {
            // Écrire la ligne originale si l'ID ne correspond pas
            fprintf(temp_file, "%s", line);
        }
    }

    fclose(file);
    fclose(temp_file);

    if (!found) {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button))),
                                                   GTK_DIALOG_DESTROY_WITH_PARENT,
                                                   GTK_MESSAGE_WARNING,
                                                   GTK_BUTTONS_OK,
                                                   "Aucun agent trouvé avec cet ID.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }

    // Remplacer l'ancien fichier par le nouveau
    remove("agent.txt");
    rename("temp.txt", "agent.txt");

    GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button))),
                                               GTK_DIALOG_DESTROY_WITH_PARENT,
                                               GTK_MESSAGE_INFO,
                                               GTK_BUTTONS_OK,
                                               "Les informations de l'agent ont été mises à jour avec succès.");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}


  






void
on_button2__retour__mod_clicked        (GtkButton       *button,
                                        gpointer         user_data)
{
GtkWidget *current_window = gtk_widget_get_toplevel(GTK_WIDGET(button));
GtkWidget *HOME =create_HOME();
    gtk_widget_show(HOME);
gtk_widget_destroy(current_window);


}


void
on_radiobutton_____homme_toggled       (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{

}


void
on_radiobutton_____femme_toggled       (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{

}


/*
void
on_buttonsearch_clicked                (GtkButton       *button,
                                        gpointer         user_data)
{



}


void
on_buttonsup_clicked                   (GtkButton       *button,
                                        gpointer         user_data)
{

}

*/







void remplir_treeview_with_we2(GtkWidget *treeviewwe2, const char *line);
void on_buttonsearch_clicked(GtkButton *button, gpointer user_data);
void on_buttonsup_clicked(GtkButton *button, gpointer user_data);

// Fonction pour rechercher un agent et l'afficher dans le TreeView
void on_buttonsearch_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *searchid = lookup_widget(GTK_WIDGET(button), "searchid");
    GtkWidget *treeviewwe2 = lookup_widget(GTK_WIDGET(button), "treeviewwe2");

    const char *id_recherche = gtk_entry_get_text(GTK_ENTRY(searchid));

    if (strlen(id_recherche) == 0) {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button))),
                                                   GTK_DIALOG_DESTROY_WITH_PARENT,
                                                   GTK_MESSAGE_WARNING,
                                                   GTK_BUTTONS_OK,
                                                   "Veuillez entrer un ID à rechercher.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }

    FILE *file = fopen("agent.txt", "r");
    if (file == NULL) {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button))),
                                                   GTK_DIALOG_DESTROY_WITH_PARENT,
                                                   GTK_MESSAGE_ERROR,
                                                   GTK_BUTTONS_OK,
                                                   "Erreur : Impossible d'ouvrir le fichier agent.txt.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }

    char line[512];
    int found = 0;
    GtkListStore *store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(treeviewwe2)));
    if (store == NULL) {
        store = gtk_list_store_new(12, // Nombre de colonnes
            G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, 
            G_TYPE_STRING, G_TYPE_STRING, G_TYPE_INT, G_TYPE_STRING, G_TYPE_STRING, 
            G_TYPE_STRING, G_TYPE_STRING);
        gtk_tree_view_set_model(GTK_TREE_VIEW(treeviewwe2), GTK_TREE_MODEL(store));
        g_object_unref(store);
    } else {
        gtk_list_store_clear(store);
    }

    while (fgets(line, sizeof(line), file) != NULL) {
        char id[50];
        sscanf(line, "%49[^;]", id);

        if (strcmp(id, id_recherche) == 0) {
            found = 1;
            remplir_treeview(treeviewwe2, line);
            break;
        }
    }

    fclose(file);

    if (!found) {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button))),
                                                   GTK_DIALOG_DESTROY_WITH_PARENT,
                                                   GTK_MESSAGE_WARNING,
                                                   GTK_BUTTONS_OK,
                                                   "Aucun agent trouvé avec l'ID : %s.", id_recherche);
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    }
}

// Fonction pour remplir le TreeView avec les informations de l'agent
void remplir_treeview_with_we2(GtkWidget *treeviewwe2, const char *line) {
    GtkListStore *store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(treeviewwe2)));

    char id[50], nom[50], prenom[50], cin[50], date_naissance[20], telephone[50], mail[50];
    int salaire;
    char gouvernorat[50], sexe[10], poste_maintenance[20], poste_securite[20], poste_agent[20];

    sscanf(line, "%49[^;];%49[^;];%49[^;];%49[^;];%19[^;];%49[^;];%49[^;];%d;%49[^;];%9[^;];%19[^;];%19[^;];%19[^\n]",
           id, nom, prenom, cin, date_naissance, telephone, mail, &salaire, gouvernorat, sexe, poste_maintenance, poste_securite, poste_agent);

    GtkTreeIter iter;
    gtk_list_store_append(store, &iter);
    gtk_list_store_set(store, &iter,
                       0, id,
                       1, nom,
                       2, prenom,
                       3, cin,
                       4, date_naissance,
                       5, telephone,
                       6, mail,
                       7, salaire,
                       8, gouvernorat,
                       9, sexe,
                       10, poste_maintenance,
                       11, poste_securite,
                       -1);

    GList *columns = gtk_tree_view_get_columns(GTK_TREE_VIEW(treeviewwe2));
    if (g_list_length(columns) == 0) {
        GtkCellRenderer *renderer;
        GtkTreeViewColumn *column;

        const char *titles[] = {"ID", "Nom", "Prénom", "CIN", "Date de naissance", "Téléphone", "Mail",
                                "Salaire", "Gouvernorat", "Sexe", "Poste Maintenance", "Poste Sécurité"};

        for (int i = 0; i < 12; i++) {
            renderer = gtk_cell_renderer_text_new();
            column = gtk_tree_view_column_new_with_attributes(titles[i], renderer, "text", i, NULL);
            gtk_tree_view_append_column(GTK_TREE_VIEW(treeviewwe2), column);
        }
    }
}

// Fonction pour supprimer un agent sélectionné du fichier et du TreeView
void on_buttonsup_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *treeviewwe2 = lookup_widget(GTK_WIDGET(button), "treeviewwe2");

    GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeviewwe2));
    GtkTreeModel *model;
    GtkTreeIter iter;

    if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
        gchar *id_to_remove;
        gtk_tree_model_get(model, &iter, 0, &id_to_remove, -1); // Récupérer l'ID de la ligne sélectionnée

        FILE *file = fopen("agent.txt", "r");
        if (file == NULL) {
            GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button))),
                                                       GTK_DIALOG_DESTROY_WITH_PARENT,
                                                       GTK_MESSAGE_ERROR,
                                                       GTK_BUTTONS_OK,
                                                       "Erreur : Impossible d'ouvrir le fichier agent.txt.");
            gtk_dialog_run(GTK_DIALOG(dialog));
            gtk_widget_destroy(dialog);
            return;
        }

        FILE *temp_file = fopen("temp_agent.txt", "w");
        if (temp_file == NULL) {
            GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button))),
                                                       GTK_DIALOG_DESTROY_WITH_PARENT,
                                                       GTK_MESSAGE_ERROR,
                                                       GTK_BUTTONS_OK,
                                                       "Erreur : Impossible d'ouvrir le fichier temporaire.");
            gtk_dialog_run(GTK_DIALOG(dialog));
            gtk_widget_destroy(dialog);
            fclose(file);
            return;
        }

        char line[512];
        int found = 0;
        while (fgets(line, sizeof(line), file) != NULL) {
            char id[50];
            sscanf(line, "%49[^;]", id);
            if (strcmp(id, id_to_remove) != 0) {
                fputs(line, temp_file); // Écrire la ligne dans le fichier temporaire si l'ID ne correspond pas
            } else {
                found = 1; // Marquer que l'ID a été trouvé et supprimé
            }
        }

        fclose(file);
        fclose(temp_file);

        if (found) {
            remove("agent.txt");
            rename("temp_agent.txt", "agent.txt"); // Remplacer l'ancien fichier par le nouveau
            GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button))),
                                                       GTK_DIALOG_DESTROY_WITH_PARENT,
                                                       GTK_MESSAGE_INFO,
                                                       GTK_BUTTONS_OK,
                                                       "Agent supprimé avec succès.");
            gtk_dialog_run(GTK_DIALOG(dialog));
            gtk_widget_destroy(dialog);

            // Réactualiser le TreeView
            gtk_list_store_clear(GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(treeviewwe2))));
        } else {
            GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button))),
                                                       GTK_DIALOG_DESTROY_WITH_PARENT,
                                                       GTK_MESSAGE_WARNING,
                                                       GTK_BUTTONS_OK,
                                                       "L'ID de l'agent sélectionné n'a pas été trouvé.");
            gtk_dialog_run(GTK_DIALOG(dialog));
            gtk_widget_destroy(dialog);
        }
    } else {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button))),
                                                   GTK_DIALOG_DESTROY_WITH_PARENT,
                                                   GTK_MESSAGE_WARNING,
                                                   GTK_BUTTONS_OK,
                                                   "Veuillez sélectionner un agent à supprimer.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    }
}








/////////////
void
on_button3retouur_clicked              (GtkButton       *button,
                                        gpointer         user_data)
{
GtkWidget *current_window = gtk_widget_get_toplevel(GTK_WIDGET(button));
GtkWidget *HOME =create_HOME(); 
    gtk_widget_show(HOME);
gtk_widget_destroy(current_window);

}


void
on_buttonretourlistes_clicked          (GtkButton       *button,
                                        gpointer         user_data)
{
GtkWidget *current_window = gtk_widget_get_toplevel(GTK_WIDGET(button));
GtkWidget *HOME =create_HOME() ;
    gtk_widget_show(HOME);
gtk_widget_destroy(current_window);
}





////////////////////////////////////////////////////


void on_afficherlesagents_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *treeview2 = lookup_widget(GTK_WIDGET(button), "treeview2");

    if (treeview2 == NULL) {
        return; // Ne rien faire si le widget n'est pas trouvé
    }

    // Définir le GtkListStore avec 11 colonnes et terminer par G_TYPE_INVALID
    GtkListStore *store = gtk_list_store_new(11, 
        G_TYPE_STRING, // ID
        G_TYPE_STRING, // Nom
        G_TYPE_STRING, // Prénom
        G_TYPE_STRING, // CIN
        G_TYPE_STRING, // Date de naissance
        G_TYPE_STRING, // Téléphone
        G_TYPE_STRING, // Mail
        G_TYPE_INT,    // Salaire
        G_TYPE_STRING, // Gouvernorat
        G_TYPE_STRING, // Sexe
        G_TYPE_STRING  // Poste
    );

    GtkTreeIter iter;
    FILE *f = fopen("agent.txt", "r");
    if (f == NULL) {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button))),
                                                   GTK_DIALOG_MODAL,
                                                   GTK_MESSAGE_ERROR,
                                                   GTK_BUTTONS_OK,
                                                   "Erreur lors de l'ouverture du fichier agent.txt");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }

    char ligne[1024];
    int count = 0; // Compteur de lignes lues
    while (fgets(ligne, sizeof(ligne), f)) {
        ligne[strcspn(ligne, "\r\n")] = 0; // Supprime le retour à la ligne

        // Afficher la ligne lue pour débogage
        printf("Ligne lue : %s\n", ligne);

        // Découper la ligne en tokens avec ";"
        char *tokens[11] = {""}; // Initialiser le tableau avec des chaînes vides
        char *token = strtok(ligne, ";");
        int i = 0;

        while (token != NULL && i < 11) {
            tokens[i++] = token;
            token = strtok(NULL, ";");
        }

        // Ajouter une chaîne vide pour les colonnes manquantes
        while (i < 11) {
            tokens[i++] = ""; // Ajoute une chaîne vide pour chaque champ manquant
        }

        // Déterminer le poste à afficher
        char *poste = "";
        if (strcmp(tokens[10], "Maintenance") == 0) {
            poste = "Maintenance";
        } else if (strcmp(tokens[10], "Sécurité") == 0) {
            poste = "Sécurité";
        } else if (strcmp(tokens[10], "Agent") == 0) {
            poste = "Agent";
        }

        // Ajouter l'élément au store
        gtk_list_store_append(store, &iter);
        gtk_list_store_set(store, &iter,
                           0, tokens[0],      // ID
                           1, tokens[1],      // Nom
                           2, tokens[2],      // Prénom
                           3, tokens[3],      // CIN
                           4, tokens[4],      // Date de naissance
                           5, tokens[5],      // Téléphone
                           6, tokens[6],      // Mail
                           7, atoi(tokens[7]),// Salaire
                           8, tokens[8],      // Gouvernorat
                           9, tokens[9],      // Sexe
                           10, poste           // Poste
                           );
        count++;
    }
    fclose(f);

    // Vérifiez combien de lignes ont été lues
    if (count == 0) {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button))),
                                                   GTK_DIALOG_MODAL,
                                                   GTK_MESSAGE_INFO,
                                                   GTK_BUTTONS_OK,
                                                   "Aucune donnée trouvée dans agent.txt");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    } else {
        printf("Lignes lues : %d\n", count); // Compte des lignes lues
    }

    gtk_tree_view_set_model(GTK_TREE_VIEW(treeview2), GTK_TREE_MODEL(store));

    // Supprimer les colonnes existantes
    while (gtk_tree_view_get_column(GTK_TREE_VIEW(treeview2), 0)) {
        gtk_tree_view_remove_column(GTK_TREE_VIEW(treeview2), gtk_tree_view_get_column(GTK_TREE_VIEW(treeview2), 0));
    }

    // Ajouter les colonnes
    GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview2), 
        gtk_tree_view_column_new_with_attributes("ID", renderer, "text", 0, NULL));
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview2), 
        gtk_tree_view_column_new_with_attributes("Nom", renderer, "text", 1, NULL));
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview2), 
        gtk_tree_view_column_new_with_attributes("Prénom", renderer, "text", 2, NULL));
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview2), 
        gtk_tree_view_column_new_with_attributes("CIN", renderer, "text", 3, NULL));
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview2), 
        gtk_tree_view_column_new_with_attributes("Date de naissance", renderer, "text", 4, NULL));
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview2), 
        gtk_tree_view_column_new_with_attributes("Téléphone", renderer, "text", 5, NULL));
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview2), 
        gtk_tree_view_column_new_with_attributes("Mail", renderer, "text", 6, NULL));
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview2), 
        gtk_tree_view_column_new_with_attributes("Salaire", renderer, "text", 7, NULL));
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview2), 
        gtk_tree_view_column_new_with_attributes("Gouvernorat", renderer, "text", 8, NULL));
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview2), 
        gtk_tree_view_column_new_with_attributes("Sexe", renderer, "text", 9, NULL));
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview2), 
        gtk_tree_view_column_new_with_attributes("Poste", renderer, "text", 10, NULL));

    g_object_unref(store);
}





void
on_buttonafficherlaliste__clicked      (GtkButton       *button,
                                        gpointer         user_data)
{
GtkWidget *LISTEAGENT;
    LISTEAGENT=create_LISTEAGENT();
    gtk_widget_show(LISTEAGENT);


}





