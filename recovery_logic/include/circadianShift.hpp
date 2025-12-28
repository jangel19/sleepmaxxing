#pragma once
#include "DayRecord.hpp"
#include <vector>
#include <cstdlib>

struct CircadianShift
{
    double shift_hours;
    bool significant;
    size_t baseline;
    size_t recent_days;
};

CircadianShift compute_circadian_shift(const std::vector<DayRecord>& days);
