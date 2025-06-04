#ifndef APP_H
#define APP_H

#include "View.h"

void appStart(void);

void on_activate(GtkApplication* app, gpointer user_data);
void navigate_to_screen(AppScreen screen);
void on_next_button_clicked(GtkWidget* widget, gpointer data);
void on_previous_button_clicked(GtkWidget* widget, gpointer data);
void on_register_button_clicked(GtkWidget* widget, gpointer data);
void on_login_button_clicked(GtkWidget* widget, gpointer data);
void clear_window_content(GtkWidget* window);

#endif
