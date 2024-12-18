#include <gtk/gtk.h>
#include <stdio.h>
#include <string.h>
#include "citoyen.h"
#include "callbacks.h"
#include "interface.h"
#include "support.h"

void
on_button_valider_clicked              (GtkWidget *objet_graphique, gpointer user_data)
{

    // Récupérer les widgets depuis l'interface
    GtkWidget *entry_nom = lookup_widget(objet_graphique, "entry_nom");
    GtkWidget *entry_id = lookup_widget(objet_graphique, "entry_id");
    GtkWidget *entry_mail = lookup_widget(objet_graphique, "entry_mail");
    GtkWidget *entry_telephone = lookup_widget(objet_graphique, "entry_telephone");
    GtkWidget *entry_passe = lookup_widget(objet_graphique, "entry_passe");
    GtkWidget *spinbutton_jours = lookup_widget(objet_graphique, "spinbutton_jours");
    GtkWidget *spinbutton_mois = lookup_widget(objet_graphique, "spinbutton_mois");
    GtkWidget *spinbutton_annee = lookup_widget(objet_graphique, "spinbutton_annee");
    GtkWidget *combobox_gouvernorat = lookup_widget(objet_graphique, "comboboxentry_gouvernorat");
    GtkWidget *radiobutton_femme = lookup_widget(objet_graphique, "radiobutton_femme");
    GtkWidget *checkbutton_acceptez = lookup_widget(objet_graphique, "checkbutton_aaceptez");

    // Extraction des valeurs saisies
    Citoyen c;
    strcpy(c.nom, gtk_entry_get_text(GTK_ENTRY(entry_nom)));
    strcpy(c.id, gtk_entry_get_text(GTK_ENTRY(entry_id)));
    strcpy(c.email, gtk_entry_get_text(GTK_ENTRY(entry_mail)));
    strcpy(c.telephone, gtk_entry_get_text(GTK_ENTRY(entry_telephone)));
    strcpy(c.mot_de_passe, gtk_entry_get_text(GTK_ENTRY(entry_passe)));
    c.jour = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinbutton_jours));
    c.mois = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinbutton_mois));
    c.annee = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinbutton_annee));

    const char *gouvernorat = gtk_combo_box_get_active_text(GTK_COMBO_BOX(combobox_gouvernorat));
    if (gouvernorat != NULL) {
        strcpy(c.gouvernorat, gouvernorat);
    } else {
        strcpy(c.gouvernorat, "Inconnu");
    }

    strcpy(c.sexe, gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radiobutton_femme)) ? "Femme" : "Homme");

    gboolean accepte = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkbutton_acceptez));

    // Vérifications des champs obligatoires
    if (!accepte) {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(objet_graphique)),
                                                   GTK_DIALOG_MODAL,
                                                   GTK_MESSAGE_ERROR,
                                                   GTK_BUTTONS_OK,
                                                   "Veuillez accepter les conditions générales.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }


// 


    if (strlen(c.nom) == 0 || strlen(c.id) == 0 || strlen(c.email) == 0 || strlen(c.telephone) == 0 || strlen(c.mot_de_passe) == 0) {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(objet_graphique)),
                                                   GTK_DIALOG_MODAL,
                                                   GTK_MESSAGE_ERROR,
                                                   GTK_BUTTONS_OK,
                                                   "Tous les champs sont obligatoires.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }
// Vérification de l'adresse e-mail
    if (strchr(c.email, '@') == NULL) {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(objet_graphique)),
                                                   GTK_DIALOG_MODAL,
                                                   GTK_MESSAGE_ERROR,
                                                   GTK_BUTTONS_OK,
                                                   "Veuillez entrer une adresse e-mail valide (doit contenir '@').");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }



// Ajouter le citoyen dans le fichier
ajouter_citoyen(c);

// Afficher un message de confirmation
GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(objet_graphique)),
                                           GTK_DIALOG_MODAL,
                                           GTK_MESSAGE_INFO,
                                           GTK_BUTTONS_OK,
                                           "Données enregistrées avec succès !");
gtk_dialog_run(GTK_DIALOG(dialog));
gtk_widget_destroy(dialog);

}







