#include "actual_outcome.hpp"
#include <algorithm>

double compute_actual_hrv_delta(const std::vector<DayRecord>& days, const std::string& pred_date,
    size_t h_days){

    size_t idx = days.size();
    for (size_t i = 0; i < days.size(); i++) {
        if (days[i].date == pred_date) {
            idx = i;
            break;
        }
    }

    if (idx == days.size()) return 0.0;
    if (idx + h_days >= days.size()) return 0.0;

    // collect first 3 HRV after prediction date
    std::vector<double> base;
    for (size_t i = idx; i < idx + 3 && i < days.size(); i++) {
        if (days[i].hrv.has_value())
            base.push_back(days[i].hrv.value());
    }

    // collect last 3 HRV in window
    std::vector<double> future;
    for (size_t i = idx + h_days - 3; i < idx + h_days && i < days.size(); i++) {
        if (days[i].hrv.has_value())
            future.push_back(days[i].hrv.value());
    }

    if (base.size() < 2 || future.size() < 2)
        return 0.0;

    auto mean = [](const std::vector<double>& v) {
        double s = 0.0;
        for (double x : v) s += x;
        return s / v.size();
    };

    return mean(future) - mean(base);

}
