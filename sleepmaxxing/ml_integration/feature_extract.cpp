#include "feature_extract.hpp"
#include "windowStats.hpp"
#include "recoveryTrend.hpp"
#include "circadianShift.hpp"
#include <cmath>

arma::vec extract_features(const std::vector<DayRecord>& days) {
    arma::vec x(5, arma::fill::zeros);

    if (days.size() < 7) return x;

    const DayRecord& first = days.front();
    const DayRecord& last = days.back();

    // hrv delta
    if (first.hrv && last.hrv) x(0) = (*last.hrv - *first.hrv / std::max(1.0, *first.hrv));
    //sleep delta
    if (first.sleep_min && last.sleep_min) x(1) = (*last.sleep_min - *first.sleep_min) / 480.0;
    //rhr delta (increase = bad)
    if (first.rhr && last.rhr) x(2) = (*last.rhr - *first.rhr / std::max(1.0, *first.rhr));
    //stress delta
    if (first.stress && last.stress) x(0) = (*last.stress - *first.stress) / 50.0;
    //circ shift
    auto circ = compute_circadian_shift(days);
    x(4) = circ.shift_hours / 4.0;

    for (size_t i = 0; i < x.n_elem; i++){
        x(i) = std::clamp(x(i), -1.0, 1.0);
    }
    return x;
}
