#pragma once
#include <string>
#include <armadillo>
#include "recoveryTrend.hpp"
#include "circadianShift.hpp"
#include "model.hpp"

std::string explain_future(const RecoverySummary& rec_sum, const CircadianShift& cs,
    PredictionResult& ml);
