#include "../inc/User.h"

#include <string.h>

void populate(User* user, char* buffer) {
  char str[MAX_STR_LEN];
  char* ptr, *nxt_ptr;
  // encontrar id
  ptr = strstr(buffer, "{\"id\":");
  ptr += 6;
  nxt_ptr = strstr(ptr, ",");
  int size = nxt_ptr - ptr;
  int index = 0;
  while (ptr != nxt_ptr) {
    char c = *ptr;
    str[index] = c;
    ++ptr;
    ++index;
  }
  str[++index] = '\0';
  strncpy(user->id, str, size);
  // encontrar name
  index = 0;
  ptr = nxt_ptr + 1;
  ptr = strstr(ptr, "\"name\":");
  ptr += 8;
  nxt_ptr = strstr(ptr, " ");
  size = nxt_ptr - ptr;
  while (ptr != nxt_ptr) {
    char c = *ptr;
    str[index] = c;
    ++ptr;
    ++index;
  }
  str[++index] = '\0';
  strncpy(user->name, str, size);
}
