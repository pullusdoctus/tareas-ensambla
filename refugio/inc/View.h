#include <gtk/gtk.h>

#define WIDTH 1280
#define HEIGHT 720

void drawPersonalInfoScreen(GtkWidget* window);
// TODO:
// void drawContactInfoScreen();
// void drawAddressInfoScreen();
// void drawVerificationScreen();

void drawHeader(GtkWidget* container);
void drawTitle(GtkWidget* container, const char* title);
void drawInputBox(GtkWidget* container, const char* header, const char* placeholder);
void drawDropdown(GtkWidget* container, const char* header, const char* placeholder);
void drawRadialButtons(GtkWidget* container, const char* header, int button_count, char** legends);
void drawNextButton(GtkWidget* container);
// TODO:
// void drawButton();
