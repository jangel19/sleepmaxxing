#pragma once
#include "recoveryTrend.hpp"
#include "circadianShift.hpp"

struct Combine
{
    RecoverySummary rec_sum;
    CircadianShift cir_shift;
};

Combine compute_final_results(const std::vector<DayRecord>& days);
