#pragma once

class TimeGrid {
 private:
    int dt_minutes;
    int step_index;
    int curr_day;
    int min_of_day;
    bool new_day_flag;
 public:
    // fixed timestamp
    explicit TimeGrid(int dt_minutes);
    // sim by onc timestamp
    double hoursSinceStart() const;
    
    void advance();
    //day counter
    int dayIndex() const;
    
    int minuteOfDay() const;

    bool isNewDay() const;

    double timeOfDayHours() const;
};


