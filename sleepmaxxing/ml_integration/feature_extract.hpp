#pragma once
#include <DayRecord.hpp>
#include <vector>
#include <armadillo>

arma::vec extract_features(const std::vector<DayRecord> &days);
