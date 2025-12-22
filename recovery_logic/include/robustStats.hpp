#pragma once
#include <cstdlib>
#include <vector>


struct RobustStats {
    double median;
    double mad;
    size_t count;
};

RobustStats compute_stats(const std::vector<double>& values);
