#include "ui.hpp"

int main() {
    int work = 1;
    int rest = 1;
    int cycles = 4;

    // Llamamos a la interfaz que creamos en ui.cpp
    iniciar_interfaz_pomodoro(work, rest, cycles);

    return 0;
}
