#include "ui.hpp"
#include <atomic>
#include <chrono>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <thread>

using namespace ftxui;
using namespace std;

void iniciar_interfaz_pomodoro(int work_mins, int rest_mins, int total_cycles) {
    auto screen = ScreenInteractive::TerminalOutput();

    // Variables atómicas para que el hilo del timer y la UI se comuniquen
    atomic<int> ciclo_actual{1};
    atomic<int> segundos_restantes{work_mins * 60};
    atomic<bool> es_descanso{false};
    atomic<bool> ejecutando{true};

    // --- HILO DE LÓGICA (Tu antiguo countdown) ---
    thread timer_thread([&]() {
        while (ejecutando && ciclo_actual <= total_cycles) {
            this_thread::sleep_for(chrono::seconds(1));

            if (segundos_restantes > 0) {
                segundos_restantes--;
            } else {
                // Cambio de estado: Trabajo <-> Descanso
                if (!es_descanso) {
                    es_descanso = true;
                    segundos_restantes = rest_mins * 60;
                } else {
                    es_descanso = false;
                    ciclo_actual++;
                    segundos_restantes = work_mins * 60;
                }
            }
            // Le pedimos a FTXUI que refresque la pantalla
            screen.PostEvent(Event::Custom);
        }
    });

    // --- RENDERIZADO (UI) ---
    auto renderer = Renderer([&] {
        int m = segundos_restantes / 60;
        int s = segundos_restantes % 60;
        float ratio = (float)segundos_restantes /
                      (float)((es_descanso ? rest_mins : work_mins) * 60);

        string tiempo = to_string(m) + ":" + (s < 10 ? "0" : "") + to_string(s);
        string titulo = es_descanso ? " ☕ DESCANSO " : " 💻 TRABAJO ";
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
             text(" [ESC] Salir ") | dim | center});
    });

    // Captura de eventos para cerrar
    auto component = CatchEvent(renderer, [&](Event event) {
        if (event == Event::Escape) {
            ejecutando = false;
            screen.Exit();
            return true;
        }
        return false;
    });

    screen.Loop(component);
    if (timer_thread.joinable())
        timer_thread.join();
}
