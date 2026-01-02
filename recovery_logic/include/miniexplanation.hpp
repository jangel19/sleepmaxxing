#pragma once
#include "recoveryTrend.hpp"
#include "circadianShift.hpp"

std::vector<std::string> generate_recommendations(const RecoverySummary& rec, const CircadianShift& circ);
