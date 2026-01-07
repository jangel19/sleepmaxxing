#include "robustStats.hpp"
#include <algorithm>
#include <limits>
#include <cmath>

RobustStats compute_stats(const std::vector<double>& values){
    if (values.empty()) {
        return {
            std::numeric_limits<double>::quiet_NaN(),
            std::numeric_limits<double>::quiet_NaN(),
            0
        };
    }

    std::vector<double> sorted = values;
    std::sort(sorted.begin(), sorted.end());

    RobustStats stats;
    stats.count = sorted.size();

    // compute median
    size_t n = sorted.size();
    if (n % 2 == 0) stats.median = (sorted[n/2 - 1] + sorted[n/2]) / 2.0;
    else stats.median = sorted[n / 2];

    // compute MAD
    std::vector<double> abs_mad;
    abs_mad.reserve(n);
    for(double x : sorted) {
        abs_mad.push_back(std::abs(x - stats.median));
    }

    std::sort(abs_mad.begin(), abs_mad.end());
    if (n % 2 == 0) stats.mad = (abs_mad[n/2 - 1] + abs_mad[n/2]) / 2.0;
    else stats.mad = abs_mad[n / 2];

    return stats;
}
