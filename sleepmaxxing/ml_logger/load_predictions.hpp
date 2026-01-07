#pragma once
#include <vector>
#include <string>


struct LoggedPredictions {
    std::string date;
    double predicted_delta;
    int predicted_class;
};

std::vector<LoggedPredictions> load_predictions();
