#include "logger.hpp"

void log_predictions(const std::string &date, double predicted_delta, int predicted_class)
{
    std::ofstream file("ml_logger/predictions.csv", std::ios::app);
    file << date << ", " << predicted_delta << ", " << predicted_class << std::endl;
}

void log_error(const std::string& date, double predicted, double actual, double error) {
    std::ofstream file("ml_logger/errors.csv", std::ios::app);
    file << date << ", " << predicted << ", " << actual << "," << error << std::endl;
}

void log_training_data(const std::string& date, const arma::vec& x, double actual_delta) {
    std::ofstream file("ml_logger/training_data.csv", std::ios::app);

    file << date;
    for (size_t i = 0; i < x.n_elem; i++) {
        file << ", " << x(i);
    }
    file << ", " << actual_delta << std::endl;
}
