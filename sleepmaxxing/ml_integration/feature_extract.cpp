#include "feature_extract.hpp"
#include "windowStats.hpp"
#include "recoveryTrend.hpp"
#include "circadianShift.hpp"

using std::vector;

arma::vec extract_features(const vector<DayRecord>& days)
{
    arma::vec x (5, arma::fill::zeros);

    auto hrv = compute_window_stats(days, Metric::HRV);
    auto sleep = compute_window_stats(days, Metric::Sleep);
    auto rhr = compute_window_stats(days, Metric::RHR);
    auto stress = compute_window_stats(days, Metric::Stress);
    auto circ = compute_circadian_shift(days);

    if (hrv.recent.valid_days >= 5 && hrv.baseline.valid_days >= 10) {
        x(0) = (hrv.recent.stats.median - hrv.baseline.stats.median)
            / hrv.baseline.stats.median;
    }
    if (sleep.recent.valid_days >= 5 && sleep.baseline.valid_days >= 10) {
        x(1) = (sleep.recent.stats.median - sleep.baseline.stats.median)
            / sleep.baseline.stats.median;
    }
    if (rhr.recent.valid_days >= 5 && rhr.baseline.valid_days >= 10) {
        x(2) = (rhr.recent.stats.median - rhr.baseline.stats.median)
            / rhr.baseline.stats.median;
    }
    if (stress.recent.valid_days >= 5 && stress.baseline.valid_days >= 10) {
        x(3) = (stress.recent.stats.median - stress.baseline.stats.median)
            / stress.baseline.stats.median;
    }

    x(4) = std::clamp(circ.shift_hours, -4.0, 4.0);

    return x;
}
