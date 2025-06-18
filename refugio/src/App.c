#include "../inc/App.h"

#include <stdio.h>
#include <stdlib.h>

// Personal Info widgets
GtkWidget* id_entry = NULL;
GtkWidget* name_entry = NULL;
GtkWidget* surnames_entry = NULL;
GtkWidget* password_entry = NULL;
GtkWidget* birth_combo = NULL;
GtkWidget* gender_radios[3];

// Contact Info widgets
GtkWidget* phone_entry = NULL;
GtkWidget* alt_phone_entry = NULL;
GtkWidget* email_username_entry = NULL;
GtkWidget* email_domain_entry = NULL;

// Address Info widgets
GtkWidget* exact_address_textview = NULL;

GtkWidget* provincia_combo = NULL;
GtkWidget* canton_combo = NULL;
GtkWidget* distrito_combo = NULL;
struct location_data_t* location_data = NULL;

static GtkWidget* main_window = NULL;
static AppScreen current_screen = SCREEN_PERSONAL_INFO;
User current_user;

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
  init_form_widgets();

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
    switch (current_screen) {
      case SCREEN_PERSONAL_INFO:
        collect_personal_info();
        break;
      case SCREEN_CONTACT_INFO:
        collect_contact_info();
        break;
      case SCREEN_ADDRESS_INFO:
        collect_address_info();
        break;
      default:
        break;
    }
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
  print_user_data(&current_user);
  cleanup_form_widgets();
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

province_t* find_or_add_province(int id, const char* name) {
  for (int i = 0; i < (int)location_data->province_count; ++i) {
    if (location_data->provinces[i].id == id) {
      return &location_data->provinces[i];
    }
  }
  // Add new province
  location_data->provinces = realloc(location_data->provinces,
      sizeof(province_t) * (location_data->province_count + 1));
  province_t* prov = &location_data->provinces[location_data->province_count++];
  prov->id = id;
  prov->name = create_string_copy(name);
  prov->county_count = 0;
  prov->counties = NULL;
  return prov;
}

county_t* find_or_add_county(province_t* prov, int id, const char* name) {
  for (int i = 0; i < (int)prov->county_count; ++i) {
    if (prov->counties[i].id == id) {
      return &prov->counties[i];
    }
  }
  // Add new county
  prov->counties = realloc(prov->counties,
      sizeof(county_t) * (prov->county_count + 1));
  county_t* county = &prov->counties[prov->county_count++];
  county->id = id;
  county->province_id = prov->id;
  county->name = create_string_copy(name);
  county->district_count = 0;
  county->districts = NULL;
  return county;
}

void add_district(county_t* county, int id, const char* name) {
  county->districts = realloc(county->districts,
      sizeof(district_t) * (county->district_count + 1));
  district_t* dist = &county->districts[county->district_count++];
  dist->id = id;
  dist->name = create_string_copy(name);
  dist->county_id = county->id;
}

void init_location_data(void) {
  if (location_data) return;
  location_data = malloc(sizeof(struct location_data_t));
  location_data->province_count = 0;
  location_data->provinces = NULL;
  FILE* file = fopen("locations.csv", "r");
  if (!file) {
    perror("Error opening CSV file");
    exit(EXIT_FAILURE);
  }
  char line[MAX_LINE_LENGTH];
  while (fgets(line, sizeof(line), file)) {
    int pid, cid, did;
    char pname[64], cname[64], dname[64];
    if (sscanf(line, "%d,%63[^,],%d,%63[^,],%d,%63[^\n]",
               &pid, pname, &cid, cname, &did, dname) == 6) {
      province_t* prov = find_or_add_province(pid, pname);
      county_t* county = find_or_add_county(prov, cid, cname);
      add_district(county, did, dname);
    }
  }
  fclose(file);
}

