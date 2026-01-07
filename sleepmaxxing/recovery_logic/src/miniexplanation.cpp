#include "miniexplanation.hpp"
#include <sstream>

std::vector<std::string> generate_recommendations(const RecoverySummary& rec, const CircadianShift& circ) {
    std::vector<std::string> recommendations;
    if (rec.trend == Trend::Declining) { // if rec is declnining
        recommendations.push_back(
            "recovery is trending downward. please prioritize recovery over performance for the next few days."
        );
    }

    // metric specific
    for (const auto& d: rec.drivers) {
        if (d.metric == Metric::Stress && !d.positive_for_recovery) {
            recommendations.push_back(
                "elevated stress detected. reduce the cognitive load and avoid unnecessary stressors."
            );
        }
        if (d.metric == Metric::HRV && !d.positive_for_recovery) {
            recommendations.push_back(
                "lower HRV suggests reduced training readiness. increase sleep and avoid high-intensity training."
            );
        }
    if (d.metric == Metric::RHR && !d.positive_for_recovery) {
            recommendations.push_back(
                "resting heart rate is elevated.\n"
                "focus on active recovery and light aerobic movement such as walking or light cycling."
            );
        }
    }

    if (circ.significant) { // circ ry guidance
        recommendations.push_back("circadian rythnm disruption detected."
        "Stabalize wake time within 30 minutes daily.");
    }

    if (rec.confidence < 0.4) {
        recommendations.push_back ("signal confidence is low. monitor trends over the next 5-7 days");
    }

    return recommendations;
}
