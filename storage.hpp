#include <fstream>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

using json = nlohmann::json;

struct Session {
    std::string date;
    int work_minutes;
    int completed_cycles;
};

// Para que la librería sepa cómo convertir tu struct a JSON automáticamente
void to_json(json &j, const Session &s) {
    j = json{{"date", s.date},
             {"work_mins", s.work_minutes},
             {"cycles", s.completed_cycles}};
}

void guardar_sesion(const Session &s) {
    json j_list;
    std::ifstream file("stats.json");

    // Si el archivo ya existe, leemos lo anterior
    if (file.is_open()) {
        file >> j_list;
        file.close();
    }

    j_list.push_back(s);

    std::ofstream out("stats.json");
    out << j_list.dump(
        4); // El 4 es para que el JSON sea legible (pretty print)
}
