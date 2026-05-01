#ifndef STORAGE_HPP
#define STORAGE_HPP

#include <ctime>
#include <fstream>
#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

struct Session {
    std::string date;
    int work_minutes;
    int completed_cycles;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Session, date, work_minutes,
                                   completed_cycles)

namespace Storage {

inline std::string get_current_date() {
    std::time_t t = std::time(nullptr);
    char mbstr[11];
    if (std::strftime(mbstr, sizeof(mbstr), "%Y-%m-%d", std::localtime(&t))) {
        return {mbstr};
    }
    return "unknown";
}

inline void guardar_progreso(int work, int cycles) {
    std::string filename = "stats.json";
    std::string hoy = get_current_date();
    Session nueva = {hoy, work, cycles};
    json j = nueva;

    std::ofstream file_out(filename);
    file_out << j.dump(4);
}

inline std::string obtener_resumen() {
    std::string filename = "stats.json";
    std::ifstream file(filename);
    if (!file.is_open()) return "No hay datos guardados.";

    json j;
    try {
        file >> j;
        Session s = j.get<Session>();
        return "Última sesión (" + s.date + "): " + std::to_string(s.work_minutes) + " min, " + 
               std::to_string(s.completed_cycles) + " ciclos completados.";
    } catch (...) {
        return "Error al leer estadísticas o no hay datos.";
    }
}
}

#endif
