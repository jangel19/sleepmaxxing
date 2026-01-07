#include "windowStats.hpp"
#include <limits>
#include <optional>

static std::optional<double> getMetric(const DayRecord& d, const Metric& m) {
    switch (m) {
    case Metric::Sleep:
        return d.sleep_min.has_value() ? std::optional<double>(d.sleep_min.value()) : std::nullopt;

    case Metric::Wake:
        return d.wake_min.has_value() ? std::optional<double>(d.wake_min.value()) : std::nullopt;

    case Metric::Activity:
        return d.activity.has_value() ? std::optional<double>(d.activity.value()) : std::nullopt;

    case Metric::HRV:
        return d.hrv.has_value() ? std::optional<double>(d.hrv.value()) : std::nullopt;

    case Metric::Stress:
        return d.stress.has_value() ? std::optional<double>(d.stress.value()) : std::nullopt;

    case Metric::RHR:
        return d.rhr.has_value() ? std::optional<double>(d.rhr.value()) : std::nullopt;
    }

    return std::nullopt;
}

WindowComparison compute_window_stats( const std::vector<DayRecord>& days, Metric metric) {
    WindowComparison result;

    if (days.empty()) {
        result.baseline.stats.median = std::numeric_limits<double>::quiet_NaN();
        result.baseline.stats.mad = std::numeric_limits<double>::quiet_NaN();
        result.baseline.stats.count = 0;
        result.baseline.valid_days = 0;

        result.recent.stats.median = std::numeric_limits<double>::quiet_NaN();
        result.recent.stats.mad = std::numeric_limits<double>::quiet_NaN();
        result.recent.stats.count = 0;
        result.recent.valid_days = 0;

        return result;
    }

    const size_t N = days.size();

    // slice
    size_t recent_begin = (N > 14) ? (N - 14) : 0;
    size_t recent_end   = N;

    size_t baseline_end   = recent_begin;
    size_t baseline_begin = (baseline_end > 14) ? (baseline_end - 14) : 0;

    std::vector<double> baseline_values;
    size_t baseline_days = 0;

    for (size_t i = baseline_begin; i < baseline_end; i++) {
        auto v = getMetric(days[i], metric);
        if (v.has_value()) {
            baseline_values.push_back(*v);
            baseline_days++;
        }
    }

    std::vector<double> recent_values;
    size_t recent_days = 0;

    for (size_t i = recent_begin; i < recent_end; i++) {
        auto v = getMetric(days[i], metric);
        if (v.has_value()) {
            recent_values.push_back(*v);
            recent_days++;
        }
    }
    result.baseline.stats = compute_stats(baseline_values);
    result.baseline.valid_days = baseline_days;

    result.recent.stats = compute_stats(recent_values);
    result.recent.valid_days = recent_days;

    return result;
}
