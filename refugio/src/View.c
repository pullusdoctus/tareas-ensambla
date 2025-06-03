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
