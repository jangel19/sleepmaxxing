#include "load_training_data.hpp"

void load_training_data(arma::mat& X, arma::rowvec& y) {
    std::ifstream file("ml_logger/training_data.csv");
    std::string line;

    std::vector<arma::vec> rows;
    std::vector<double> labels;

    std::getline(file, line);

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string cell;

        std::getline(ss, cell, ',');

        arma::vec x(5);
        for (size_t i = 0; i < 5; i++) {
            std::getline(ss, cell, ',');
            x(i) = std::stod(cell);
        }

        std::getline(ss, cell, ',');
        labels.push_back(std::stod(cell));

        rows.push_back(x);
    }

    X.set_size(5, rows.size());
    y.set_size(rows.size());

    for (size_t i = 0; i < rows.size(); i++) {
        X.col(i) = rows[i];
        y(i) = labels[i];
    }
}
