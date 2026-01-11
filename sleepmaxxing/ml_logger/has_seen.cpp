#include "has_seen.hpp"
#include <fstream>

bool has_seen_date(const std::string& date) {
    std::ifstream file("ml_logger/training_data.csv");
    if (!file.is_open()) return false;

    std::string line;
    std::getline(file, line);

    while (std::getline(file, line)) {
        if (line.rfind(date + ",", 0) == 0) return true;
    }
    return false;
}
