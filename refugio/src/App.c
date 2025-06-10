#include "../inc/App.h"

#include <stdio.h>
#include <stdlib.h>

GtkWidget* provincia_combo = NULL;
GtkWidget* canton_combo = NULL;
GtkWidget* distrito_combo = NULL;
struct location_data_t* location_data = NULL;

static GtkWidget* main_window = NULL;
static AppScreen current_screen = SCREEN_PERSONAL_INFO;

void appStart(void) {
  GtkApplication* app =
    gtk_application_new("com.app.refugio", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect(app, "activate", G_CALLBACK(on_activate), NULL);

  g_application_run(G_APPLICATION(app), 0, NULL);
  g_object_unref(app);
}

void on_activate(GtkApplication* app, gpointer user_data) {
  (void)user_data;

  main_window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(main_window), "refugio app");
  gtk_window_set_default_size(GTK_WINDOW(main_window), WIDTH, HEIGHT);
  gtk_window_set_position(GTK_WINDOW(main_window), GTK_WIN_POS_CENTER);

  navigate_to_screen(LOGIN);

  gtk_widget_show_all(main_window);
}

void navigate_to_screen(AppScreen screen) {
  current_screen = screen;
  clear_window_content(main_window);
  switch (screen) {
    case LOGIN:
      drawLoginScreen(main_window);
      break;
    case SCREEN_PERSONAL_INFO:
      drawPersonalInfoScreen(main_window);
      break;
    case SCREEN_CONTACT_INFO:
      drawContactInfoScreen(main_window);
      break;
    case SCREEN_ADDRESS_INFO:
      drawAddressInfoScreen(main_window);
      break;
    case SCREEN_VERIFICATION:
      drawVerificationScreen(main_window);
      break;
    default:
      break;
  }
  gtk_widget_show_all(main_window);
}

void on_province_changed(GtkComboBox* combo, gpointer user_data) {
  (void)user_data;
  gint active = gtk_combo_box_get_active(combo);
  if (active <= 0) { // 0 is "Seleccionar provincia..."
    // Clear canton and distrito dropdowns
    gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(canton_combo));
    gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(distrito_combo));
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(canton_combo), "Seleccionar cantón...");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(distrito_combo), "Seleccionar distrito...");
    gtk_combo_box_set_active(GTK_COMBO_BOX(canton_combo), 0);
    gtk_combo_box_set_active(GTK_COMBO_BOX(distrito_combo), 0);
    return;
  }
  // Find the selected province (active-1 because first item is placeholder)
  if (active-1 < (gint)location_data->province_count) {
    province_t* selected_province = &location_data->provinces[active-1];
    // Clear and populate canton dropdown
    gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(canton_combo));
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(canton_combo), "Seleccionar cantón...");
    for (size_t i = 0; i < selected_province->county_count; i++) {
      gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(canton_combo),
                                   selected_province->counties[i].name);
    }
    gtk_combo_box_set_active(GTK_COMBO_BOX(canton_combo), 0);
    // Clear distrito dropdown
    gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(distrito_combo));
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(distrito_combo), "Seleccionar distrito...");
    gtk_combo_box_set_active(GTK_COMBO_BOX(distrito_combo), 0);
  }
}

// Callback for canton selection change
void on_canton_changed(GtkComboBox* combo, gpointer user_data) {
  (void)user_data;
  gint province_active = gtk_combo_box_get_active(GTK_COMBO_BOX(provincia_combo));
  gint canton_active = gtk_combo_box_get_active(combo);
  if (province_active <= 0 || canton_active <= 0) {
    // Clear distrito dropdown
    gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(distrito_combo));
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(distrito_combo), "Seleccionar distrito...");
    gtk_combo_box_set_active(GTK_COMBO_BOX(distrito_combo), 0);
    return;
  }
  // Find selected province and canton
  if (province_active-1 < (gint)location_data->province_count) {
    province_t* selected_province = &location_data->provinces[province_active-1];
    if (canton_active-1 < (gint)selected_province->county_count) {
      county_t* selected_county = &selected_province->counties[canton_active-1];
      // Clear and populate distrito dropdown
      gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(distrito_combo));
      gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(distrito_combo), "Seleccionar distrito...");
      for (size_t i = 0; i < selected_county->district_count; i++) {
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(distrito_combo), 
                                     selected_county->districts[i].name);
      }
      gtk_combo_box_set_active(GTK_COMBO_BOX(distrito_combo), 0);
    }
  }
}

void on_next_button_clicked(GtkWidget* widget, gpointer data) {
  (void)widget;
  (void)data;
  if (current_screen < SCREEN_COUNT - 1) {
    navigate_to_screen(current_screen + 1);
  }
}

void on_previous_button_clicked(GtkWidget* widget, gpointer data) {
  (void)widget;
  (void)data;
  if (current_screen > 0) {
    navigate_to_screen(current_screen - 1);
  }
}