void collect_personal_info(void) {
  // Clear the user struct
  memset(&current_user, 0, sizeof(User));
  // Personal Info - with NULL checks
  if (id_entry && GTK_IS_ENTRY(id_entry)) {
    const char* id_text = gtk_entry_get_text(GTK_ENTRY(id_entry));
    if (id_text) {
      strncpy(current_user.id, id_text, sizeof(current_user.id) - 1);
      current_user.id[sizeof(current_user.id) - 1] = '\0'; // Ensure null termination
    }
  }
  if (name_entry && GTK_IS_ENTRY(name_entry)) {
    const char* name_text = gtk_entry_get_text(GTK_ENTRY(name_entry));
    if (name_text) {
      strncpy(current_user.name, name_text, sizeof(current_user.name) - 1);
      current_user.name[sizeof(current_user.name) - 1] = '\0';
    }
  }
  if (surnames_entry && GTK_IS_ENTRY(surnames_entry)) {
    const char* surnames_text = gtk_entry_get_text(GTK_ENTRY(surnames_entry));
    if (surnames_text) {
      strncpy(current_user.surnames, surnames_text, sizeof(current_user.surnames) - 1);
      current_user.surnames[sizeof(current_user.surnames) - 1] = '\0';
    }
  }
  if (password_entry && GTK_IS_ENTRY(password_entry)) {
    const char* password_text = gtk_entry_get_text(GTK_ENTRY(password_entry));
    if (password_text) {
      strncpy(current_user.password, password_text, sizeof(current_user.password) - 1);
      current_user.password[sizeof(current_user.password) - 1] = '\0';
    }
  }
  // Birth date with NULL checks
  if (birth_combo && GTK_IS_COMBO_BOX(birth_combo)) {
    GtkWidget* child = gtk_bin_get_child(GTK_BIN(birth_combo));
    if (child && GTK_IS_ENTRY(child)) {
      const char* birth_text = gtk_entry_get_text(GTK_ENTRY(child));
      if (birth_text && strlen(birth_text) >= 10) {
        // Simple parsing assuming YYYY-MM-DD format
        char year_str[5], month_str[3], day_str[3];
        memset(year_str, 0, sizeof(year_str));
        memset(month_str, 0, sizeof(month_str));
        memset(day_str, 0, sizeof(day_str));
        strncpy(year_str, birth_text, 4);
        strncpy(month_str, birth_text + 5, 2);
        strncpy(day_str, birth_text + 8, 2);
        current_user.birth_year = atoi(year_str);
        current_user.birth_month = atoi(month_str);
        current_user.birth_day = atoi(day_str);
      }
    }
  }
  // Gender from radio buttons with NULL checks
  for (int i = 0; i < 3; i++) {
    if (gender_radios[i] && GTK_IS_RADIO_BUTTON(gender_radios[i]) && 
        gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gender_radios[i]))) {
      switch(i) {
        case 0: 
          strncpy(current_user.gender, "Masculino", sizeof(current_user.gender) - 1);
          break;
        case 1: 
          strncpy(current_user.gender, "Femenino", sizeof(current_user.gender) - 1);
          break;
        case 2: 
          strncpy(current_user.gender, "Otro/Prefiere no decir", sizeof(current_user.gender) - 1);
          break;
      }
      current_user.gender[sizeof(current_user.gender) - 1] = '\0';
      break;
    }
  }
}

void collect_contact_info(void) {
  // Contact Info with NULL checks
  if (phone_entry && GTK_IS_ENTRY(phone_entry)) {
    const char* phone_text = gtk_entry_get_text(GTK_ENTRY(phone_entry));
    if (phone_text) {
      strncpy(current_user.phone_number, phone_text, sizeof(current_user.phone_number) - 1);
      current_user.phone_number[sizeof(current_user.phone_number) - 1] = '\0';
    }
  }
  if (alt_phone_entry && GTK_IS_ENTRY(alt_phone_entry)) {
    const char* alt_phone_text = gtk_entry_get_text(GTK_ENTRY(alt_phone_entry));
    if (alt_phone_text) {
      strncpy(current_user.alt_phone_number, alt_phone_text, sizeof(current_user.alt_phone_number) - 1);
      current_user.alt_phone_number[sizeof(current_user.alt_phone_number) - 1] = '\0';
    }
  }
  // Email with NULL checks
  if (email_username_entry && GTK_IS_ENTRY(email_username_entry) && 
      email_domain_entry && GTK_IS_ENTRY(email_domain_entry)) {
    const char* username = gtk_entry_get_text(GTK_ENTRY(email_username_entry));
    const char* domain = gtk_entry_get_text(GTK_ENTRY(email_domain_entry));
    if (username && domain) {
      snprintf(current_user.email, sizeof(current_user.email), "%s@%s", username, domain);
      current_user.email[sizeof(current_user.email) - 1] = '\0';
    }
  }
}

