# Informe Tarea 1: Juego de la Vida de Conway en Ensamblador x86

## Integrantes

- Jeremy Rojas, B96804
- Jhonny Leung, C34259

## Aprendizajes

- **Gestión de registros**: Aprendimos a mantener un control riguroso sobre qué valores estaban almacenados en qué registros, lo cual fue esencial para que el programa no se cayera.
- **Manejo manual de memoria**: Como no hay capas de abstracción, fue necesario calcular manualmente las direcciones de memoria, especialmente para indexar correctamente la matriz.
- **Llamadas al sistema**: Aprendimos a interactuar directamente con el sistema operativo usando `int 0x80`, incluyendo operaciones como escribir en la terminal, salir del programa correctamente y controlar el flujo y el tiempo de ejecución.
- **Control de flujo a bajo nivel**: Implementar bucles, condicionales y subrutinas implicó una gestión precisa de saltos (`jmp`) y etiquetas, lo cual nos permitió apreciar la abstracción que ofrecen los lenguajes de alto nivel.

## Dificultades

- **Seguimiento del uso de registros**: Una fuente frecuente de errores fue sobrescribir registros por accidente. Esto causaba errores difíciles de rastrear, especialmente cuando se olvidaban instrucciones como `push`/`pop`.
- **Lógica de acceso a memoria**: Calcular correctamente la dirección de cada celda en la matriz implicó mucho cuidado. Errores por desbordamiento o por índices mal calculados fueron comunes durante el desarrollo.
- **Salida en terminal**: Imprimir la cuadrícula en la terminal requirió experimentar con la salida de caracteres, saltos de línea y buffering, aspectos poco intuitivos en ensamblador.
- **Depuración**: Sin herramientas modernas de depuración o mensajes de error en tiempo de ejecución, resolver errores requería examinar el programa instrucción por instrucción con herramientas como `gdb`.

---

## Conclusión

Implementar el Juego de la Vida de Conway en x86 nos permitió entender mejor el funcionamiento interno de los programas. Nos dejó ver con claridad la importancia de un manejo cuidadoso de la memoria y los registros, y cómo se realizan las operaciones de entrada y salida a un bajo nivel. Finalmente, esta tarea nos ayudó a reforzar nuestro conocimiento de la sintaxis y estructura del ensamblador.
