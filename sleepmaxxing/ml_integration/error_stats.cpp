#include "error_stats.hpp"
#include <fstream>
#include <sstream>
#include <string>


double load_mean_error() {
    std::ifstream file("ml_logger/errors.csv");
    std::string line;
    double sum = 0;
    int count = 0;

    if (!file.is_open()) return 0.0;

    std::getline(file, line);

    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string cell;

        std::getline(ss, cell, ',');
        std::getline(ss, cell, ',');
        std::getline(ss, cell, ',');
        std::getline(ss, cell, ',');
        try {
            double error = std::stod(cell);
            sum += error;
            count++;
        } catch (...) { continue; }
    }

    return count ? sum / count : 0.0;
}
