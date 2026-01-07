#include "dataset_prepper.hpp"

vector<Sample> build_dataset(const vector<DayRecord> &days)
{
    vector<Sample> dataset;

    for (size_t i = 0; i + WINDOW_SIZE + LABEL_WINDOW <= days.size(); i++)
    {
        vector<DayRecord> feature_window(days.begin() + i, days.begin() + i + WINDOW_SIZE);
        vector<DayRecord> label_window(
            days.begin() + i + WINDOW_SIZE,
            days.begin() + i + WINDOW_SIZE + LABEL_WINDOW);

        Sample s;
        s.x = extract_features(feature_window);
        if (!s.x.is_finite())
            continue;

        s.y = compute_actual_hrv_delta(days, days[i + WINDOW_SIZE - 1].date, 7);
        dataset.push_back(s);
    }
    return dataset;
}
