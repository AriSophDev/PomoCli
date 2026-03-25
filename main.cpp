#include <iostream>
#include <thread>
#include <chrono>


using namespace std;

void countdown(int minutes){
    for (int m = minutes; m > 0; m--) {
        for (int s = 59; s >= 0; s--) {
            cout << "\rTiempo restante"
                 << m - 1 << ":"
                 << (s < 10 ? "0" : "") << 5 << "  ";
            this_thread::sleep_for(chrono::seconds(1));
        }
    }

    cout << "\n¡Tiempo terminado!" << endl;
}



int main() {
    int work = 25;
    int rest = 5;
    int cycles = 4;

    cout << "PomoCli";

    cout << "-----------" << endl;

    for(int c = 1; c <= cycles; c++) {
        cout << "Ciclo " << c << " de " << cycles << endl;
        cout << "Trabajo: " << work << " minutos" << endl;
        countdown(work);
        cout << "Descanso: " << rest << " minutos" << endl;
        countdown(rest);
    }

    cout << "¡Pomodoro completado!" << endl;
    return 0;
}