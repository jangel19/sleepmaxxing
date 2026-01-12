#include "learning_stats.hpp"
#include "load_errors.hpp"
#include <numeric>
#include <sstream>
#include <fstream>

std::string model_learning_sum() {
    auto errors = load_pred_errors();

    if (errors.empty()) {
        return
            "Model learning status:\n"
            "- Baseline collected\n"
            "- Awaiting first evaluation window\n";
    }

    std::ostringstream out;
    out << "MODEL RELIABILITY:\n";
    out << "- Directional Accuracy: Improving (" << errors.size() << " evaluations)\n";

    if (errors.size() == 1) {
        out << "- Mean prediction error (7d HRV): " << errors[0] << "\n";
        return out.str();
    }

    size_t mid = errors.size() / 2;

    double early_avg =
        std::accumulate(errors.begin(), errors.begin() + mid, 0.0) / mid;

    double recent_avg =
        std::accumulate(errors.begin() + mid, errors.end(), 0.0) / (errors.size() - mid);

    out << "- Mean prediction error (7d HRV): "
        << early_avg << " -> " << recent_avg << "\n";

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
