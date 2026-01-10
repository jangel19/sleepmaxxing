#include "CSVreader.hpp"
#include "combine.hpp"
#include "feature_extract.hpp"
#include "model.hpp"
#include "explain.hpp"
#include "logger.hpp"
#include "actual_outcome.hpp"
#include "learning_stats.hpp"
#include "has_seen.hpp"

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::endl;
using std::string;


std::ostream& double_endl(std::ostream& os) {
    return os << '\n' << '\n';
}

static size_t find_date_idx(const vector<DayRecord>& days, const string& date) {
    for (size_t i = 0; i < days.size(); i++) {
        if (days[i].date == date) return i;
    }
    return days.size();
}

static void print_base_message(size_t days_seen) {
    cout << "ML Status: Collecting baseline data\n";
    cout << "Days observed: " << days_seen << endl;
    cout << "Prediction will activate after the first evaluation window completes" << double_endl;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "usage: " << argv[0] << "<path to csv> <prediction date (yyyy-mm-dd)>" << endl;
        return 1;
    }

    const string csv_path = argv[1];
    const string pred_date = argv[2];

    vector<DayRecord> days = load_csv(csv_path);
    if (days.empty()) {
        std::cerr << "error: no valid data loaded" << endl;
        return 1;
    }

    cout << "Loaded " << days.size() << " days of data" << endl;

    // analysis time heh
    Combine result = compute_final_results(days);

    cout << "\n--------------RECOVERY SUMMARY-----------------" << double_endl;
    switch (result.rec_sum.trend) {
        case Trend::Improving: cout << "Recovery is Improving. Keep up the Grind" << double_endl;
            break;
        case Trend::Stable: cout << "Recovery is Stable." << double_endl; break;
        case Trend::Declining: cout << "Recovery is Declining. ATTENTION NEEDED" << double_endl; break;
    }

    if (result.rec_sum.confidence < 0.34) cout << "Confidence: Low (limited historical data)" << double_endl;
    else if (result.rec_sum.confidence < 0.67)/*6767lol*/ cout << "Confidence: Moderate" << double_endl;
    else cout << "Confidence: High" << double_endl;

    cout << "Circadian Shift" << double_endl;
    cout << "Shift: " << result.cir_shift.shift_hours << " hours" << endl;
    cout << (result.cir_shift.significant ? "This had a SIGNIFICANT effect on your body"
        : "This DID NOT have a significant effect on your body") << double_endl;

    // get the pred point
    size_t pred_idx = find_date_idx(days, pred_date);
    if (pred_idx == days.size()) {
        std::cerr << "ERROR: prediction date NOT FOUND" << endl;
        return 1;
    }

    vector<DayRecord> history(days.begin(), days.begin() + pred_idx + 1);
    cout << "Prediction Date: " << pred_date << double_endl;

    // training data log
    for (size_t i = 0; i + WINDOW_SIZE + LABEL_WINDOW <= days.size(); i++) {
        const size_t anchor = i + WINDOW_SIZE - 1;
        const string& date = days[anchor].date;

        if (has_seen_date(date)) continue;

        double y = compute_actual_hrv_delta(days, date, LABEL_WINDOW);
        if (y == 0.0) continue;

        vector<DayRecord> window(days.begin() + i,  days.begin() + i + WINDOW_SIZE);

        arma::vec x = extract_features(window);
        if (!x.is_finite()) continue;

        log_training_data(date, x, y);
    }

    // v1 logic (v2 will be even better i promise)
    bool eval_ready = pred_idx + LABEL_WINDOW < days.size();
    if (!eval_ready) {
        print_base_message(history.size());
        cout << "ANALYSIS HAS FINISHED" << endl;
        return 0;
    }

    // ml prediction (v1 is very simple)
    PredictionResult prediction = train_and_predict(history);
    cout << explain_future(result.rec_sum, result.cir_shift, prediction) << double_endl;

    // eval
    double actual = compute_actual_hrv_delta(days, pred_date, LABEL_WINDOW);
    double error = prediction.prediction - actual;

    log_error(pred_date, prediction.prediction, actual, error);

    cout << "ACTUAL HRV CHANGE: " << actual << endl;
    cout << "PREDICTION ERROR: " << error << double_endl;

    // learning stats
    cout << model_learning_sum() << endl;
    double accuracy_ = directional_accu();
    cout << "MACHINE LEARNING DIRECTIONAL ACCURACY: " << (accuracy_ > 0 ? std::to_string(accuracy_)
        + "%" : "PENDING") << double_endl;

    cout <<
        "NOTE: THIS SYSTEM LEARNS WEEKLY USING ONLY YOUR LOCAL DATA\n"
        "PREDICTIONS ARE EVALUATED AND REFINED AS OUTCOMES ARRIVE\n"
        "NO CLOUD PROCESSING. NO SUBSCRIPTIONS" << double_endl;

    cout << "ANALYSIS HAS FINISHED" << endl;
}


// static size_t find_date_index(const std::vector<DayRecord>& days,
//                               const std::string& date)
// {
//     for (size_t i = 0; i < days.size(); i++) {
//         if (days[i].date == date)
//             return i;
//     }
//     return days.size();
// }

// static void print_min_data_message(size_t have, size_t need, const char* label)
// {
//     if (have >= need)
//         return;