void on_button_chercher_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *entry_id = lookup_widget(GTK_WIDGET(button), "e_id");
    GtkWidget *treeview = lookup_widget(GTK_WIDGET(button), "tableau_treeview");

    if (GTK_IS_ENTRY(entry_id)) {
        const char *id_recherche = gtk_entry_get_text(GTK_ENTRY(entry_id));

        if (strlen(id_recherche) == 0) {
            GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button))),
                                                       GTK_DIALOG_MODAL,
                                                       GTK_MESSAGE_ERROR,
                                                       GTK_BUTTONS_OK,
                                                       "Veuillez entrer un ID valide.");
            gtk_dialog_run(GTK_DIALOG(dialog));
            gtk_widget_destroy(dialog);
            return;
        }

        // Effacer le contenu existant du TreeView
        GtkListStore *store = gtk_list_store_new(8,
                                                 G_TYPE_STRING, // ID
                                                 G_TYPE_STRING, // Nom
                                                 G_TYPE_STRING, // Date de naissance
                                                 G_TYPE_STRING, // Sexe
                                                 G_TYPE_STRING, // Gouvernorat
                                                 G_TYPE_STRING, // Mail
                                                 G_TYPE_STRING, // Numéro de téléphone
                                                 G_TYPE_STRING  // Mot de passe
        );
        GtkTreeIter iter;

        // Lecture du fichier citoyen.txt
        FILE *f = fopen("citoyen.txt", "r");
        if (f == NULL) {
            GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button))),
                                                       GTK_DIALOG_MODAL,
                                                       GTK_MESSAGE_ERROR,
                                                       GTK_BUTTONS_OK,
                                                       "Erreur lors de l'ouverture du fichier citoyen.txt.");
            gtk_dialog_run(GTK_DIALOG(dialog));
            gtk_widget_destroy(dialog);
            return;
        }

        char ligne[1024];
        gboolean trouve = FALSE;

        while (fgets(ligne, sizeof(ligne), f)) {
            ligne[strcspn(ligne, "\r\n")] = '\0'; // Supprimer les retours à la ligne

            // Découper la ligne en tokens avec "/"
            char *nom = strtok(ligne, "/");
            char *id = strtok(NULL, "/");
            char *mail = strtok(NULL, "/");
            char *telephone = strtok(NULL, "/");
            char *mot_de_passe = strtok(NULL, "/");
            char *jour = strtok(NULL, "/");
            char *mois = strtok(NULL, "/");
            char *annee = strtok(NULL, "/");
            char *gouvernorat = strtok(NULL, "/");
            char *sexe = strtok(NULL, "/");

            // Vérifier si l'ID correspond
            if (id && strcmp(id, id_recherche) == 0) {
                trouve = TRUE;

                // Formater la date de naissance
                char date_naissance[20];
                snprintf(date_naissance, sizeof(date_naissance), "%s/%s/%s", jour, mois, annee);

                // Ajouter les données au modèle
                gtk_list_store_append(store, &iter);
                gtk_list_store_set(store, &iter,
                                   0, id,
                                   1, nom,
                                   2, date_naissance,
                                   3, sexe,
                                   4, gouvernorat,
                                   5, mail,
                                   6, telephone,
                                   7, mot_de_passe,
                                   -1);
                break; // Sortir après avoir trouvé l'ID
            }
        }
        fclose(f);

        // Mettre à jour le TreeView avec le nouveau modèle
        gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), GTK_TREE_MODEL(store));

        // Libérer la mémoire du modèle
        g_object_unref(store);

        if (!trouve) {
            GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button))),
                                                       GTK_DIALOG_MODAL,
                                                       GTK_MESSAGE_ERROR,
                                                       GTK_BUTTONS_OK,
                                                       "ID non trouvé.");
            gtk_dialog_run(GTK_DIALOG(dialog));
            gtk_widget_destroy(dialog);
        }
    } else {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button))),
                                                   GTK_DIALOG_MODAL,
                                                   GTK_MESSAGE_ERROR,
                                                   GTK_BUTTONS_OK,
                                                   "Erreur : Le widget d'entrée est invalide.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    }
}




