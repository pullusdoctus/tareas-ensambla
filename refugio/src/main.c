#include "../inc/App.h"
#include "../inc/requests.h"
#include "../inc/Usuario.h"

int main(void) {
  char* url = "https://rickandmortyapi.com/api/character/175";
  char buffer[1025];
  RequestsHandler* handler = NULL;
  int size;
  req_init();
  handler = req_get(NULL, url, "");
  if (handler) {
    req_display_headers(handler);
    while ((size = req_read_output_body(handler, buffer, 1024)) > 0) {
      buffer[size] = '\0';
      printf("%s", buffer);
    }
    printf("\n");
    req_close_connection(&handler);
  } else {
    int error = req_get_last_error();
    printf("Failed to get data.\nError: %d", error);
  }
  Usuario usuario;
  populate(&usuario, buffer);
  printf("%s\n", usuario.id);
  req_cleanup();
  return 0;
}

/*
int main(void) {
  appStart();
  return 0;
}*/

// TODO: generar myObj.name de la respuesta json
// avisarle al profe para poder hacer post al servidor