//     cout << label << ": insufficient data\n";
//     cout << "Have: " << have << " days\n";
//     cout << "Need: " << need << " days\n";
//     cout << "Missing: " << (need - have) << " days\n\n";
// }

// int main(int argc, char* argv[])
// {
//     if (argc < 3) {
//         std::cerr << "usage: " << argv[0]
//                   << " <path_to_csv> <prediction_date YYYY-MM-DD>\n";
//         return 1;
//     }

//     const string csv_path = argv[1];
//     const string pred_date = argv[2];

//     std::vector<DayRecord> days = load_csv(csv_path);

//     if (days.empty()) {
//         std::cerr << "error: no valid data loaded\n";
//         return 1;
//     }

//     cout << "loaded " << days.size() << " days of data\n";

//     Combine result = compute_final_results(days);

//     cout << "\n------- RECOVERY SUMMARY -------\n\n";

//     switch (result.rec_sum.trend) {
//         case Trend::Improving: cout << "Recovery is Improving\n\n"; break;
//         case Trend::Stable:    cout << "Recovery is Stable\n\n";    break;
//         case Trend::Declining: cout << "Recovery is Declining\n\n"; break;
//     }

//     if (result.rec_sum.confidence < 0.34)
//         cout << "Confidence: Low (limited historical data)\n\n";
//     else if (result.rec_sum.confidence < 0.67)
//         cout << "Confidence: Moderate\n\n";
//     else
//         cout << "Confidence: High\n\n";

//     cout << "Circadian Shift\n\n";
//     cout << "Shift: " << result.cir_shift.shift_hours << " hours\n";
//     cout << "or " << result.cir_shift.shift_hours * 60 << " minutes\n\n";
//     cout << "Did this have a significant effect on your body: "
//          << (result.cir_shift.significant ? "Yes" : "No") << "\n\n";

//     // ---------------- Training data logging ----------------
//     // Need at least WINDOW_SIZE history + LABEL_WINDOW future to generate labels safely.
//     const size_t min_train_days = WINDOW_SIZE + LABEL_WINDOW;

//     if (days.size() < min_train_days) {
//         print_min_data_message(days.size(), min_train_days, "TRAINING DATA");
//     } else {
//         for (size_t i = 0; i + WINDOW_SIZE + LABEL_WINDOW <= days.size(); i++) {

//             const size_t anchor = i + WINDOW_SIZE - 1;
//             if (anchor >= days.size())
//                 break;

//             const string& date = days[anchor].date;

//             if (has_seen_date(date))
//                 continue;

//             double y = compute_actual_hrv_delta(days, date, LABEL_WINDOW);

//             // skip invalid / unavailable outcomes
//             if (y == 0.0)
//                 continue;

//             std::vector<DayRecord> window(
//                 days.begin() + i,
//                 days.begin() + i + WINDOW_SIZE
//             );

//             arma::vec x = extract_features(window);
//             if (!x.is_finite())
//                 continue;

//             log_training_data(date, x, y);
//         }
//     }

//     // ---------------- Prediction ----------------
//     size_t pred_index = find_date_index(days, pred_date);
//     if (pred_index == days.size() - 1) {
//         cout << "(Predicting forward from most recent data)"<< double_endl;
//     } else if (pred_index == days.size()) {
//         std::cerr << "error: prediction date not found" << double_endl;
//         return 1;
//     }

//     std::vector<DayRecord> history(days.begin(),
//                                    days.begin() + pred_index + 1);

//     cout << "Prediction Date: " << pred_date << "\n\n";

//     // Stop early if not enough history for feature window
//     if (history.size() < WINDOW_SIZE) {
//         print_min_data_message(history.size(), WINDOW_SIZE, "ML STATUS");
//         cout << "Predicted HRV: N/A\n\n";
//         cout << model_learning_sum() << "\n";
//         cout << "ML Directional Accuracy: N/A\n\n";
//         cout << "ANALYSIS HAS FINISHED\n";
//         return 0;
//     }

//     PredictionResult prediction = train_and_predict(history);

//     cout << explain_future(result.rec_sum,
//                            result.cir_shift,
//                            prediction) << "\n\n";

//     double actual = compute_actual_hrv_delta(days, pred_date, LABEL_WINDOW);

//     // ---------------- Error logging ----------------
//     if (actual != 0.0) {
//         double pred_value = prediction.prediction;
//         double error = pred_value - actual;

//         log_error(pred_date, pred_value, actual, error);

//         cout << "Actual HRV change: " << actual << "\n";
//         cout << "Prediction error: " << error << "\n\n";
//     } else {
//         cout << "Actual HRV over next "
//              << LABEL_WINDOW << " days: PENDING\n\n";
//     }

//     cout << model_learning_sum() << "\n";

//     double acc = directional_accu();
//     if (acc < 0)
//         cout << "ML Directional Accuracy: N/A\n\n";
//     else
//         cout << "ML Directional Accuracy: " << acc << "%\n\n";

//     cout << "\nNote: These insights are generated locally using your own data.\n"
//             "This projection improves automatically as more weeks of data are added.\n"
//             "No cloud processing. No subscriptions. No external health models.\n\n";

//     cout << "ANALYSIS HAS FINISHED" << endl;

//     return 0;
// }
