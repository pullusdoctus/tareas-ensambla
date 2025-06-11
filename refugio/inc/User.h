#ifndef USUARIO_H
#define USUARIO_H

#define MAX_STR_LEN 2048

typedef struct {
  // informacion personal
  char id[10];                          // id
  char name[MAX_STR_LEN];               // name
  char surnames[MAX_STR_LEN];           // type
  int birth_year;                       // created
  int birth_month;
  int birth_day;
  char password[MAX_STR_LEN];           // status
  char gender[MAX_STR_LEN];             // gender
  // contacto
  char phone_number[MAX_STR_LEN];       // 8080 8080
  char alt_phone_number[MAX_STR_LEN];   // 4040 4040
  char email[MAX_STR_LEN];              // jerry@gmail.com
  // domicilio
  char province[MAX_STR_LEN];           // san jose
  char canton[MAX_STR_LEN];             // san jose
  char district[MAX_STR_LEN];           // san jose
  char exact_address[MAX_STR_LEN];      // origin.name
} User;

void populate(User* user, char* buffer);

#endif
