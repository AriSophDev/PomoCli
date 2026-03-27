#include "ui.hpp"

int main() {
    int work = 25;
    int rest = 5;
    int cycles = 4;

    // Llamamos a la interfaz que creamos en ui.cpp
    iniciar_interfaz_pomodoro(work, rest, cycles);

    return 0;
}
