#pragma once
#include <vector>
#include <string>
#include "DayRecord.hpp"
#include <optional>

// load csv into the list
std::vector<DayRecord> load_csv(const std::string& path);
