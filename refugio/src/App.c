#include "../inc/App.h"

#include <stdio.h>
#include <stdlib.h>

void appStart(void) {
  GtkApplication* app =
    gtk_application_new("com.app.refugio", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect(app, "activate", G_CALLBACK(on_activate), NULL);

  g_application_run(G_APPLICATION(app), 0, NULL);
  g_object_unref(app);
}

static void on_activate(GtkApplication* app, gpointer user_data) {
  (void)user_data;

  GtkWidget* window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), "refugio app");
  gtk_window_set_default_size(GTK_WINDOW(window), WIDTH, HEIGHT);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

  drawPersonalInfoScreen(window);

  gtk_widget_show_all(window);
}
