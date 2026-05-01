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

    std::string hoy = get_current_date();
    bool encontrado = false;

    for (auto& item : j_list) {
        if (item.contains("date") && item["date"] == hoy) {
            item["work_minutes"] = item.get<Session>().work_minutes + work;
            item["completed_cycles"] = item.get<Session>().completed_cycles + cycles;
            encontrado = true;
            break;
        }
    }

    if (!encontrado) {
        Session nueva = {hoy, work, cycles};
        j_list.push_back(nueva);
    }

    std::ofstream file_out(filename);
    file_out << j_list.dump(4);
}

inline std::string obtener_resumen() {
    std::string filename = "stats.json";
    std::ifstream file(filename);
    if (!file.is_open()) return "No hay datos guardados.";

    json j_list;
    try {
        file >> j_list;
    } catch (...) {
        return "Error al leer estadísticas.";
    }

    int total_minutos = 0;
    int total_ciclos = 0;
    for (const auto& item : j_list) {
        Session s = item.get<Session>();
        total_minutos += s.work_minutes;
        total_ciclos += s.completed_cycles;
    }

    return "Total: " + std::to_string(total_minutos) + " min, " + 
           std::to_string(total_ciclos) + " ciclos completados.";
}
}

#endif
