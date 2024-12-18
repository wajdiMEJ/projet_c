#ifndef CALLBACKS_H
#define CALLBACKS_H

#include <gtk/gtk.h>

// Declaration of the functions
void on_button_ajouter_clicked(GtkButton *button, gpointer user_data);



void
on_button__rechercher_clicked          (GtkButton       *button,
                                        gpointer         user_data);

void
on_radiobutton_homme_toggled           (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_radiobutton_femme_toggled           (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_checkbutton_maintenence_toggled     (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_checkbutton_securite_toggled        (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_checkbutton_agent_toggled           (GtkToggleButton *togglebutton,
                                        gpointer         user_data);


void
on_radiobutton__homme_toggled           (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_radiobutton__femme_toggled           (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_button__modifier_clicked            (GtkButton       *button,
                                        gpointer         user_data);

void
on_button__supprimer__clicked          (GtkButton       *button,
                                        gpointer         user_data);

void
on_button__affrev_clicked              (GtkButton       *button,
                                        gpointer         user_data);

void
on_button2__rechercher_clicked         (GtkButton       *button,
                                        gpointer         user_data);

void
on_button__ajouter_clicked             (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_retour_clicked               (GtkButton       *button,
                                        gpointer         user_data);

void
on_button2__retour__rech_clicked       (GtkButton       *button,
                                        gpointer         user_data);

void
on_button____RECHERCHER__clicked       (GtkButton       *button,
                                        gpointer         user_data);

void
on_button__MODIFIER__clicked           (GtkButton       *button,
                                        gpointer         user_data);

void
on_button2__retour__mod_clicked        (GtkButton       *button,
                                        gpointer         user_data);

void on_radiobutton____homme_toggled(GtkToggleButton *togglebutton, gpointer user_data);
void on_radiobutton____femme_toggled(GtkToggleButton *togglebutton, gpointer user_data);





void
on_radiobutton_____homme_toggled       (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_radiobutton_____femme_toggled       (GtkToggleButton *togglebutton,
                                        gpointer         user_data);




void
on_button___rechercher_clicked         (GtkButton       *button,
                                        gpointer         user_data);

void
on_buttonsearch_clicked                (GtkButton       *button,
                                        gpointer         user_data);

void
on_buttonsup_clicked                   (GtkButton       *button,
                                        gpointer         user_data);

void
on_button3retouur_clicked              (GtkButton       *button,
                                        gpointer         user_data);

void
on_buttonretourlistes_clicked          (GtkButton       *button,
                                        gpointer         user_data);

void
on_buttonlisteagent_clicked            (GtkButton       *button,
                                        gpointer         user_data);

void
on_buttonafficherlaliste__clicked      (GtkButton       *button,
                                        gpointer         user_data);

#endif

void
on_afficherlesagents_clicked           (GtkButton       *button,
                                        gpointer         user_data);
