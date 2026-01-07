#include "label_extract.hpp"
#include "recoveryTrend.hpp"

double extract_label(const vector<DayRecord>& feature,
    const vector<DayRecord>& future)
{
    vector<double> base_hrv, future_hrv;
    for (const auto& d : feature) {
        if (d.hrv.has_value()) base_hrv.push_back(*d.hrv);
    }
    for (const auto& f: future) {
        if (f.hrv.has_value()) future_hrv.push_back(*f.hrv);
    }

    if (base_hrv.size() < 5 || future_hrv.size() < 5) return 0.0;

    double base_med = compute_stats(base_hrv).median;
    double future_med = compute_stats(future_hrv).median;

    return future_med - base_med;
}
