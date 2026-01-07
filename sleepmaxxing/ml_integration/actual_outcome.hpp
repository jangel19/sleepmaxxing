#pragma once
#include <vector>
#include <string>
#include "DayRecord.hpp"
#include "recoveryTrend.hpp"

double compute_actual_hrv_delta(const std::vector<DayRecord>& days, const std::string& pred_date,
    size_t h_days);
