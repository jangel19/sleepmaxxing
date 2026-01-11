#include "miniexplanation.hpp"

std::vector<std::string> generate_recommendations(
    const RecoverySummary& rec,
    const CircadianShift& circ
) {
    std::vector<std::string> recommendations;

    //overal recovery trend
    if (rec.trend == Trend::Declining) {
        recommendations.push_back(
            "Recovery is trending downward. Prioritize recovery over performance for the next few days."
        );
    }

    // specifivc drivers
    for (const auto& d : rec.drivers) {
        if (d.metric == Metric::Stress && !d.positive_for_recovery) {
            recommendations.push_back(
                "Elevated stress detected. Reduce cognitive load and avoid unnecessary stressors where possible."
            );
        }

        if (d.metric == Metric::HRV && !d.positive_for_recovery) {
            recommendations.push_back(
                "Lower HRV suggests reduced training readiness. Increase sleep duration and avoid high-intensity training."
            );
        }

        if (d.metric == Metric::RHR && !d.positive_for_recovery) {
            recommendations.push_back(
                "Resting heart rate is elevated. Focus on active recovery and light aerobic movement such as walking or easy cycling."
            );
        }
    }

    // good reinforcement when recovery is improving
    if (rec.trend == Trend::Improving && recommendations.size() <= 1) {
        recommendations.push_back(
            "Recovery is trending positively. Maintain current routines and avoid making major changes this week."
        );
    }

    // circ rhythm guidance
    if (circ.significant) {
        recommendations.push_back(
            "Circadian rhythm disruption detected. Stabilize wake time within 30 minutes each day."
        );
    }

    // confidence
    if (rec.confidence < 0.4) {
        recommendations.push_back(
            "Model confidence is still building. Monitor trends over the next 5–7 days for clearer insight."
        );
    }

    return recommendations;
}
