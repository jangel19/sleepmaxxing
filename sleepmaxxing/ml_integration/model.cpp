#include "model.hpp"
#include <iostream>

using namespace mlpack;

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

    if (dataset.size() < 6) return {0.0, arma::vec(), arma::vec()};

    load_training_data(X, y);
    if (X.n_cols < 5) return {0.0, arma::vec(), arma::vec()};

    LinearRegression lr;
    lr.Train(X, y, 0.1); // since health data is noisy
    arma::vec weights = lr.Parameters();

    arma::vec latest_x = extract_features(
        std::vector<DayRecord>(days.end() - WINDOW_SIZE, days.end())
    );
    arma::mat latest_X = latest_x;
    latest_X.reshape(latest_x.n_elem, 1);

    arma::rowvec prediction;
    lr.Predict(latest_X, prediction);

    double predicted_delta = prediction(0);

    double mean_error = load_mean_error();
    predicted_delta -= 0.5 * mean_error;
    predicted_delta = std::clamp(predicted_delta, -10.0, 10.0);

    arma::vec features = latest_X;

    return {predicted_delta, weights, features};
}
