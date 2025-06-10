#ifndef APP_H
#define APP_H

#include "View.h"
#include "Location.h"

extern GtkWidget* provincia_combo;
extern GtkWidget* canton_combo;
extern GtkWidget* distrito_combo;
extern struct location_data_t* location_data;

void appStart(void);
void init_location_data(void);

void on_activate(GtkApplication* app, gpointer user_data);
void navigate_to_screen(AppScreen screen);

void on_province_changed(GtkComboBox* combo, gpointer user_data);
void on_canton_changed(GtkComboBox* combo, gpointer user_data);

void on_next_button_clicked(GtkWidget* widget, gpointer data);
void on_previous_button_clicked(GtkWidget* widget, gpointer data);
void on_finish_button_clicked(GtkWidget* widget, gpointer data);
void on_register_button_clicked(GtkWidget* widget, gpointer data);
void on_login_button_clicked(GtkWidget* widget, gpointer data);

void clear_window_content(GtkWidget* window);
void cleanup_address_dropdowns(void);

struct selected_address_t get_selected_address(void);

#endif