void on_finish_button_clicked(GtkWidget* widget, gpointer data) {
  (void)widget;
  (void)data;
  GtkApplication* app = gtk_window_get_application(GTK_WINDOW(gtk_widget_get_toplevel(widget)));
  g_application_quit(G_APPLICATION(app));
}

void on_register_button_clicked(GtkWidget* widget, gpointer data) {
  (void)widget;
  (void)data;
  navigate_to_screen(SCREEN_PERSONAL_INFO);
}

void on_login_button_clicked(GtkWidget* widget, gpointer data) {
  (void)widget;
  (void)data;
  return;
}

void clear_window_content(GtkWidget* window) {
  GList* children = gtk_container_get_children(GTK_CONTAINER(window));
  for (GList* iter = children; iter != NULL; iter = g_list_next(iter)) {
    gtk_widget_destroy(GTK_WIDGET(iter->data));
  }
  g_list_free(children);
}

void cleanup_address_dropdowns(void) {
  if (location_data) {
    destroy_location_data(location_data);
    location_data = NULL;
  }
}

struct selected_address_t get_selected_address(void) {
  struct selected_address_t result = {0};
  gint province_active = gtk_combo_box_get_active(GTK_COMBO_BOX(provincia_combo));
  gint canton_active = gtk_combo_box_get_active(GTK_COMBO_BOX(canton_combo));
  gint distrito_active = gtk_combo_box_get_active(GTK_COMBO_BOX(distrito_combo));
  if (province_active > 0 && province_active-1 < (gint)location_data->province_count) {
    struct province_t* province = &location_data->provinces[province_active-1];
    result.province_id = province->id;
    result.province_name = province->name;
    if (canton_active > 0 && canton_active-1 < (gint)province->county_count) {
      struct county_t* county = &province->counties[canton_active-1];
      result.county_id = county->id;
      result.county_name = county->name;
      if (distrito_active > 0 && distrito_active-1 < (gint)county->district_count) {
        struct district_t* district = &county->districts[distrito_active-1];
        result.district_id = district->id;
        result.district_name = district->name;
      }
    }
  }
  return result;
}

