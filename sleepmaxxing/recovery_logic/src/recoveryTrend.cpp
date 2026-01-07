#include "recoveryTrend.hpp"
#include <cmath>
#include <limits>
#include <algorithm>

constexpr size_t MIN_VALID_DAYS = 3; // updated to three for better tolerance
constexpr double EFFECT_THRESHOLD = 0.4; // updated it for same reason
constexpr double EPS = 1e-6;

//make sure we have valid data
static bool qualityControl(const WindowComparison& wc) {
    return wc.baseline.valid_days >= MIN_VALID_DAYS &&
           wc.recent.valid_days   >= MIN_VALID_DAYS;
}

RecoverySummary compute_recovery_trend(const std::vector<DayRecord>& days) {
    RecoverySummary result;
    result.trend = Trend::Stable;
    result.confidence = 0.0;
    result.drivers.clear();

    double total_effect = 0.0;
    size_t num_votes = 0;

    {
        WindowComparison hrv = compute_window_stats(days, Metric::HRV);
        if (qualityControl(hrv)) {
            double baseline = hrv.baseline.stats.median;
            double recent   = hrv.recent.stats.median;

            double scale = std::max(std::max(hrv.baseline.stats.mad, hrv.recent.stats.mad),EPS);

            double delta  = recent - baseline;
            double effect = delta / scale; // higher the better
            double vote   = effect;

            if (std::abs(vote) >= EFFECT_THRESHOLD) {
                total_effect += vote;
                num_votes++;

                result.drivers.push_back({ Metric::HRV, baseline, recent, delta, effect, vote > 0 });
            }
        }
    }

    {
        WindowComparison rhr = compute_window_stats(days, Metric::RHR);
        if (qualityControl(rhr)) {
            double baseline = rhr.baseline.stats.median;
            double recent   = rhr.recent.stats.median;

            double scale = std::max(std::max(rhr.baseline.stats.mad, rhr.recent.stats.mad),EPS);

            double delta  = recent - baseline;
            double effect = delta / scale;
            double vote   = -effect; //lower is better

            if (std::abs(vote) >= EFFECT_THRESHOLD) {
                total_effect += vote;
                num_votes++;

                result.drivers.push_back({ Metric::RHR, baseline, recent, delta, effect, vote > 0 });
            }
        }
    }

    {
        WindowComparison stress = compute_window_stats(days, Metric::Stress);
        if (qualityControl(stress)) {
            double baseline = stress.baseline.stats.median;
            double recent   = stress.recent.stats.median;

            double scale = std::max(std::max(stress.baseline.stats.mad, stress.recent.stats.mad), EPS);

            double delta  = recent - baseline;
            double effect = delta / scale;
            double vote   = -effect; //lower is better

            if (std::abs(vote) >= EFFECT_THRESHOLD) {
                total_effect += vote;
                num_votes++;

                result.drivers.push_back({ Metric::Stress, baseline, recent, delta, effect, vote > 0 });
            }
        }
    }

    if (num_votes == 0) {
        result.trend = Trend::Stable;
        result.confidence = 0.0;
        return result;
    }

    double avg_effect = total_effect / static_cast<double>(num_votes);

    if (avg_effect > EFFECT_THRESHOLD) {
        result.trend = Trend::Improving;
    } else if (avg_effect < -EFFECT_THRESHOLD) {
        result.trend = Trend::Declining;
    } else {
        result.trend = Trend::Stable;
    }

    result.confidence = static_cast<double>(num_votes) / 3.0;

    return result;
}