void
on_button_modifier_clicked             (GtkButton       *button,
                                        gpointer         user_data)
{
    GtkWidget *entry_id = lookup_widget(GTK_WIDGET(button), "e_Id");
    const char *id_recherche = gtk_entry_get_text(GTK_ENTRY(entry_id));

    if (strlen(id_recherche) == 0) {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button))),
                                                   GTK_DIALOG_MODAL,
                                                   GTK_MESSAGE_ERROR,
                                                   GTK_BUTTONS_OK,
                                                   "Veuillez entrer un ID valide.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }

    Citoyen c_modifie;

    // Widgets pour les champs de modification
    GtkWidget *entry_nom = lookup_widget(GTK_WIDGET(button), "e_nom");
    GtkWidget *spinbutton_jours = lookup_widget(GTK_WIDGET(button), "sb_jours");
    GtkWidget *spinbutton_mois = lookup_widget(GTK_WIDGET(button), "sb_mois");
    GtkWidget *spinbutton_annee = lookup_widget(GTK_WIDGET(button), "sb_annee");
    GtkWidget *rb_femme = lookup_widget(GTK_WIDGET(button), "rb_femme");
    GtkWidget *rb_homme = lookup_widget(GTK_WIDGET(button), "rb_homme");
    GtkWidget *combobox_gouvernorat = lookup_widget(GTK_WIDGET(button), "cbe_gouvernorat");
    GtkWidget *entry_mail = lookup_widget(GTK_WIDGET(button), "e_mail");
    GtkWidget *entry_telephone = lookup_widget(GTK_WIDGET(button), "e_telephone");
    GtkWidget *entry_passe = lookup_widget(GTK_WIDGET(button), "e_passe");

    // Remplir le citoyen avec les nouvelles valeurs
    strcpy(c_modifie.id, id_recherche);
    strcpy(c_modifie.nom, gtk_entry_get_text(GTK_ENTRY(entry_nom)));
    c_modifie.jour = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinbutton_jours));
    c_modifie.mois = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinbutton_mois));
    c_modifie.annee = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinbutton_annee));
    strcpy(c_modifie.gouvernorat, gtk_combo_box_get_active_text(GTK_COMBO_BOX(combobox_gouvernorat)));
    strcpy(c_modifie.email, gtk_entry_get_text(GTK_ENTRY(entry_mail)));
    strcpy(c_modifie.telephone, gtk_entry_get_text(GTK_ENTRY(entry_telephone)));
    strcpy(c_modifie.mot_de_passe, gtk_entry_get_text(GTK_ENTRY(entry_passe)));
    strcpy(c_modifie.sexe, gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(rb_femme)) ? "Femme" : "Homme");

    // Vérification de l'adresse e-mail
    if (strchr(c_modifie.email, '@') == NULL) {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button))),
                                                   GTK_DIALOG_MODAL,
                                                   GTK_MESSAGE_ERROR,
                                                   GTK_BUTTONS_OK,
                                                   "Veuillez entrer une adresse e-mail valide (doit contenir '@').");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }

    // Modifier le citoyen
    if (modifier_citoyen(id_recherche, c_modifie)) {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button))),
                                                   GTK_DIALOG_MODAL,
                                                   GTK_MESSAGE_INFO,
                                                   GTK_BUTTONS_OK,
                                                   "Modification réussie !");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    } else {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button))),
                                                   GTK_DIALOG_MODAL,
                                                   GTK_MESSAGE_ERROR,
                                                   GTK_BUTTONS_OK,
                                                   "Erreur : ID introuvable.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    }
}






