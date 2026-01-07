#include "load_predictions.hpp"
#include <fstream>
#include <sstream>

std::vector<LoggedPredictions> load_predictions() {
    std::vector<LoggedPredictions> predictions;
    std::ifstream file("ml_logger/predictions.csv");

    std::string line;
    while(std::getline(file, line)) {
        std::stringstream ss(line);
        LoggedPredictions p;
        char comma;

        ss >> p.date >> comma >> p.predicted_delta >> comma >> p.predicted_class;
        predictions.push_back(p);
    }
    return predictions;
}
