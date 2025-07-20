# Compilación

Se provee un Makefile que realiza todo el trabajo. Es necesario tener instalado NASM.

Ademas se necesita instalar SDL2 y SDL2 TTF


## Lista de comandos

- `sudo apt-get install libsdl2-dev`: instala sdl2 para Ubuntu / Debian
- `sudo dnf install SDL2-devel`: instala sdl2 para Fedora
- `sudo pacman -S sdl2`: instala sdl2 para Arch Linux
- `sudo apt-get install libsdl2-ttf-dev`: instala sdl2 TTF
- `make`: compila el ejecutable.
- `make run`: compila (si es necesario) el ejecutable y lo ejecuta.
- `make clean`: elimina los archivos generados durante la compilación.
