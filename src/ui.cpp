#include "ui.hpp"
#include "storage.hpp"
#include <atomic>
#include <chrono>
#include <cstdlib>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <thread>
#include <string>

void enviar_notificacion(std::string titulo, std::string mensaje) {
    const char* home_env = std::getenv("HOME");
    std::string home = home_env ? home_env : "";
    std::string icono = home + "/.local/share/icons/pomocli.png";

    // Construimos el comando con el icono personalizado
    std::string comando = "notify-send '" + titulo + "' '" + mensaje + "' -i " + icono + " &";

    std::system(comando.c_str());
}

void iniciar_interfaz_pomodoro(int work_mins, int rest_mins, int total_cycles) {
    auto screen = ftxui::ScreenInteractive::TerminalOutput();

    // Variables atómicas para que el hilo del timer y la UI se comuniquen
    std::atomic<bool> pausado{false};
    std::atomic<int> ciclo_actual{1};
    std::atomic<int> segundos_restantes{work_mins * 60};
    std::atomic<bool> es_descanso{false};
    std::atomic<bool> ejecutando{true};

    // --- HILO DE LÓGICA  ---
    std::thread timer_thread([&]() {
        while (ejecutando.load() && ciclo_actual.load() <= total_cycles) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            if (!pausado.load()) {
                if (segundos_restantes.load() > 0) {
                    segundos_restantes--;
                } else {
                    // Cambio de estado: Trabajo <-> Descanso
                    if (!es_descanso.load()) {
                        es_descanso = true;
                        segundos_restantes = rest_mins * 60;

                        // sonido cuando "termine de trabajar"
                        std::system("pw-play /usr/share/sounds/freedesktop/stereo/complete.oga &");
                        enviar_notificacion("PomoCli", "Tiempo terminado! A descansar.");
                    } else {
                        es_descanso = false;
                        ciclo_actual++;
                        segundos_restantes = work_mins * 60;
                        // sonido de "se acabo el Descanso"
                        std::system("pw-play /usr/share/sounds/freedesktop/stereo/complete.oga &");
                        enviar_notificacion("PomoCli", "Tiempo de descanso terminado! A trabajar.");
                    }
                }
            } else {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
            screen.PostEvent(ftxui::Event::Custom);
        }

        Storage::guardar_progreso(work_mins, ciclo_actual.load() - (es_descanso ? 0 : 1));
    });

    // --- RENDERIZADO (UI) ---
    auto renderer = ftxui::Renderer([&] {
        int r = segundos_restantes.load();
        int m = r / 60;
        int s = r % 60;
        float ratio = (float)r / (float)((es_descanso.load() ? rest_mins : work_mins) * 60);

        std::string tiempo = std::to_string(m) + ":" + (s < 10 ? "0" : "") + std::to_string(s);
        std::string titulo = es_descanso.load() ? " ☕ DESCANSO " : " 💻 TRABAJO ";
        if (pausado.load())
            titulo = " (PAUSADO)";

        auto color_tema = es_descanso.load() ? ftxui::Color::Green : ftxui::Color::Red;

        return ftxui::vbox(
            {ftxui::text(" PomoCli ") | ftxui::bold | ftxui::center | ftxui::border, ftxui::separator(),
             ftxui::hbox({
                 ftxui::text(" Ciclo: ") | ftxui::dim,
                 ftxui::text(std::to_string(ciclo_actual.load()) + "/" + std::to_string(total_cycles)) | ftxui::bold,
             }),
             ftxui::vbox({
                 ftxui::text(titulo) | ftxui::center | ftxui::color(color_tema),
                 ftxui::text(tiempo) | ftxui::center | ftxui::color(color_tema),
                 ftxui::gauge(ratio) | ftxui::color(color_tema),
             }) | ftxui::flex | ftxui::border,
             ftxui::text(" [Space] Pausa [S] Skip [Q] Salir ") | ftxui::dim | ftxui::center});
    });

    // Captura de eventos para cerrar
    auto component = ftxui::CatchEvent(renderer, [&](ftxui::Event event) {
        if (event == ftxui::Event::Character('S') || event == ftxui::Event::Character('s')) {
            segundos_restantes = 0; // Forzar cambio de estado
            return true;
        }

        if (event == ftxui::Event::Character(' ')) {
            pausado = !pausado.load(); // Toggle pausa
            return true;
        }

        if (event == ftxui::Event::Character('q') || event == ftxui::Event::Character('Q')) {
            ejecutando = false;
            screen.ExitLoopClosure()();
            return true;
        }

        return false;
    });

    screen.Loop(component);
    if (timer_thread.joinable())
        timer_thread.join();
}
