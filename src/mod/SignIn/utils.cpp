#include "mod/SignIn/SignIn.h"

#include <ctime>

namespace signin::utils {

std::tm getCurrentTime() {
    struct std::tm tm;
    std::time_t    now;
    std::time(&now);
    localtime_s(&tm, &now);
    return tm;
}

bool isLeapYear(int year) {
    bool isLeapYear = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    return isLeapYear;
};

int getDaysInMonth(int month, int year) {
    int daysInMonth = 30;
    switch (month + 1) {
    case 1:
    case 3:
    case 5:
    case 7:
    case 8:
    case 10:
    case 12:
        daysInMonth = 31;
        break;
    case 4:
    case 6:
    case 9:
    case 11:
        break;
    case 2:
        daysInMonth = isLeapYear(year) ? 29 : 28;
        break;
    }
    return daysInMonth;
}
} // namespace signin::utils
