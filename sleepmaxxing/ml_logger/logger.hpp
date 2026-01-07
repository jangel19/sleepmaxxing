#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <armadillo>

void log_predictions(const std::string &date, double predicted_delta, int predicted_class);

void log_error(const std::string& date, double predicted, double actual, double error);

void log_training_data(const std::string& date, const arma::vec& x, double actual_delta);
