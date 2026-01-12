#include "CSVreader.hpp"
#include "combine.hpp"
#include "feature_extract.hpp"
#include "model.hpp"
#include "explain.hpp"
#include "logger.hpp"
#include "actual_outcome.hpp"
#include "learning_stats.hpp"
#include "has_seen.hpp"
#include "miniexplanation.hpp"

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

using std::cout;
using std::endl;
using std::string;

constexpr size_t BASELINE_DAYS = 7;
constexpr size_t MIN_DAYS_FOR_PRED = 21;

static std::ostream& double_endl(std::ostream& os) {
    return os << '\n' << '\n';
}

static bool evaluate_last_prediction_if_ready(
    const std::vector<DayRecord>& days,
    size_t run_idx
) {
    std::ifstream file("ml_logger/predictions.csv");
    if (!file.is_open()) return false;

    std::string line, last_line;
    std::getline(file, line);

    while (std::getline(file, line)) {
        if (!line.empty()) last_line = line;
    }

    if (last_line.empty()) return false;

    std::stringstream ss(last_line);
    std::string pred_date, pred_val;

    std::getline(ss, pred_date, ',');
    std::getline(ss, pred_val, ',');

    while (!pred_date.empty() && pred_date[0] == ' ') pred_date.erase(0,1);
    while (!pred_val.empty() && pred_val[0] == ' ') pred_val.erase(0,1);

    size_t pred_idx = days.size();
    for (size_t i = 0; i < days.size(); i++) {
        if (days[i].date == pred_date) {
            pred_idx = i;
            break;
        }
    }

    if (pred_idx == days.size()) return false;
    if (pred_idx + LABEL_WINDOW > run_idx) return false;

    double predicted = std::stod(pred_val);
    double actual = compute_actual_hrv_delta(days, pred_date, LABEL_WINDOW);
    if (actual == 0.0) return false;

    double err = predicted - actual;
    log_error(pred_date, predicted, actual, err);

    cout << "\n------------PREVIOUS PREDICTION EVALUATION------------\n\n";
    cout << "Prediction date: " << pred_date << endl;
    cout << "Predicted HRV Δ: " << predicted << endl;
    cout << "Actual HRV Δ:    " << actual << endl;
    cout << "Prediction error: " << err << double_endl;

    return true;
}

static void ensure_log_headers() {
    {
        std::ifstream in("ml_logger/training_data.csv");
        if (!in.good() || in.peek() == std::ifstream::traits_type::eof()) {
            std::ofstream out("ml_logger/training_data.csv", std::ios::app);
            out << "date,hrv,sleep,rhr,stress,circ_shift,actual_hrv_delta\n";
        }
    }
    {
        std::ifstream in("ml_logger/errors.csv");
        if (!in.good() || in.peek() == std::ifstream::traits_type::eof()) {
            std::ofstream out("ml_logger/errors.csv", std::ios::app);
            out << "date,predicted_delta,actual_delta,error\n";
        }
    }
    {
        std::ifstream in("ml_logger/predictions.csv");
        if (!in.good() || in.peek() == std::ifstream::traits_type::eof()) {
            std::ofstream out("ml_logger/predictions.csv", std::ios::app);
            out << "date,predicted_delta,predicted_class\n";
        }
    }
}

static void print_base_message(size_t days_seen) {
    cout << "ML Status: Collecting baseline data\n";
    cout << "Days observed: " << days_seen << endl;
    cout << "Predictions activate after "
         << MIN_DAYS_FOR_PRED << " days of logging"
         << double_endl;
}