void on_button_treeview_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *treeview = lookup_widget(GTK_WIDGET(button), "tableau_treeview");

    // Modèle pour le TreeView
    GtkListStore *store = gtk_list_store_new(8, 
        G_TYPE_STRING, // ID
        G_TYPE_STRING, // Nom
        G_TYPE_STRING, // Date de naissance
        G_TYPE_STRING, // Sexe
        G_TYPE_STRING, // Gouvernorat
        G_TYPE_STRING, // Mail
        G_TYPE_STRING, // Numéro de téléphone
        G_TYPE_STRING  // Mot de passe
    );
    GtkTreeIter iter;

    // Lecture du fichier et ajout des citoyens au modèle
    FILE *f = fopen("citoyen.txt", "r");
    if (f == NULL) {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button))),
                                                   GTK_DIALOG_MODAL,
                                                   GTK_MESSAGE_ERROR,
                                                   GTK_BUTTONS_OK,
                                                   "Erreur lors de l'ouverture du fichier citoyen.txt");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }

    char ligne[1024];
    while (fgets(ligne, sizeof(ligne), f)) {
        // Supprimer les retours à la ligne
        ligne[strcspn(ligne, "\r\n")] = '\0';

        // Découper la ligne en tokens avec "/"
        char *nom = strtok(ligne, "/");
        char *id = strtok(NULL, "/");
        char *mail = strtok(NULL, "/");
        char *telephone = strtok(NULL, "/");
        char *mot_de_passe = strtok(NULL, "/");
        char *jour = strtok(NULL, "/");
        char *mois = strtok(NULL, "/");
        char *annee = strtok(NULL, "/");
        char *gouvernorat = strtok(NULL, "/");
        char *sexe = strtok(NULL, "/");

        // Vérifier que toutes les données sont présentes
        if (nom && id && mail && telephone && mot_de_passe && jour && mois && annee && gouvernorat && sexe) {
            // Formater la date de naissance
            char date_naissance[20];
            snprintf(date_naissance, sizeof(date_naissance), "%s/%s/%s", jour, mois, annee);

            // Ajouter les données au modèle
            gtk_list_store_append(store, &iter);
            gtk_list_store_set(store, &iter,
                               0, id,
                               1, nom,
                               2, date_naissance,
                               3, sexe,
                               4, gouvernorat,
                               5, mail,
                               6, telephone,
                               7, mot_de_passe,
                               -1);
        }
    }
    fclose(f);

    // Lier le modèle au TreeView
    gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), GTK_TREE_MODEL(store));

    // Supprimer toutes les colonnes existantes avant d'en ajouter de nouvelles
    while (gtk_tree_view_get_column(GTK_TREE_VIEW(treeview), 0)) {
        gtk_tree_view_remove_column(GTK_TREE_VIEW(treeview), gtk_tree_view_get_column(GTK_TREE_VIEW(treeview), 0));
    }

    // Ajouter les colonnes au TreeView
    GtkCellRenderer *renderer = gtk_cell_renderer_text_new();

    GtkTreeViewColumn *col_id = gtk_tree_view_column_new_with_attributes("ID", renderer, "text", 0, NULL);
    GtkTreeViewColumn *col_nom = gtk_tree_view_column_new_with_attributes("Nom", renderer, "text", 1, NULL);
    GtkTreeViewColumn *col_date_naissance = gtk_tree_view_column_new_with_attributes("Date de naissance", renderer, "text", 2, NULL);
    GtkTreeViewColumn *col_sexe = gtk_tree_view_column_new_with_attributes("Sexe", renderer, "text", 3, NULL);
    GtkTreeViewColumn *col_gouvernorat = gtk_tree_view_column_new_with_attributes("Gouvernorat", renderer, "text", 4, NULL);
    GtkTreeViewColumn *col_mail = gtk_tree_view_column_new_with_attributes("Mail", renderer, "text", 5, NULL);
    GtkTreeViewColumn *col_numero = gtk_tree_view_column_new_with_attributes("Numéro de téléphone", renderer, "text", 6, NULL);
    GtkTreeViewColumn *col_mot_de_passe = gtk_tree_view_column_new_with_attributes("Mot de passe", renderer, "text", 7, NULL);

    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), col_id);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), col_nom);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), col_date_naissance);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), col_sexe);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), col_gouvernorat);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), col_mail);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), col_numero);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), col_mot_de_passe);

    // Libérer la mémoire du modèle
    g_object_unref(store);
}





