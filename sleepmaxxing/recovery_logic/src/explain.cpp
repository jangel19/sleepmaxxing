#include "explain.hpp"
#include <sstream>
#include <cmath>

static bool has_driver(Metric m, const RecoverySummary& s) {
    for (const auto& d: s.drivers) {
        if (d.metric == m) return true;
    }
    return false;
}

std::string explain_future(const RecoverySummary& rec_sum, const CircadianShift& cs,
    PredictionResult& ml)
{
    std::ostringstream out;

    out << "---------------------WHAT THIS MEANS FOR YOU (ML INSIGHT)-------------------------\n\n";

    if (ml.weights.n_elem > 0) {
        out << "The model has learned preliminary personal recovery drivers "
            "based on your last month of data.\n"
            "Prediction accuracy will improve as future outcomes become available.\n\n";
    } else {
        out << "Machine learning insights are limited due to insufficient historical data.\n"
            "As more days are logged, this system will begin identifying which physiological\n"
            "signals most strongly influence your recovery.\n\n";
    }

    if (ml.prediction < -0.5) {
        out << "Your current habits are likely suppressing recovery. \n";
    } else if (ml.prediction < 0.5) {
        out << "Your recovery appears stable if current habits continue.\n";
    } else {
        out << "Your current habits are supporting improving recovery.\n";
    }

    if (ml.weights.n_elem > 1 && ml.features.n_elem > 0) {
        arma::vec contributions = ml.weights.tail(ml.features.n_elem) % ml.features;

        arma::uword dominant_index = contributions.index_max();

        out << "Primary physiological driver detected: \n";

        switch (dominant_index)
        {
        case 0:
            out << "- HRV deviation is the strongest factor affecting recovery.\n";
            break;
        case 1:
            out << "- Sleep consistency is currently the dominant recovery driver.\n";
            break;
        case 2:
            out << "- Elevated resting heart rate is limiting recovery.\n";
            break;
        case 3:
            out << "- Stress load is significantly impacting recovery capacity.\n";
            break;
        case 4:
            out << "- Circadian misalignment is suppressing recovery signals.\n";
            break;
        default:
            break;
        }

        out << "\n";
    }

    if (rec_sum.trend == Trend::Stable) {
    out <<
        "Your recovery metrics are currently stable.\n"
        "This suggests your current balance of sleep, stress, and activity\n"
        "is sufficient to maintain baseline recovery.\n"
        "Focus on consistency rather than pushing volume or intensity.\n\n";
    }

    // sleep driver
    if (rec_sum.trend == Trend::Declining && has_driver(Metric::Sleep, rec_sum)) {
        out <<
            "Sleep loss appears before the drop in HRV, indicating reduced recovery capacity\n"
            "rather than excessive training load.\n"
            "If this remains unchanged:\n"
            "- In 7 days: HRV may continue to decline\n"
            "- In 2–4 weeks: recovery baseline may reset lower\n"
            "HOW TO PREVENT:\n"
            "Restore sleep duration and consistency before increasing training intensity.\n\n";
    }

    // stress driver
    if (rec_sum.trend == Trend::Declining && has_driver(Metric::Stress, rec_sum)) {
        out <<
            "Elevated stress has contributed to a physiological recovery decline.\n"
            "This suggests cognitive or lifestyle strain is impacting recovery.\n"
            "If unchanged:\n"
            "- Recovery may remain suppressed even with reduced training\n"
            "HOW TO PREVENT:\n"
            "Reduce non-training stressors and cognitive load.\n\n";
    }

    // rhr driver
    if (rec_sum.trend == Trend::Declining && has_driver(Metric::RHR, rec_sum)) {
        out <<
            "Resting heart rate increased alongside declining HRV.\n"
            "This pattern often reflects accumulated fatigue or incomplete recovery.\n"
            "HOW TO PREVENT:\n"
            "Prioritize low-intensity activity and active recovery for 2–3 days.\n\n";
    }

    // circ rhythmn explanation
    if (cs.significant) {
        out <<
            "Your circadian rhythm has shifted significantly.\n"
            "Even with adequate sleep duration, circadian misalignment can blunt recovery.\n"
            "If unchanged:\n"
            "- Hormonal recovery signals may remain suppressed\n"
            "HOW TO PREVENT:\n"
            "Maintain a consistent wake time within ±30 minutes for the next week.\n\n";
    } else {
        out <<
        "No significant circadian shift detected.\n"
        "Your wake timing appears stable relative to recent history.\n\n";
    }

    // trend going up
    if (rec_sum.trend == Trend::Improving) {
        out <<
            "Your current routine supports recovery improvements.\n"
            "Avoid unnecessary changes and allow adaptations to consolidate.\n\n";
    }
    if (rec_sum.confidence < 0.4) {
    out <<
        "Confidence is currently low due to limited historical data.\n"
        "Expect insight quality to improve after 2–3 additional weeks of logging.\n\n";
    }

    return out.str();

}
