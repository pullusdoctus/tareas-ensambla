#include "../inc/Usuario.h"

#include <string.h>

void populate(Usuario* usuario, char* buffer) {
  char str[MAX_STR_LEN];
  char* ptr, *comma_ptr;
  // encontrar id
  ptr = strstr(buffer, "{\"id\":");
  ptr += 6;
  comma_ptr = strstr(ptr, ",");
  int size = comma_ptr - ptr;
  while (ptr != comma_ptr) {
    char c = *ptr;
    strncat(str, &c, 1);
    ++ptr;
  }
  strncpy(usuario->id, str, size);
  str[0] = '\0';
}