#pragma once
#include <vector>
#include "DayRecord.hpp"

using std::vector;

double extract_label(const vector<DayRecord>& feature, const vector<DayRecord>& future);
