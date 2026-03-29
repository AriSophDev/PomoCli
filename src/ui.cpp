#include "ui.hpp"
#include "storage.hpp"
#include <atomic>
#include <chrono>
#include <cstdlib>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <thread>

using namespace ftxui;
using namespace std;

void enviar_notificacion(std::string titulo, std::string mensaje) {
    string home = std::getenv("HOME");
    string icono = home + "../icon.png";

    // Construimos el comando con el icono personalizado
    string comando =
        "notify-send '" + titulo + "' '" + mensaje + "' -i " + icono + " &";

    std::system(comando.c_str());
}

void iniciar_interfaz_pomodoro(int work_mins, int rest_mins, int total_cycles) {
    auto screen = ScreenInteractive::TerminalOutput();

    // Variables atómicas para que el hilo del timer y la UI se comuniquen
    atomic<bool> pausado{false};
    atomic<int> ciclo_actual{1};
    atomic<int> segundos_restantes{work_mins * 60};
    atomic<bool> es_descanso{false};
    atomic<bool> ejecutando{true};

    // --- HILO DE LÓGICA  ---
    thread timer_thread([&]() {
        while (ejecutando && ciclo_actual <= total_cycles) {
            this_thread::sleep_for(chrono::seconds(1));
            if (!pausado) {
                this_thread::sleep_for(chrono::seconds(1));

                if (segundos_restantes > 0) {
                    segundos_restantes--;
                } else {
                    // Cambio de estado: Trabajo <-> Descanso
                    if (!es_descanso) {
                        es_descanso = true;
                        ciclo_actual++;
                        segundos_restantes = rest_mins * 60;

                        // sonido cuando "termine de trabajar"
                        system("pw-play "
                               "/usr/share/sounds/freedesktop/stereo/"
                               "complete.oga &");
                        enviar_notificacion("PomoCli",
                                            "Tiempo terminado! A descansar,");
                    } else {
                        // sonido de "se acabo el Descanso"
                        system("pw-play "
                               "/usr/share/sounds/freedesktop/stereo/"
                               "complete.oga &");
                        enviar_notificacion("PomoCli",
                                            "Tiempo terminado! A descansar,");
                    }
                }
                // FTXUI que refresque la pantalla
            } else {
                this_thread::sleep_for(chrono::milliseconds(100));
            }
            screen.PostEvent(Event::Custom);
        }

        Storage::guardar_progreso(work_mins, total_cycles);
    });

    // --- RENDERIZADO (UI) ---
    auto renderer = Renderer([&] {
        int m = segundos_restantes / 60;
        int s = segundos_restantes % 60;
        float ratio = (float)segundos_restantes /
                      (float)((es_descanso ? rest_mins : work_mins) * 60);

        string tiempo = to_string(m) + ":" + (s < 10 ? "0" : "") + to_string(s);
        string titulo = es_descanso ? " ☕ DESCANSO " : " 💻 TRABAJO ";
        if (pausado)
            titulo = " (PAUSADO)";

        auto color_tema = es_descanso ? Color::Green : Color::Red;

        return vbox(
            {text(" PomoCli ") | bold | center | border, separator(),
             hbox({
                 text(" Ciclo: ") | dim,
                 text(to_string(ciclo_actual) + "/" + to_string(total_cycles)) |
                     bold,
             }),
             vbox({
                 text(titulo) | center | color(color_tema),
                 text(tiempo) | center | color(color_tema),
                 gauge(ratio) | color(color_tema),
             }) | flex |
                 border,
             text(" [Space] Pausa [S] Skip [Q] Salir ") | dim | center});
    });

    // Captura de eventos para cerrar
    auto component = CatchEvent(renderer, [&](Event event) {
        if (event == Event::Character('S') || event == Event::Character('s')) {
            segundos_restantes = 0; // Forzar cambio de estado
            return true;
        }

        if (event == Event::Character(' ')) {
            pausado = !pausado; // Toggle pausa
            return true;
        }

        if (event == Event::Character('q') || event == Event::Character('Q')) {
            ejecutando = false;
            if (timer_thread.joinable())
                timer_thread.detach(); // Evitar bloqueo al salir
            screen.ExitLoopClosure()();
            return true;
        }

        return false;
    });

    screen.Loop(component);
    if (timer_thread.joinable())
        timer_thread.join();
}