int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "usage: " << argv[0]
                  << " <path to csv> <run date (yyyy-mm-dd)>" << endl;
        return 1;
    }

    const string csv_path = argv[1];
    const string run_date = argv[2];

    ensure_log_headers();

    std::vector<DayRecord> days = load_csv(csv_path);
    if (days.empty()) {
        std::cerr << "error: no valid data loaded\n";
        return 1;
    }

    cout << "LOADED " << days.size() << " days of data" << endl;

    Combine result = compute_final_results(days);

    cout << "\n--------------RECOVERY SUMMARY-----------------" << double_endl;

    switch (result.rec_sum.trend) {
        case Trend::Improving: cout << "Recovery is Improving\n\n"; break;
        case Trend::Stable:    cout << "Recovery is Stable\n\n"; break;
        case Trend::Declining: cout << "Recovery is Declining. ATTENTION IS NEEDED" << double_endl; break;
    }

    // new added rec score section
    cout << "RECOVERY SCORE FOR THIS WEEK: " << result.rec_sum.rec_score << endl;
    if (result.rec_sum.rec_score >= 75) cout << "Strong Recovery" << double_endl;
    else if (result.rec_sum.rec_score >= 50) cout << "Moderate Recovery" << double_endl;
    else cout << "RECOVERY NEEDS ATTENTION" << double_endl;

    if (result.rec_sum.confidence < 0.34) cout << "CONFIDENCE: Low\n\n";
    else if (result.rec_sum.confidence < 0.67) cout << "CONFIDENCE: Moderate\n\n";
    else cout << "CONFIDENCE: High\n\n";

    size_t run_idx = days.size();
    for (size_t i = 0; i < days.size(); i++) {
        if (days[i].date == run_date) {
            run_idx = i;
            break;
        }
    }

    if (run_idx == days.size()) {
        std::cerr << "ERROR: run date NOT FOUND in csv\n";
        return 1;
    }

    std::vector<DayRecord> history(days.begin(), days.begin() + run_idx + 1);

    cout << "Run Date: " << run_date << endl;
    cout << "Predicting recovery over next "
         << LABEL_WINDOW << " days" << double_endl;

    // log supervised training samples
    for (size_t i = 0; i + WINDOW_SIZE + LABEL_WINDOW <= days.size(); i++) {
        const size_t anchor = i + WINDOW_SIZE - 1;
        const string& anchor_date = days[anchor].date;

        if (has_seen_date(anchor_date)) continue;

        double y = compute_actual_hrv_delta(days, anchor_date, LABEL_WINDOW);
        if (std::abs(y) < 0.25) continue;

        std::vector<DayRecord> window(days.begin() + i,
                                      days.begin() + i + WINDOW_SIZE);

        arma::vec x = extract_features(window);
        if (!x.is_finite()) continue;

        log_training_data(anchor_date, x, y);
    }

    if (history.size() < MIN_DAYS_FOR_PRED) {
        print_base_message(history.size());
        cout << "ANALYSIS HAS FINISHED\n";
        return 0;
    }

    // eval last weeks prediction
    evaluate_last_prediction_if_ready(days, run_idx);

    // pred next week
    PredictionResult pred = train_and_predict(history);
    if (!std::isfinite(pred.prediction)) pred.prediction = 0.0;

    int pred_class = (pred.prediction >= 0.0) ? 1 : -1;

    cout << "\n---------------------ML PREDICTION-------------------------\n\n";
    cout << "PREDICTED HRV DELTA (next "
         << LABEL_WINDOW << " days): " << pred.prediction << double_endl;

    cout << "INTERPRETATION:\n";
    if (pred.prediction < 0) {
        cout << "- The model expects your recovery to DECLINE over the next week.\n";
        cout << "- An estimated drop of " << std::abs(pred.prediction)
            << " ms in HRV suggests increased fatigue or insufficient recovery.\n";
    } else {
        cout << "- The model expects your recovery to IMPROVE over the next week.\n";
        cout << "- An estimated increase of " << pred.prediction
            << " ms in HRV suggests improving physiological resilience.\n";
    }

    if (pred.prediction >= 5.0)
        cout <<"- Strong recovery improvement expected. Your body is adapting well to current habits!\n";
    else if (pred.prediction >= 2.0)
        cout << "- Recovery is trending upward. Small but meaningful improvements are expected.\n";
    else if (pred.prediction > -2.0)
        cout << "- Recovery is expected to remain stable. Day-to-day variation is normal\n";
    else if (pred.prediction > -5.0)
        cout << "- Recovery may decline slightly. Early signs of fatigue are present.\n";
    else
        cout << "- Significan recovery decline expected. "
            << "This reflects accumulated stress or insufficient recovery.\n";


    cout << "\n";

    log_predictions(run_date, pred.prediction, pred_class);

    cout << explain_future(result.rec_sum, result.cir_shift, pred) << double_endl;

    auto rec = generate_recommendations(result.rec_sum, result.cir_shift);

    if (!rec.empty()) {
        cout << "\n---------------------ACTIONABLE GUIDANCE---------------------" << double_endl;
        cout << "THIS WEEKS FOCUS: " << endl;
        for (const auto& r : rec) {
            cout << "- " << r << "\n";
        }
        cout << "\n";
    }

    cout << model_learning_sum() << std::flush;

    double acc = directional_accu();
    if (acc > 0) {
        cout << "- Machine Learning accuracy: " << acc << "%" << double_endl;
    } else {
        cout << "- Machine Learning accuracy: PENDING\n";
        cout << "   - Directional accuracy requires multiple evaluated predictions.\n";
        cout << "   - This metric becomes reliable after ~3–4 weeks of use.\n\n";
    }

    cout <<
        "NOTE: THIS SYSTEM LEARNS WEEKLY USING ONLY YOUR LOCAL DATA\n"
        "PREDICTIONS ARE EVALUATED AND REFINED AS OUTCOMES ARRIVE\n"
        "NO CLOUD PROCESSING. NO SUBSCRIPTIONS" << double_endl;

    cout << "ANALYSIS HAS FINISHED\n";
    return 0;
}