void collect_address_info(void) {
  // Address Info with NULL checks
  struct selected_address_t address = get_selected_address();
  if (address.province_name) {
    strncpy(current_user.province, address.province_name, sizeof(current_user.province) - 1);
    current_user.province[sizeof(current_user.province) - 1] = '\0';
  }
  if (address.county_name) {
    strncpy(current_user.canton, address.county_name, sizeof(current_user.canton) - 1);
    current_user.canton[sizeof(current_user.canton) - 1] = '\0';
  }
  if (address.district_name) {
    strncpy(current_user.district, address.district_name, sizeof(current_user.district) - 1);
    current_user.district[sizeof(current_user.district) - 1] = '\0';
  }
  // Exact address from text view with NULL checks
  if (exact_address_textview && GTK_IS_TEXT_VIEW(exact_address_textview)) {
    GtkTextBuffer* buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(exact_address_textview));
    if (buffer && GTK_IS_TEXT_BUFFER(buffer)) {
      GtkTextIter start, end;
      gtk_text_buffer_get_bounds(buffer, &start, &end);
      char* exact_address_text = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);
      if (exact_address_text) {
        strncpy(current_user.exact_address, exact_address_text, sizeof(current_user.exact_address) - 1);
        current_user.exact_address[sizeof(current_user.exact_address) - 1] = '\0';
        g_free(exact_address_text);
      }
    }
  }
}

void print_user_data(const User* user) {
  printf("\n=== USER REGISTRATION DATA ===\n");
  printf("ID: %s\n", user->id);
  printf("Name: %s\n", user->name);
  printf("Surnames: %s\n", user->surnames);
  printf("Birth Date: %04d-%02d-%02d\n", user->birth_year, user->birth_month, user->birth_day);
  printf("Password: %s\n", user->password);
  printf("Gender: %s\n", user->gender);
  printf("Phone: %s\n", user->phone_number);
  printf("Alt Phone: %s\n", user->alt_phone_number);
  printf("Email: %s\n", user->email);
  printf("Province: %s\n", user->province);
  printf("Canton: %s\n", user->canton);
  printf("District: %s\n", user->district);
  printf("Exact Address: %s\n", user->exact_address);
  printf("==============================\n\n");
}

void init_form_widgets() {
  id_entry = gtk_entry_new();
  name_entry = gtk_entry_new();
  surnames_entry = gtk_entry_new();
  password_entry = gtk_entry_new();
  birth_combo = gtk_combo_box_text_new();
  phone_entry = gtk_entry_new();
  alt_phone_entry = gtk_entry_new();
  email_username_entry = gtk_entry_new();
  email_domain_entry = gtk_entry_new();
  exact_address_textview = gtk_text_view_new();
  for (int i = 0; i < 3; i++) {
    gender_radios[i] = gtk_radio_button_new_with_label(
      i == 0 ? NULL : gtk_radio_button_get_group(GTK_RADIO_BUTTON(gender_radios[0])),
      (i == 0) ? "Male" : (i == 1) ? "Female" : "Other"
    );
  }
}

void cleanup_form_widgets() {
  id_entry = NULL;
  name_entry = NULL;
  surnames_entry = NULL;
  password_entry = NULL;
  birth_combo = NULL;
  phone_entry = NULL;
  alt_phone_entry = NULL;
  email_username_entry = NULL;
  email_domain_entry = NULL;
  exact_address_textview = NULL;
  for (int i = 0; i < 3; i++) {
    gender_radios[i] = NULL;
  }
}
