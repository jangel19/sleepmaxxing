#include "load_errors.hpp"
#include <fstream>
#include <sstream>
#include <string>

std::vector<double> load_pred_errors() {
    std::vector<double> errors;
    std::ifstream file("ml_logger/errors.csv");

    if (!file.is_open()) return errors;

    std::string line;
    std::getline(file, line);

    while(std::getline(file, line)) {
        std::stringstream ss(line);
        std::string date, pred, actual, error;

        std::getline(ss, date, ',');
        std::getline(ss, pred, ',');
        std::getline(ss, actual, ',');
        std::getline(ss, error, ',');

        try {
            errors.push_back(std::stod(error));
        } catch (...) {}
    }
    return errors;
}