void init_location_data(void) {
  if (location_data) return; // Already initialized
  location_data = create_location_data();
  // Example data - replace with actual data loading
  location_data->province_count = 7;
  location_data->provinces = malloc(sizeof(province_t) * 7);
  // San José
  location_data->provinces[0].id = 1;
  location_data->provinces[0].name = create_string_copy("San José");
  location_data->provinces[0].county_count = 1;
  location_data->provinces[0].counties = malloc(sizeof(county_t) * 1);
  // Cantones San José
  location_data->provinces[0].counties[0].id = 11;
  location_data->provinces[0].counties[0].name = create_string_copy("San José");
  location_data->provinces[0].counties[0].province_id = 1;
  location_data->provinces[0].counties[0].district_count = 2;
  location_data->provinces[0].counties[0].districts = malloc(sizeof(district_t) * 2);
  // Distritos San José
  location_data->provinces[0].counties[0].districts[0].id = 111;
  location_data->provinces[0].counties[0].districts[0].name = create_string_copy("Carmen");
  location_data->provinces[0].counties[0].districts[0].county_id = 11;
  location_data->provinces[0].counties[0].districts[1].id = 112;
  location_data->provinces[0].counties[0].districts[1].name = create_string_copy("Merced");
  location_data->provinces[0].counties[0].districts[1].county_id = 11;
  // Alajuela
  location_data->provinces[1].id = 2;
  location_data->provinces[1].name = create_string_copy("Alajuela");
  location_data->provinces[1].county_count = 1;
  location_data->provinces[1].counties = malloc(sizeof(county_t) * 1);
  // Cantones Alajuela
  location_data->provinces[1].counties[0].id = 21;
  location_data->provinces[1].counties[0].name = create_string_copy("Alajuela");
  location_data->provinces[1].counties[0].province_id = 2;
  location_data->provinces[1].counties[0].district_count = 2;
  location_data->provinces[1].counties[0].districts = malloc(sizeof(district_t) * 2);
  // Distritos Alajuela
  location_data->provinces[1].counties[0].districts[0].id = 211;
  location_data->provinces[1].counties[0].districts[0].name = create_string_copy("Alajuela");
  location_data->provinces[1].counties[0].districts[0].county_id = 21;
  location_data->provinces[1].counties[0].districts[1].id = 212;
  location_data->provinces[1].counties[0].districts[1].name = create_string_copy("San José");
  location_data->provinces[1].counties[0].districts[1].county_id = 21;
  // Cartago
  location_data->provinces[2].id = 3;
  location_data->provinces[2].name = create_string_copy("Cartago");
  location_data->provinces[2].county_count = 1;
  location_data->provinces[2].counties = malloc(sizeof(county_t) * 1);
  // Cantones Cartago
  location_data->provinces[2].counties[0].id = 31;
  location_data->provinces[2].counties[0].name = create_string_copy("Cartago");
  location_data->provinces[2].counties[0].province_id = 3;
  location_data->provinces[2].counties[0].district_count = 2;
  location_data->provinces[2].counties[0].districts = malloc(sizeof(district_t) * 2);
  // Distritos Cartago
  location_data->provinces[2].counties[0].districts[0].id = 311;
  location_data->provinces[2].counties[0].districts[0].name = create_string_copy("Oriental");
  location_data->provinces[2].counties[0].districts[0].county_id = 31;
  location_data->provinces[2].counties[0].districts[1].id = 312;
  location_data->provinces[2].counties[0].districts[1].name = create_string_copy("Occidental");
  location_data->provinces[2].counties[0].districts[1].county_id = 31;
  // Heredia
  location_data->provinces[3].id = 4;
  location_data->provinces[3].name = create_string_copy("Heredia");
  location_data->provinces[3].county_count = 1;
  location_data->provinces[3].counties = malloc(sizeof(county_t) * 1);
  // Cantones Heredia
  location_data->provinces[3].counties[0].id = 41;
  location_data->provinces[3].counties[0].name = create_string_copy("Heredia");
  location_data->provinces[3].counties[0].province_id = 4;
  location_data->provinces[3].counties[0].district_count = 2;
  location_data->provinces[3].counties[0].districts = malloc(sizeof(district_t) * 2);
  // Distritos Heredia
  location_data->provinces[3].counties[0].districts[0].id = 411;
  location_data->provinces[3].counties[0].districts[0].name = create_string_copy("Heredia");
  location_data->provinces[3].counties[0].districts[0].county_id = 41;
  location_data->provinces[3].counties[0].districts[1].id = 412;
  location_data->provinces[3].counties[0].districts[1].name = create_string_copy("Mercedes");
  location_data->provinces[3].counties[0].districts[1].county_id = 41;
  // Guanacaste
  location_data->provinces[4].id = 5;
  location_data->provinces[4].name = create_string_copy("Guanacaste");
  location_data->provinces[4].county_count = 1;
  location_data->provinces[4].counties = malloc(sizeof(county_t) * 1);
  // Cantones Guanacaste
  location_data->provinces[4].counties[0].id = 51;
  location_data->provinces[4].counties[0].name = create_string_copy("Liberia");
  location_data->provinces[4].counties[0].province_id = 5;
  location_data->provinces[4].counties[0].district_count = 2;
  location_data->provinces[4].counties[0].districts = malloc(sizeof(district_t) * 2);
  // Distritos Guanacaste
  location_data->provinces[4].counties[0].districts[0].id = 511;
  location_data->provinces[4].counties[0].districts[0].name = create_string_copy("Liberia");
  location_data->provinces[4].counties[0].districts[0].county_id = 51;
  location_data->provinces[4].counties[0].districts[1].id = 512;
  location_data->provinces[4].counties[0].districts[1].name = create_string_copy("Cañas Dulces");
  location_data->provinces[4].counties[0].districts[1].county_id = 51;
  // Puntarenas
  location_data->provinces[5].id = 6;
  location_data->provinces[5].name = create_string_copy("Puntarenas");
  location_data->provinces[5].county_count = 1;
  location_data->provinces[5].counties = malloc(sizeof(county_t) * 1);
  // Cantones Puntarenas
  location_data->provinces[5].counties[0].id = 61;
  location_data->provinces[5].counties[0].name = create_string_copy("Puntarenas");
  location_data->provinces[5].counties[0].province_id = 6;
  location_data->provinces[5].counties[0].district_count = 2;
  location_data->provinces[5].counties[0].districts = malloc(sizeof(district_t) * 2);
  // Distritos Puntarenas
  location_data->provinces[5].counties[0].districts[0].id = 611;
  location_data->provinces[5].counties[0].districts[0].name = create_string_copy("Puntarenas");
  location_data->provinces[5].counties[0].districts[0].county_id = 61;
  location_data->provinces[5].counties[0].districts[1].id = 612;
  location_data->provinces[5].counties[0].districts[1].name = create_string_copy("Pitahaya");
  location_data->provinces[5].counties[0].districts[1].county_id = 61;
  // Limón
  location_data->provinces[6].id = 7;
  location_data->provinces[6].name = create_string_copy("Limón");
  location_data->provinces[6].county_count = 1;
  location_data->provinces[6].counties = malloc(sizeof(county_t) * 1);
  // Cantones Limón
  location_data->provinces[6].counties[0].id = 71;
  location_data->provinces[6].counties[0].name = create_string_copy("Limón");
  location_data->provinces[6].counties[0].province_id = 7;
  location_data->provinces[6].counties[0].district_count = 2;
  location_data->provinces[6].counties[0].districts = malloc(sizeof(district_t) * 2);
  // Distritos Limón
  location_data->provinces[6].counties[0].districts[0].id = 711;
  location_data->provinces[6].counties[0].districts[0].name = create_string_copy("Limón");
  location_data->provinces[6].counties[0].districts[0].county_id = 71;
  location_data->provinces[6].counties[0].districts[1].id = 712;
  location_data->provinces[6].counties[0].districts[1].name = create_string_copy("Valle La Estrella");
  location_data->provinces[6].counties[0].districts[1].county_id = 71;
}

