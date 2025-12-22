#pragma once
#include "DayRecord.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

std::vector<DayRecord> load_csv(const std::string& path);
