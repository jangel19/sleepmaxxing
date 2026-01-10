#include "model.hpp"
#include <iostream>

using namespace mlpack;

constexpr double RIDGE_LAM = 1.0;
constexpr size_t MIN_SAMPLES = 5;

static NormStats compute_norm_stats(const arma::mat &X)
{
    NormStats stats;
    stats.mean = arma::mean(X, 1);
    stats.std = arma::stddev(X, 0, 1);

    // prevent zero div
    for (size_t i = 0; i < stats.std.n_elem; i++)
    {
        if (stats.std(i) == 0.0) stats.std(i) = 1.0;
    }
    return stats;
}

static void normalize(arma::mat &X, const NormStats &stats)
{
    X.each_col() -= stats.mean;
    X.each_col() /= stats.std;
}

static void normalize_vector(arma::vec &x, const NormStats &stats)
{
    x -= stats.mean;
    x /= stats.std;
}

void run_pipeline(std::vector<DayRecord> &days)
{
    auto dataset = build_dataset(days);
    std::cout << "generated " << days.size() << " training samples" << std::endl;
}

PredictionResult train_and_predict(const std::vector<DayRecord> &days)
{
    arma::mat X;
    arma::rowvec y;

    std::string latest_date = days.back().date;
    auto dataset = build_dataset(days);

    if (dataset.size() < MIN_SAMPLES)
        return {0.0, arma::vec(), arma::vec()};

    load_training_data(X, y);
    if (X.n_cols < MIN_SAMPLES)
        return {0.0, arma::vec(), arma::vec()};

    // norm
    NormStats stats = compute_norm_stats(X);
    normalize(X, stats);

    // ridge
    LinearRegression rr(X, y, RIDGE_LAM);

    //norm sample
    arma::vec latest_x = extract_features(days);
    normalize_vector(latest_x, stats);

    arma::rowvec prediction;
    rr.Predict(latest_x, prediction);

    double predicted_delta = std::clamp(predicted_delta, -10.0, 10.0);
    //learned weight
    arma::vec weights = rr.Parameters().subvec(0, latest_x.n_elem -1);

    return {predicted_delta, weights, latest_x};
}
