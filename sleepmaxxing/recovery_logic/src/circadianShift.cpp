#include "circadianShift.hpp"
#include <cmath>

// need a min days
// basline slicing
// get valid days only
// make sure it has more than min days
constexpr size_t MIN_VALID_DAYS = 5; // make sure we have at least one work week's data (preferred)
constexpr double MINUTES_PER_DAY = 1440.0; // angle formula
constexpr double TWO_PI = 2.0 * M_PI;
constexpr double MIN_STABILITY = 0.3; // seems right as anythig under is chaotic

CircadianShift compute_circadian_shift(const std::vector<DayRecord> &days) {
    CircadianShift result{};

    const size_t N = days.size();
    // safety
    if (N == 0) {
        result.shift_hours = 0.0;
        result.significant = false;
        return result;
    }

    // sliceeeee into two windows
    size_t recent_begin = (N > 14) ? (N - 14) : 0;
    size_t recent_end = N;
    size_t baseline_begin = (recent_begin > 14) ? (recent_begin - 14) : 0;
    size_t baseline_end = recent_begin;

    size_t baseline_days = 0;

    double base_sum_x = 0.0; // directions
    double base_sum_y = 0.0;

    for (size_t i = baseline_begin; i < baseline_end; i++) {
        auto v = days[i].wake_min;
        if (v.has_value()) {
            // min from midnight
            double base_minutes = static_cast<double>(*v);
            double base_angle = base_minutes * (TWO_PI / MINUTES_PER_DAY); // clock baseline

            base_sum_x += std::cos(base_angle);
            base_sum_y += std::sin(base_angle); // where time points

            baseline_days++;
        }
    }
    // make sure we have enough days
    if (baseline_days < MIN_VALID_DAYS) {
        result.shift_hours = 0;
        result.significant = false;
        return result;
    }

    // circular mean
    double avg_base_x = base_sum_x / baseline_days;
    double avg_base_y = base_sum_y / baseline_days;

    //vector stability 0-1
    double base_r = std::sqrt(std::pow(avg_base_x, 2) + std::pow(avg_base_y, 2));

    // convert to "clocl" angle
    double mean_angle = std::atan2(avg_base_y, avg_base_x);

    if (mean_angle < 0) { // so it doesnt return negative angle
       mean_angle += TWO_PI;
    }

    double mean_minutes = mean_angle * (MINUTES_PER_DAY / TWO_PI);
    // double mean_hours = mean_minutes / 60.0; didnt need this

    size_t recent_days = 0;
    double rec_sum_x = 0.0, rec_sum_y = 0.0;

    for (size_t i = recent_begin; i < recent_end; i++) {
        auto v = days[i].wake_min;
        if (v.has_value()) {
            double rec_minutes = static_cast<double>(*v);
            double rec_angle = rec_minutes * (TWO_PI / MINUTES_PER_DAY);

            rec_sum_x += std::cos(rec_angle);
            rec_sum_y += std::sin(rec_angle);

            recent_days++;
        }
    }

    if (recent_days < MIN_VALID_DAYS) {
        result.shift_hours = 0;
        result.significant = false;
        return result;
    }

    double avg_rec_x = rec_sum_x / recent_days;
    double avg_rec_y = rec_sum_y / recent_days;

    double rec_r = std::sqrt(std::pow(avg_rec_x, 2) + std::pow(avg_rec_y, 2));

    bool baseline_stable = base_r >= MIN_STABILITY;
    bool rec_stable = rec_r >= MIN_STABILITY;

    // not enough to reliably shift
    if (!baseline_stable || !rec_stable) {
        result.shift_hours = 0.0;
        result.significant = false;
        return result;
    }

    double rec_mean_angle = std::atan2(avg_rec_y, avg_rec_x);

    if (rec_mean_angle < 0) rec_mean_angle += TWO_PI;

    double rec_mean_minutes = rec_mean_angle * (MINUTES_PER_DAY / TWO_PI);
    // double rec_mean_hours = rec_mean_minutes / 60.0; didnt need this

    // get shortest distance
    double delta_min = rec_mean_minutes - mean_minutes;

    if (delta_min > 720) {
        delta_min -= 1440;
    }
    else if (delta_min < -720) {
        delta_min += 1440;
    }

    double shift_hours = delta_min / 60;

    result.baseline = baseline_days;
    result.recent_days = recent_days;
    result.shift_hours = shift_hours;
    // the shift is significant if its longer than half an hour
    result.significant = std::abs(shift_hours) >= 0.5;

    return result;
}
