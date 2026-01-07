#include "combine.hpp"

Combine compute_final_results(const std::vector<DayRecord>& days) {
    Combine result;
    result.rec_sum = compute_recovery_trend(days);
    result.cir_shift = compute_circadian_shift(days);

    return result;
}
