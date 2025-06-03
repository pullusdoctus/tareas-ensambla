#include "../inc/App.h"
#include "../inc/View.h"

void drawPersonalInfoScreen(GtkWidget* window) {
  GtkWidget* main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
  gtk_container_set_border_width(GTK_CONTAINER(main_box), 20);
  gtk_container_add(GTK_CONTAINER(window), main_box);

  drawHeader(main_box);
  drawTitle(main_box, "1. Información Personal");

  drawInputBox(main_box, "Cédula o Pasaporte", "XX-XXXX-XXXX");

  // caja para acomodar nombres y apellidos a la par
  GtkWidget* name_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
  gtk_box_pack_start(GTK_BOX(main_box), name_box, FALSE, FALSE, 0);

  drawInputBox(name_box, "Nombre", "Su nombre");
  drawInputBox(name_box, "Apellidos", "Sus apellidos");

  drawDropdown(main_box, "Fecha de nacimiento", "YYYY-MM-DD");
  drawInputBox(main_box, "Contraseña", "Su contraseña");

  char* legends[] = {"Masculino", "Femenino", "Otro/Prefiere no decir"};
  drawRadialButtons(main_box, "Género", 3, legends);

  drawPreviousButton(main_box);
  drawNextButton(main_box);
}

void drawContactInfoScreen(GtkWidget* window) {
  GtkWidget* main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
  gtk_container_set_border_width(GTK_CONTAINER(main_box), 20);
  gtk_container_add(GTK_CONTAINER(window), main_box);

  drawHeader(main_box);
  drawTitle(main_box, "2. Información de contacto");

  drawInputBox(main_box, "Teléfono", "XXXX-XXXX");
  drawInputBox(main_box, "Teléfono alternativo", "XXXX-XXXX");

  drawEmailBox(main_box);

  drawPreviousButton(main_box);
  drawNextButton(main_box);
}

void drawAddressInfoScreen(GtkWidget* window) {
  GtkWidget* main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
  gtk_container_set_border_width(GTK_CONTAINER(main_box), 20);
  gtk_container_add(GTK_CONTAINER(window), main_box);

  drawHeader(main_box);
  drawTitle(main_box, "3. Información de domicilio");

  drawAddressDropdowns(main_box);
  drawExactAddressBox(main_box);

  drawPreviousButton(main_box);
  drawNextButton(main_box);
}

void drawHeader(GtkWidget* container) {
  GtkWidget* header_label = gtk_label_new("REFUGIO APP");
  PangoAttrList* attrs = pango_attr_list_new();
  PangoAttribute* attr = pango_attr_weight_new(PANGO_WEIGHT_BOLD);
  pango_attr_list_insert(attrs, attr);
  attr = pango_attr_scale_new(1.5);
  pango_attr_list_insert(attrs, attr);
  gtk_label_set_attributes(GTK_LABEL(header_label), attrs);
  pango_attr_list_unref(attrs);

  gtk_box_pack_start(GTK_BOX(container), header_label, FALSE, FALSE, 10);
}

void drawTitle(GtkWidget* container, const char* title) {
  GtkWidget* title_label = gtk_label_new(title);
  PangoAttrList* attrs = pango_attr_list_new();
  PangoAttribute* attr = pango_attr_weight_new(PANGO_WEIGHT_BOLD);
  pango_attr_list_insert(attrs, attr);
  attr = pango_attr_scale_new(1.2);
  pango_attr_list_insert(attrs, attr);
  gtk_label_set_attributes(GTK_LABEL(title_label), attrs);
  pango_attr_list_unref(attrs);

  gtk_box_pack_start(GTK_BOX(container), title_label, FALSE, FALSE, 5);
}

void drawInputBox(GtkWidget* container, const char* header,
                  const char* placeholder) {
  GtkWidget* vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);

  GtkWidget* label = gtk_label_new(header);
  gtk_label_set_xalign(GTK_LABEL(label), 0.0);
  gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 0);

  GtkWidget* entry = gtk_entry_new();
  gtk_entry_set_placeholder_text(GTK_ENTRY(entry), placeholder);
  gtk_box_pack_start(GTK_BOX(vbox), entry, FALSE, FALSE, 0);

  gtk_box_pack_start(GTK_BOX(container), vbox, FALSE, FALSE, 5);
}

void drawDropdown(GtkWidget* container, const char* header,
                  const char* placeholder) {
  GtkWidget* vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);

  GtkWidget* label = gtk_label_new(header);
  gtk_label_set_xalign(GTK_LABEL(label), 0.0);
  gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 0);

  GtkWidget* combo = gtk_combo_box_text_new_with_entry();
  GtkWidget* entry = gtk_bin_get_child(GTK_BIN(combo));
  gtk_entry_set_placeholder_text(GTK_ENTRY(entry), placeholder);
  gtk_box_pack_start(GTK_BOX(vbox), combo, FALSE, FALSE, 0);

  gtk_box_pack_start(GTK_BOX(container), vbox, FALSE, FALSE, 5);
}

void drawRadialButtons(GtkWidget* container, const char* header,
                       int button_count, char** legends) {
  GtkWidget* vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

  GtkWidget* label = gtk_label_new(header);
  gtk_label_set_xalign(GTK_LABEL(label), 0.0);
  gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 0);

  GSList* group = NULL;
  for (int i = 0; i < button_count; ++i) {
    GtkWidget* radio = gtk_radio_button_new_with_label(group, legends[i]);
    group = gtk_radio_button_get_group(GTK_RADIO_BUTTON(radio));
    gtk_box_pack_start(GTK_BOX(vbox), radio, FALSE, FALSE, 0);
  }

  gtk_box_pack_start(GTK_BOX(container), vbox, FALSE, FALSE, 5);
}

