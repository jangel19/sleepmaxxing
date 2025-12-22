#pragma once
#include <string>
#include <optional>

struct DayRecord {
    std::string date;
    std::optional<int> sleep_min; // sleep duration
    std::optional<int> wake_min; // min awake since midnight
    std::optional<double> activity; // user activity
    std::optional<double> hrv; // user heart rate variability
    std::optional<double> stress; // user's stress
    std::optional<double> rhr; // users resting heart rate
};
