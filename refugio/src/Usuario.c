#include "../inc/Usuario.h"

#include <string.h>

void populate(Usuario* usuario, char* buffer) {
  char str[MAX_STR_LEN];
  char* ptr, comma_ptr;
  // encontrar id
  ptr = strstr(buffer, "{\"id\":");
  ptr += 7;
  comma_ptr = strstr(ptr, ",");
  while (ptr != comma_ptr) {
    char c = *ptr;
    strncat(str, c, 1);
    ++ptr;
  }
  usuario->id = str;
  str = "";
}