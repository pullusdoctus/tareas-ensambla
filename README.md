# tareas-ensambla

- [ ] Celda poblada
- [ ] Celda no poblada
- [ ] Inicio (cargar matriz, imprimir matriz)

- [ ] Fin (chequear próxima celda, preguntar si quiere terminar)

int main() {
  cargarMatriz();
  imprimirMatriz();
  while (true) {
   if (!quiereSeguir()) {
    break;
  }
  for  (...) {
    for (...) {
      if (celdaPoblada) {
        celdaPoblada();
      } else {
        celdaNoPoblada();
      }
    }
  }
  imprimirMatriz();
  }
}
