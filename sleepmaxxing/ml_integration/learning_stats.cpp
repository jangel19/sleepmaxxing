#include "learning_stats.hpp"
#include "load_errors.hpp"
#include <numeric>
#include <sstream>
#include <fstream>

std::string model_learning_sum() {
    auto errors = load_pred_errors();

    if (errors.size() < 3) return "Model learning status: \n not enough historical data\n";

    // adds everything
    double early_avg = std::accumulate(errors.begin(), errors.begin() + errors.size() / 2, 0.0) /
        (errors.size() / 2);

    double recent_avg = std::accumulate(errors.begin() + errors.size() / 2, errors.end(), 0.0) /
        (errors.size() - errors.size() /2);

    std::ostringstream out;

    out << "Model learning status:\n";
    out << "- Historical samples: " << errors.size() << std::endl;
    out << "- Mean prediction error (7d HRV): " << early_avg << "->" << recent_avg << std::endl;

    return out.str();
}

double directional_accu() {
    std::ifstream file("ml_logger/errors.csv");
    std::string line;
    int correct = 0, total =0;

    if (!file.is_open()) return 0.0;

    std::getline(file, line);

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string date, pred, actual, err;

        std::getline(ss, date, ',');
        std::getline(ss, pred, ',');
        std::getline(ss, actual, ',');
        std::getline(ss, err, ',');
        try {
            double p = std::stod(pred);
            double a = std::stod(actual);
            if ((p >= 0 && a >= 0) || (p < 0 && a < 0)) correct++;
            total++;
        } catch (...) { continue; }
    }
    
    return total ? (100.0 * correct / total) : 0.0;

}