void on_button_thez_modifier_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *window2;

    // Crée et affiche la nouvelle fenêtre (window2)
    window2 = create_window2();
    gtk_widget_show(window2);

    // Ferme la fenêtre actuelle (window1)
    GtkWidget *window1 = gtk_widget_get_toplevel(GTK_WIDGET(button));
    gtk_widget_destroy(window1);

   
}








void on_button_supprimer_clicked(GtkButton *button, gpointer user_data) {
    // Récupérer le TreeView
    GtkWidget *treeview = lookup_widget(GTK_WIDGET(button), "tableau_treeview");

    // Récupérer la sélection actuelle dans le TreeView
    GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));
    GtkTreeModel *model;
    GtkTreeIter iter;

    // Vérifier si une ligne est sélectionnée
    if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
        // Récupérer l'ID du citoyen sélectionné dans le TreeView
        gchar *id_to_delete;
        gtk_tree_model_get(model, &iter, 0, &id_to_delete, -1);  // 0 correspond à la colonne ID

        // Supprimer la ligne du TreeView
        gtk_list_store_remove(GTK_LIST_STORE(model), &iter);

        // Ouvrir le fichier citoyen.txt
        FILE *f = fopen("citoyen.txt", "r");
        if (f == NULL) {
            perror("Erreur d'ouverture du fichier");
            return;
        }

        // Ouvrir un fichier temporaire pour sauvegarder les données après suppression
        FILE *f_temp = fopen("temp.txt", "w");
        if (f_temp == NULL) {
            perror("Erreur d'ouverture du fichier temporaire");
            fclose(f);
            return;
        }

        char buffer[1024];
        int id_found = 0;

        // Lire le fichier et copier tout sauf la ligne contenant l'ID à supprimer
        while (fgets(buffer, sizeof(buffer), f)) {
            if (strstr(buffer, id_to_delete) != NULL) {
                id_found = 1;  // Marquer que l'ID a été trouvé et supprimé
            } else {
                fputs(buffer, f_temp);  // Copier la ligne dans le fichier temporaire
            }
        }

        fclose(f);
        fclose(f_temp);

        // Si l'ID n'a pas été trouvé
        if (!id_found) {
            GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button))),
                                                       GTK_DIALOG_MODAL,
                                                       GTK_MESSAGE_ERROR,
                                                       GTK_BUTTONS_OK,
                                                       "Aucun citoyen trouvé avec cet ID.");
            gtk_dialog_run(GTK_DIALOG(dialog));
            gtk_widget_destroy(dialog);
            return;
        }

        // Remplacer le fichier citoyen.txt par le fichier temporaire
        remove("citoyen.txt");
        rename("temp.txt", "citoyen.txt");

        // Afficher un message de confirmation
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button))),
                                                   GTK_DIALOG_MODAL,
                                                   GTK_MESSAGE_INFO,
                                                   GTK_BUTTONS_OK,
                                                   "Citoyen supprimé avec succès.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);

        // Libérer la mémoire allouée pour l'ID
        g_free(id_to_delete);
    } else {
        // Si aucune ligne n'est sélectionnée
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button))),
                                                   GTK_DIALOG_MODAL,
                                                   GTK_MESSAGE_ERROR,
                                                   GTK_BUTTONS_OK,
                                                   "Veuillez sélectionner un citoyen à supprimer.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    }
}




void
on_button_login_clicked                (GtkButton       *button,
                                        gpointer         user_data)
{
  GtkWidget *window3;

    window3 = create_window3();
    gtk_widget_show(window3);


  GtkWidget *window5;

    window5 = create_window5();
    gtk_widget_show(window5);

GtkWidget *window4 = gtk_widget_get_toplevel(GTK_WIDGET(button));
    gtk_widget_destroy(window4);

}


void
on_button_newacoount_clicked           (GtkButton       *button,
                                        gpointer         user_data)
{
 GtkWidget *window1;

    // Crée et affiche la nouvelle fenêtre (window2)
    window1 = create_window1();
    gtk_widget_show(window1);

GtkWidget *window4 = gtk_widget_get_toplevel(GTK_WIDGET(button));
    gtk_widget_destroy(window4);

}


