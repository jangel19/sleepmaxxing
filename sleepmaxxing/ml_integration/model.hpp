#pragma once
#include "dataset_prepper.hpp"
#include "has_seen.hpp"
#include "load_training_data.hpp"
#include "error_stats.hpp"
#include <mlpack/core.hpp>
#include <mlpack/methods/linear_regression/linear_regression.hpp>
struct PredictionResult {
    double prediction;
    arma::vec weights;
    arma::vec features;
};

struct NormStats {
    arma::vec mean;
    arma::vec std;
};

void run_pipeline(std::vector<DayRecord> &days);

PredictionResult train_and_predict(const std::vector<DayRecord>& days);
