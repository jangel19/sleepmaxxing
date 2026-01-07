#include "TimeGrid.h"

TimeGrid::TimeGrid(int dt_min)
    : dt_minutes(dt_min), step_index(0), curr_day(0), min_of_day(0), new_day_flag(false) {}

void TimeGrid::advance() {
    new_day_flag = false;

    step_index++;

    min_of_day += dt_minutes;

    if (min_of_day >= 1440) {
        min_of_day -= 1440;
        curr_day++;
        new_day_flag = true;
    }
}

int TimeGrid::dayIndex() const { return curr_day; }

double TimeGrid::hoursSinceStart() const {
    return (step_index * dt_minutes) / 60.0; 
}

int TimeGrid::minuteOfDay() const { return min_of_day; }

bool TimeGrid::isNewDay() const { return new_day_flag; }

double TimeGrid::timeOfDayHours() const { return min_of_day / 60.0; }


