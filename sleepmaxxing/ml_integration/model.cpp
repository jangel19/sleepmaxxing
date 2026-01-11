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
    arma::mat X; arma::rowvec y;

    load_training_data(X, y);

    if (X.n_cols < MIN_SAMPLES) {
        std::cerr << "WARNING: Insufficient training samples (" << X.n_cols <<
            " < " << MIN_SAMPLES << ")\n";
        return {0.0, arma::vec(), arma::vec()};
    }

    //norm training set
    NormStats stats = compute_norm_stats(X);
    normalize(X, stats);

    //train rr
    mlpack::LinearRegression rr(X, y, RIDGE_LAM);

    //build prediction sample using most recent window size
    if (days.size() < WINDOW_SIZE) return {0.0, arma::vec(), arma::vec()};

    std::vector<DayRecord> recent(days.end() - WINDOW_SIZE, days.end());
    arma::vec latest_x = extract_features(recent);
    if (!latest_x.is_finite()) return {0.0, arma::vec(), arma::vec()};

    normalize_vector(latest_x, stats);

    //predict
    arma::mat point(latest_x.n_elem, 1);
    point.col(0) = latest_x;

    arma::rowvec pred_row;
    rr.Predict(point, pred_row);

    double predicted_delta =pred_row(0);
    if (!std::isfinite(predicted_delta)) predicted_delta = 0.0;

    predicted_delta = std::clamp(predicted_delta, -10.0, 10.0);

    // keep only weights
    arma::vec params = rr.Parameters();
    arma::vec weights;
    if (params.n_elem == latest_x.n_elem + 1) weights = params.subvec(1, params.n_elem - 1);
    else weights = params; // fallback
    return {predicted_delta, weights, latest_x};
}
