#pragma once
#include "feature_extract.hpp"
#include "label_extract.hpp"
#include "actual_outcome.hpp"
#include "DayRecord.hpp"
#include <armadillo>

constexpr size_t WINDOW_SIZE = 7;
constexpr size_t LABEL_WINDOW = 7;

struct Sample
{
    arma::vec x;
    int y;
};

vector<Sample> build_dataset(const vector<DayRecord>& days);
