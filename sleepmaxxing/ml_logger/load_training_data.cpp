#include "load_training_data.hpp"

void load_training_data(arma::mat& X, arma::rowvec& y) {
    std::ifstream file("ml_logger/training_data.csv");

    if (!file.is_open()) {
        std::cerr << "ERROR: cannot open training_dats.csv" << std::endl;
        X.set_size(5,0);
        y.set_size(0);
        return;
    }

    std::string line;
    std::vector<arma::vec> rows;
    std::vector<double> labels;

    if (!std::getline(file, line)) {
        std::cerr << "ERROR: training_data.csv is empty" << std::endl;
        X.set_size(5,0);
        y.set_size(0);
        return;
    }

    size_t line_num = 1;
    while (std::getline(file, line)) {
        line_num++;

        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string cell;

        //skip date col
        if (!std::getline(ss, cell, ',')) {
            std::cerr << "WARNING: Skipping malformed line " << line_num << "\n";
            continue;
        }

        arma::vec x(5);
        bool valid = true;

        // Read 5 features
        for (size_t i = 0; i < 5; i++) {
            if (!std::getline(ss, cell, ',')) {
                std::cerr << "WARNING: Incomplete feature at line " << line_num << "\n";
                valid = false;
                break;
            }

            try {
                x(i) = std::stod(cell);
                if (!std::isfinite(x(i))) {
                    std::cerr << "WARNING: Non-finite feature at line " << line_num << "\n";
                    valid = false;
                    break;
                }
            } catch (const std::exception& e) {
                std::cerr << "WARNING: Cannot parse feature at line " << line_num
                          << " (" << e.what() << ")\n";
                valid = false;
                break;
            }
        }

        if (!valid) continue;

        // Read label
        if (!std::getline(ss, cell, ',')) {
            std::cerr << "WARNING: Missing label at line " << line_num << "\n";
            continue;
        }

        try {
            double label = std::stod(cell);
            if (!std::isfinite(label)) {
                std::cerr << "WARNING: Non-finite label at line " << line_num << "\n";
                continue;
            }

            labels.push_back(label);
            rows.push_back(x);

        } catch (const std::exception& e) {
            std::cerr << "WARNING: Cannot parse label at line " << line_num
                      << " (" << e.what() << ")\n";
            continue;
        }
    }

    file.close();

    // Build matrices
    if (rows.empty()) {
        std::cerr << "WARNING: No valid training samples found in training_data.csv\n";
        X.set_size(5, 0);
        y.set_size(0);
        return;
    }

    X.set_size(5, rows.size());
    y.set_size(rows.size());

    for (size_t i = 0; i < rows.size(); i++) {
        X.col(i) = rows[i];
        y(i) = labels[i];
    }

    std::cout << "Loaded " << rows.size() << " training samples from disk\n";
}
