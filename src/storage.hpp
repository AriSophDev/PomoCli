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

// Macro para serializar el struct
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Session, date, work_minutes,
                                   completed_cycles)

namespace Storage {
// Función para obtener la fecha
static std::string get_current_date() {
    std::time_t t = std::time(nullptr);
    char mbstr[11];
    if (std::strftime(mbstr, sizeof(mbstr), "%Y-%m-%d", std::localtime(&t))) {
        return {mbstr};
    }
    return "unknown";
}

// Cambiamos a static para evitar problemas de enlace múltiple
static void guardar_progreso(int work, int cycles) {
    std::string filename = "stats.json";
    json j_list = json::array();

    std::ifstream file_in(filename);
    if (file_in.is_open()) {
        try {
            file_in >> j_list;
        } catch (...) {
            j_list = json::array();
        }
        file_in.close();
    }

    Session nueva = {get_current_date(), work, cycles};
    j_list.push_back(nueva);

    std::ofstream file_out(filename);
    file_out << j_list.dump(4);
}
} // namespace Storage

#endif
