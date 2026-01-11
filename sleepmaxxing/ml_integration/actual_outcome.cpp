#include "actual_outcome.hpp"
#include <algorithm>

double compute_actual_hrv_delta(const std::vector<DayRecord>& days, const std::string& pred_date, size_t h_days)
{
    size_t index_ = days.size();
    for (size_t i = 0; i < days.size(); i++) {
        if (days[i].date == pred_date) {
            index_ = i;
            break;
        }
    }

    // if the date is not found
    if (index_ == days.size()) return 0.0;

    // if there isnt enough data
    if (index_ + h_days >= days.size()) return 0.0;

    //base hrv
    std::vector<double> base;
    for (size_t i = (index_ >= 3 ? index_ - 3 : 0); i < index_; i++) {
        if (days[i].hrv.has_value()) base.push_back(days[i].hrv.value());
    }

    // fut hrv
    std::vector<double>future;
    size_t start = index_ + h_days - 3;
    for (size_t i = start; i < index_ + h_days && i < days.size(); i++) {
        if (days[i].hrv.has_value()) future.push_back(days[i].hrv.value());
    }

    if (base.empty() || future.empty()) return 0.0; //not enough data

    auto mean = [](const std::vector<double>& v) {
        double s = 0.0;
        for (double x: v) s+=x;
        return s/ v.size();
    };

    return mean(future) - mean(base);
}
