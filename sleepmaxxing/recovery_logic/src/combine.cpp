#include "combine.hpp"

Combine compute_final_results(const std::vector<DayRecord>& days) {
    Combine result;
    result.rec_sum = compute_recovery_trend(days);
    result.cir_shift = compute_circadian_shift(days);

    // recovery score
    int score = 70; // baseline score

    if (result.rec_sum.trend == Trend::Improving) score += 10;
    if (result.rec_sum.trend == Trend::Declining) score -= 10;

    for (const auto& s : result.rec_sum.drivers) {
        if (!s.positive_for_recovery) score -= 5;
    }

    if (result.rec_sum.confidence < 0.37) score -= 5;

    score = std::max(0, std::min(100, score));

    result.rec_sum.rec_score = score;
    
    return result;
}
