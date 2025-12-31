#pragma once
#include "windowStats.hpp"
#include "DayRecord.hpp"
#include <vector>

enum class Trend {
    Improving, Stable, Declining
};

struct MetricDriver {
    Metric metric;
    double baseline_median, recent_median, delta, effect;
    bool positive_for_recovery;
};

struct RecoverySummary {
    Trend trend;
    double confidence;
    std::vector<MetricDriver> drivers;
};


RecoverySummary compute_recovery_trend(const std::vector<DayRecord>& days);
