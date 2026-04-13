#include "ui.hpp"
#include "storage.hpp"
#include <atomic>
#include <chrono>
#include <cstdlib>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <memory>
#include <thread>

using namespace ftxui;
using namespace std;

void enviar_notificacion(std::string titulo, std::string mensaje) {
    std::string comando;

#ifdef __APPLE__
    comando = "osascript -e 'display notification \"" + mensaje + 
              "\" with title \"" + titulo + "\"' &";
#else
    std::string home = std::getenv("HOME");
    std::string icono = home + "/.local/share/icons/pomocli.png";
    comando = "notify-send '" + titulo + "' '" + mensaje + "' -i " + icono + " &";
#endif

    std::system(comando.c_str());
}

void iniciar_interfaz_pomodoro(int work_mins, int rest_mins, int total_cycles) {
    auto screen = ScreenInteractive::TerminalOutput();

    // Variables atómicas para que el hilo del timer y la UI se comuniquen
    auto pausado = make_shared<atomic<bool>>(false);
    auto ciclo_actual = make_shared<atomic<int>>(1);
    auto segundos_restantes = make_shared<atomic<int>>(work_mins * 60);
    auto es_descanso = make_shared<atomic<bool>>(false);
    auto ejecutando = make_shared<atomic<bool>>(true);

    // --- HILO DE LÓGICA  ---
    thread timer_thread([=, &screen]() {
        while (*ejecutando && *ciclo_actual <= total_cycles) {
            this_thread::sleep_for(chrono::seconds(1));
            if (!*pausado) {
                if (*segundos_restantes > 0) {
                    (*segundos_restantes)--;
                } else {
                    // Cambio de estado: Trabajo <-> Descanso
                    if (!*es_descanso) {
                        *es_descanso = true;
                        *segundos_restantes = rest_mins * 60;

                        // sonido cuando "termine de trabajar"
#ifdef __APPLE__
                        system("afplay /System/Library/Sounds/Glass.aiff &");
#else
                        system("pw-play /usr/share/sounds/freedesktop/stereo/complete.oga &");
#endif
                        enviar_notificacion("PomoCli", "¡Tiempo terminado! A descansar.");
                    } else {
                        *es_descanso = false;
                        (*ciclo_actual)++;
                        *segundos_restantes = work_mins * 60;
                        // sonido de "se acabo el Descanso"
#ifdef __APPLE__
                        system("afplay /System/Library/Sounds/Glass.aiff &");
#else
                        system("pw-play /usr/share/sounds/freedesktop/stereo/complete.oga &");
#endif
                        enviar_notificacion("PomoCli", "¡Descanso terminado! A trabajar.");
                    }
                }
                screen.PostEvent(Event::Custom);
            }
        }

        Storage::guardar_progreso(work_mins, *ciclo_actual - 1);
    });

    // --- RENDERIZADO (UI) ---
    auto renderer = Renderer([&] {
        int m = *segundos_restantes / 60;
        int s = *segundos_restantes % 60;
        float ratio = (float)*segundos_restantes /
                      (float)((*es_descanso ? rest_mins : work_mins) * 60);

        string tiempo = to_string(m) + ":" + (s < 10 ? "0" : "") + to_string(s);
        string titulo = *es_descanso ? " ☕ DESCANSO " : " 💻 TRABAJO ";
        if (*pausado)
            titulo = " (PAUSADO)";

        auto color_tema = *es_descanso ? Color::Green : Color::Red;

        return vbox(
            {text(" PomoCli ") | bold | center | border, separator(),
             hbox({
                 text(" Ciclo: ") | dim,
                 text(to_string(*ciclo_actual) + "/" + to_string(total_cycles)) |
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
            *segundos_restantes = 0; // Forzar cambio de estado
            return true;
        }

        if (event == Event::Character(' ')) {
            *pausado = !*pausado; // Toggle pausa
            return true;
        }

        if (event == Event::Character('q') || event == Event::Character('Q')) {
            *ejecutando = false;
            screen.ExitLoopClosure()();
            return true;
        }

        return false;
    });

    screen.Loop(component);
    *ejecutando = false;
    if (timer_thread.joinable())
        timer_thread.join();
}