void
on_button_mesinformations_clicked      (GtkButton       *button,
                                        gpointer         user_data)
{
GtkWidget *treeview = lookup_widget(GTK_WIDGET(button), "treeview_donnne_modifer");

    // Modèle pour le TreeView
    GtkListStore *store = gtk_list_store_new(8, 
        G_TYPE_STRING, // ID
        G_TYPE_STRING, // Nom
        G_TYPE_STRING, // Date de naissance
        G_TYPE_STRING, // Sexe
        G_TYPE_STRING, // Gouvernorat
        G_TYPE_STRING, // Mail
        G_TYPE_STRING, // Numéro de téléphone
        G_TYPE_STRING  // Mot de passe
    );
    GtkTreeIter iter;

    // Lecture du fichier et ajout des citoyens au modèle
    FILE *f = fopen("citoyen.txt", "r");
    if (f == NULL) {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button))),
                                                   GTK_DIALOG_MODAL,
                                                   GTK_MESSAGE_ERROR,
                                                   GTK_BUTTONS_OK,
                                                   "Erreur lors de l'ouverture du fichier citoyen.txt");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }

    char ligne[1024];
    while (fgets(ligne, sizeof(ligne), f)) {
        // Supprimer les retours à la ligne
        ligne[strcspn(ligne, "\r\n")] = '\0';

        // Découper la ligne en tokens avec "/"
        char *nom = strtok(ligne, "/");
        char *id = strtok(NULL, "/");
        char *mail = strtok(NULL, "/");
        char *telephone = strtok(NULL, "/");
        char *mot_de_passe = strtok(NULL, "/");
        char *jour = strtok(NULL, "/");
        char *mois = strtok(NULL, "/");
        char *annee = strtok(NULL, "/");
        char *gouvernorat = strtok(NULL, "/");
        char *sexe = strtok(NULL, "/");

        // Vérifier que toutes les données sont présentes
        if (nom && id && mail && telephone && mot_de_passe && jour && mois && annee && gouvernorat && sexe) {
            // Formater la date de naissance
            char date_naissance[20];
            snprintf(date_naissance, sizeof(date_naissance), "%s/%s/%s", jour, mois, annee);

            // Ajouter les données au modèle
            gtk_list_store_append(store, &iter);
            gtk_list_store_set(store, &iter,
                               0, id,
                               1, nom,
                               2, date_naissance,
                               3, sexe,
                               4, gouvernorat,
                               5, mail,
                               6, telephone,
                               7, mot_de_passe,
                               -1);
        }
    }
    fclose(f);

    // Lier le modèle au TreeView
    gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), GTK_TREE_MODEL(store));

    // Supprimer toutes les colonnes existantes avant d'en ajouter de nouvelles
    while (gtk_tree_view_get_column(GTK_TREE_VIEW(treeview), 0)) {
        gtk_tree_view_remove_column(GTK_TREE_VIEW(treeview), gtk_tree_view_get_column(GTK_TREE_VIEW(treeview), 0));
    }

    // Ajouter les colonnes au TreeView
    GtkCellRenderer *renderer = gtk_cell_renderer_text_new();

    GtkTreeViewColumn *col_id = gtk_tree_view_column_new_with_attributes("ID", renderer, "text", 0, NULL);
    GtkTreeViewColumn *col_nom = gtk_tree_view_column_new_with_attributes("Nom", renderer, "text", 1, NULL);
    GtkTreeViewColumn *col_date_naissance = gtk_tree_view_column_new_with_attributes("Date de naissance", renderer, "text", 2, NULL);
    GtkTreeViewColumn *col_sexe = gtk_tree_view_column_new_with_attributes("Sexe", renderer, "text", 3, NULL);
    GtkTreeViewColumn *col_gouvernorat = gtk_tree_view_column_new_with_attributes("Gouvernorat", renderer, "text", 4, NULL);
    GtkTreeViewColumn *col_mail = gtk_tree_view_column_new_with_attributes("Mail", renderer, "text", 5, NULL);
    GtkTreeViewColumn *col_numero = gtk_tree_view_column_new_with_attributes("Numéro de téléphone", renderer, "text", 6, NULL);
    GtkTreeViewColumn *col_mot_de_passe = gtk_tree_view_column_new_with_attributes("Mot de passe", renderer, "text", 7, NULL);

    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), col_id);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), col_nom);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), col_date_naissance);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), col_sexe);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), col_gouvernorat);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), col_mail);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), col_numero);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), col_mot_de_passe);

    // Libérer la mémoire du modèle
    g_object_unref(store);
}



