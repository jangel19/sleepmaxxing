#pragma once
#include "robustStats.hpp"
#include "windowStats.hpp"
#include "DayRecord.hpp"

struct MetricWindowStats {
    RobustStats stats;
    size_t valid_days;
};

struct WindowComparison {
    MetricWindowStats recent;
    MetricWindowStats baseline;
};

enum class Metric { HRV, RHR, Stress, Activity, Sleep, Wake };

static std::optional<double> getMetric(const DayRecord& d, const Metric& m);
WindowComparison compute_window_stats( const std::vector<DayRecord>& days, Metric metric);