void drawEmailBox(GtkWidget* container) {
  GtkWidget* email_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
  // username input field
  GtkWidget* username_entry = gtk_entry_new();
  gtk_entry_set_placeholder_text(GTK_ENTRY(username_entry), "youremail");
  gtk_widget_set_size_request(username_entry, 150, -1);
  // @ label
  GtkWidget* at_label = gtk_label_new("@");
  // domain input field
  GtkWidget* domain_entry = gtk_entry_new();
  gtk_entry_set_placeholder_text(GTK_ENTRY(domain_entry), "email.com");
  gtk_widget_set_size_request(domain_entry, 100, -1);
  // pack components in box
  gtk_box_pack_start(GTK_BOX(email_box), username_entry, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(email_box), at_label, FALSE, FALSE, 5);
  gtk_box_pack_start(GTK_BOX(email_box), domain_entry, FALSE, FALSE, 0);
  // pack box in container
  gtk_box_pack_start(GTK_BOX(container), email_box, FALSE, FALSE, 10);
}

void drawAddressDropdowns(GtkWidget* container) {
  GtkWidget* dropdowns_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 15);
  // provincia
  GtkWidget* provincia_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  GtkWidget* provincia_label = gtk_label_new("Provincia:");
  gtk_widget_set_halign(provincia_label, GTK_ALIGN_START);
  // lista de provincias
  GtkWidget* provincia_combo = gtk_combo_box_text_new();
  gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(provincia_combo),
                                 "Seleccionar provincia...");
  gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(provincia_combo),
                                 "San José");
  gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(provincia_combo),
                                 "Alajuela");
  gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(provincia_combo),
                                 "Cartago");
  gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(provincia_combo),
                                 "Heredia");
  gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(provincia_combo),
                                 "Guanacaste");
  gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(provincia_combo),
                                 "Puntarenas");
  gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(provincia_combo),
                                 "Limón");
  gtk_combo_box_set_active(GTK_COMBO_BOX(provincia_combo), 0);
  gtk_box_pack_start(GTK_BOX(provincia_box), provincia_label, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(provincia_box), provincia_combo, FALSE, FALSE, 0);
  // cantones
  GtkWidget* canton_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  GtkWidget* canton_label = gtk_label_new("Cantón:");
  gtk_widget_set_halign(canton_label, GTK_ALIGN_START);
  GtkWidget* canton_combo = gtk_combo_box_text_new();
  gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(canton_combo),
                                 "Seleccionar cantón...");
  // TODO: agregar lista de cantones segun provincia
  gtk_combo_box_set_active(GTK_COMBO_BOX(canton_combo), 0);
  gtk_box_pack_start(GTK_BOX(canton_box), canton_label, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(canton_box), canton_combo, FALSE, FALSE, 0);
  // distritos
  GtkWidget* distrito_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  GtkWidget* distrito_label = gtk_label_new("Distrito:");
  gtk_widget_set_halign(distrito_label, GTK_ALIGN_START);
  GtkWidget* distrito_combo = gtk_combo_box_text_new();
  // TODO: agregar lista de distritos segun canton
  gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(distrito_combo),
                                 "Seleccionar distrito...");
  gtk_combo_box_set_active(GTK_COMBO_BOX(distrito_combo), 0);
  gtk_box_pack_start(GTK_BOX(distrito_box), distrito_label, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(distrito_box), distrito_combo, FALSE, FALSE, 0);
  // all boxes in container
  gtk_box_pack_start(GTK_BOX(dropdowns_box), provincia_box, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(dropdowns_box), canton_box, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(dropdowns_box), distrito_box, FALSE, FALSE, 0);
  // dropdown container in main container
  gtk_box_pack_start(GTK_BOX(container), dropdowns_box, FALSE, FALSE, 10);
}

void drawExactAddressBox(GtkWidget* container) {
  GtkWidget* address_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  GtkWidget* address_label = gtk_label_new("Dirección exacta:");
  gtk_widget_set_halign(address_label, GTK_ALIGN_START);
  // scrolled window
  GtkWidget* scrolled_window = gtk_scrolled_window_new(NULL, NULL);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window),
                                 GTK_POLICY_AUTOMATIC,
                                 GTK_POLICY_AUTOMATIC);
  gtk_widget_set_size_request(scrolled_window, 400, 120);
  // for multi-line input
  GtkWidget* text_view = gtk_text_view_new();
  gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text_view), GTK_WRAP_WORD);
  gtk_text_view_set_accepts_tab(GTK_TEXT_VIEW(text_view), FALSE);
  // text view to scrolled window
  gtk_container_add(GTK_CONTAINER(scrolled_window), text_view);
  // components in box
  gtk_box_pack_start(GTK_BOX(address_box), address_label, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(address_box), scrolled_window, TRUE, TRUE, 0);
  // box in container
  gtk_box_pack_start(GTK_BOX(container), address_box, FALSE, FALSE, 10);
}

void drawPreviousButton(GtkWidget* container) {
  GtkWidget* button = gtk_button_new_with_label("Volver");
  gtk_widget_set_halign(button, GTK_ALIGN_START);
  g_signal_connect(button, "clicked",
                   G_CALLBACK(on_previous_button_clicked), NULL);
  gtk_box_pack_start(GTK_BOX(container), button, FALSE, FALSE, 10);
}

void drawNextButton(GtkWidget* container) {
  GtkWidget* button = gtk_button_new_with_label("Siguiente");
  gtk_widget_set_halign(button, GTK_ALIGN_END);
  g_signal_connect(button, "clicked",
                   G_CALLBACK(on_next_button_clicked), NULL);
  gtk_box_pack_start(GTK_BOX(container), button, FALSE, FALSE, 10);
}
