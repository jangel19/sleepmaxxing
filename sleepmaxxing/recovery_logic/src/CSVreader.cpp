#include "CSVreader.hpp"
#include <sstream>
#include <fstream>
#include <iostream>
#include <optional>

// parsers to validate data
static std::optional<int> int_parser(const std::string& s, int _min, int _max) {
    if (s.empty()) return std::nullopt;
    try {
        int v = std::stoi(s);
        if (v < _min || v > _max) return std::nullopt;
        return v;
    } catch (...) {
        return std::nullopt;
    }
}

static std::optional<int> wake_parser(const std::string& s) {
    if (s.empty()) return std::nullopt;

    if (std::isdigit(s[0])) { // added this for formatting issues
        try {
            int v = std::stoi(s);
            if (v >= 0 && v < 1440) return v;
        } catch (...) {};
    }

    int hh, mm;
    char colon;
    std::istringstream iss(s);
    if (!(iss >> hh >> colon >> mm) || colon != ':') return std::nullopt;
    if (hh < 0 || hh > 23 || mm < 0 || mm > 59) return std::nullopt;

    return hh * 60 + mm;
}

static std::optional<double> double_parser(const std::string& s) {
    if (s.empty()) return std::nullopt;
    try {
        double value = std::stod(s);
        if (value < 0) return std::nullopt;
        return value;
    } catch (...) {
        return std::nullopt;
    }
}


std::vector<DayRecord> load_csv(const std::string& path){
    std::ifstream file(path);
    std::vector<DayRecord> data;

    if(!file.is_open()) {
        std::cerr << "could not open file " << path << std::endl;
        return data;
    }

    std::string line;
    std::getline(file, line);

    while(std::getline(file, line)) {
        std::stringstream ss(line);
        DayRecord user_data;
        std::string sleep_min_str, wake_min_str, activity_str, hrv_str, stress_str, rhr_str;

        std::getline(ss, user_data.date, ',');
        if (user_data.date.empty()) {
            std::cerr << "skipping row it is missing value" << std::endl;
            continue;
        }

        std::getline(ss, sleep_min_str, ',');
        user_data.sleep_min = int_parser(sleep_min_str, 0, 1440);

        std::getline(ss, wake_min_str, ',');
        user_data.wake_min = wake_parser(wake_min_str);

        std::getline(ss, activity_str, ',');
        user_data.activity = double_parser(activity_str);

        std::getline(ss, hrv_str, ',');
        user_data.hrv = double_parser(hrv_str);

        std::getline(ss, stress_str, ',');
        user_data.stress = double_parser(stress_str);

        std::getline(ss, rhr_str, ',');
        user_data.rhr = double_parser(rhr_str);

        data.push_back(user_data);

        #if DEBUG
        std::cerr << "loaded" << data.size() << " rows" std::endl;
        #endif

    }
    return data;

}
