#ifndef VIEW_H
#define VIEW_H

#include <gtk/gtk.h>

#define WIDTH 1280
#define HEIGHT 720

typedef enum {
  SCREEN_PERSONAL_INFO = 0,
  SCREEN_CONTACT_INFO,
  SCREEN_ADDRESS_INFO,
  SCREEN_VERIFICATION,
  SCREEN_COUNT
} AppScreen;

void drawPersonalInfoScreen(GtkWidget* window);
void drawContactInfoScreen(GtkWidget* window);
void drawAddressInfoScreen(GtkWidget* window);
void drawVerificationScreen(GtkWidget* window);

void drawHeader(GtkWidget* container);
void drawTitle(GtkWidget* container, const char* title);
void drawInputBox(GtkWidget* container, const char* header, const char* placeholder);
void drawDropdown(GtkWidget* container, const char* header, const char* placeholder);
void drawRadialButtons(GtkWidget* container, const char* header, int button_count, char** legends);
void drawEmailBox(GtkWidget* container);
void drawAddressDropdowns(GtkWidget* container);
void drawExactAddressBox(GtkWidget* container);
void drawPreviousButton(GtkWidget* container);
void drawNextButton(GtkWidget* container);

#endif