void
on_button_modifermesinfos_clicked      (GtkButton       *button,
                                        gpointer         user_data)
{
GtkWidget *window2;

   
    window2 = create_window2();
    gtk_widget_show(window2);

}


void
on_button_help1_clicked                (GtkButton       *button,
                                        gpointer         user_data)
{
GtkWidget *window6;

   
    window6 = create_window6();
    gtk_widget_show(window6);
}


void
on_button_help3_clicked                (GtkButton       *button,
                                        gpointer         user_data)
{
GtkWidget *window6;

   
    window6 = create_window6();
    gtk_widget_show(window6);
}


void
on_button_help2_clicked                (GtkButton       *button,
                                        gpointer         user_data)
{
GtkWidget *window6;

   
    window6 = create_window6();
    gtk_widget_show(window6);
}



#include <gtk/gtk.h>

void on_button_envoyer_clicked(GtkButton *button, gpointer user_data) {
    // Créer une boîte de dialogue de type message
    GtkWidget *dialog = gtk_message_dialog_new(
        GTK_WINDOW(user_data),                // Fenêtre parente
        GTK_DIALOG_MODAL,                    // Propriétés de la boîte de dialogue
        GTK_MESSAGE_INFO,                    // Type de message : INFO
        GTK_BUTTONS_OK,                      // Boutons affichés : OK
        "Le problème a été envoyé au service technique." // Message à afficher
    );

    // Afficher la boîte de dialogue et attendre une réponse
    gtk_dialog_run(GTK_DIALOG(dialog));

    // Détruire la boîte de dialogue après fermeture
    gtk_widget_destroy(dialog);
}






void
on_button_tarjee_bienvenue_clicked     (GtkButton       *button,
                                        gpointer         user_data)
{
    GtkWidget *window4;

    // Crée et affiche la nouvelle fenêtre (window2)
    window4 = create_window4();
    gtk_widget_show(window4);

    // Ferme la fenêtre actuelle (window1)
    GtkWidget *window1 = gtk_widget_get_toplevel(GTK_WIDGET(button));
    gtk_widget_destroy(window1);
}


void
on_button_back_compte_clicked          (GtkButton       *button,
                                        gpointer         user_data)
{


    // Ferme la fenêtre actuelle (window1)
    GtkWidget *window2 = gtk_widget_get_toplevel(GTK_WIDGET(button));
    gtk_widget_destroy(window2);

   GtkWidget *window3;

    // Crée et affiche la nouvelle fenêtre (window2)
    window3 = create_window3();
    gtk_widget_show(window3);
}



void
on_button_anuuler_help_clicked         (GtkButton       *button,
                                        gpointer         user_data)
{
 GtkWidget *window6 = gtk_widget_get_toplevel(GTK_WIDGET(button));
    gtk_widget_destroy(window6);
}





void
on_button_fermer_citoyen_clicked       (GtkButton       *button,
                                        gpointer         user_data)
{
    GtkWidget *window4;

    // Crée et affiche la nouvelle fenêtre (window2)
    window4 = create_window4();
    gtk_widget_show(window4);

    // Ferme la fenêtre actuelle (window1)
    GtkWidget *window3 = gtk_widget_get_toplevel(GTK_WIDGET(button));
    gtk_widget_destroy(window3);
}


void
on_button_fermer_compte_clicked        (GtkButton       *button,
                                        gpointer         user_data)
{
    GtkWidget *window4;

    // Crée et affiche la nouvelle fenêtre (window2)
    window4 = create_window4();
    gtk_widget_show(window4);

    // Ferme la fenêtre actuelle (window1)
    GtkWidget *window5 = gtk_widget_get_toplevel(GTK_WIDGET(button));
    gtk_widget_destroy(window5);
}

