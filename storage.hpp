#ifndef STORAGE_HPP
#define STORAGE_HPP

#include <ctime>
#include <fstream>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

using namespace std;
using json = nlohmann::json;

struct Session {
    std::string date;
    int work_minutes;
    int completed_cycles;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Session, date, work_minutes,
                                   completed_cycles)

namespace Storage {

inline string get_current_date() {
    time_t now = time(0);
    char mbstr[11];
    if (strftime(mbstr, sizeof(mbstr), "%Y-%m-%d", localtime(&now))) {
        return string(mbstr);
    }
    return "unknown";
}

inline void save_session(const Session &session) {
    string filename = "sessions.json";
    json j_list = json::array();

    ifstream file_in(filename);
    if (file_in.is_open()) {
        try {
            file_in >> j_list;
        } catch (...) {
            j_list = json::array();
        }
        file_in.close();
    }

    Session nueva = {get_current_date(), session.work_minutes,
                     session.completed_cycles};
    j_list.push_back(nueva);

    ofstream file_out(filename);
    file_out << j_list.dump(4);
}
}

#endif
