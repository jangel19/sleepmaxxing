#include <iostream>
#include "TimeGrid.h"
using namespace std;

int main() {
    TimeGrid time(15);

    for (int i = 0; i < 100; i++) {
        cout << "steps " << i << "  day " << time.dayIndex() << "  min of day " << time.minuteOfDay()
            << "  hours since beginning " << time.hoursSinceStart() << "  is it a nwe day?" 
            << (time.isNewDay() ? "YES" : "no") << endl;
        time.advance();
    }
    return 0;
}
