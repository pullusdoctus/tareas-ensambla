#ifndef APP_H
#define APP_H

#include "View.h"
#include "Location.h"
#include "User.h"

// Personal Info widgets
extern GtkWidget* id_entry;
extern GtkWidget* name_entry;
extern GtkWidget* surnames_entry;
extern GtkWidget* password_entry;
extern GtkWidget* birth_combo;
extern GtkWidget* gender_radios[3];

// Contact Info widgets
extern GtkWidget* phone_entry;
extern GtkWidget* alt_phone_entry;
extern GtkWidget* email_username_entry;
extern GtkWidget* email_domain_entry;

// Address Info widgets
extern GtkWidget* exact_address_textview;

extern GtkWidget* provincia_combo;
extern GtkWidget* canton_combo;
extern GtkWidget* distrito_combo;
extern struct location_data_t* location_data;

extern User current_user;

#define MAX_LINE_LENGTH 512

void appStart(void);
void init_location_data(void);
void init_widget_references(void);
province_t* find_or_add_province(int id, const char* name);
county_t* find_or_add_county(province_t* prov, int id, const char* name);
void add_district(county_t* county, int id, const char* name);
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

void collect_user_data(void);
void print_user_data(const User* user);

#endif
