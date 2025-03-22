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

int getSumDay(int year, int month, int day) {
    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int totalDays     = 0;

    if (isLeapYear(year)) {
        daysInMonth[1] = 29;
    }

    for (int i = 0; i < month - 1; i++) {
        totalDays += daysInMonth[i];
    }
    totalDays += day;

    return totalDays;
}

int calcDayOfWeek(int year, int month, int day) {
    int w;
    if (month == 1 || month == 2) month += 12, year--;
    w = (day + 2 * month + 3 * (month + 1) / 5 + year + year / 4 - year / 100 + year / 400 + 1) % 7;
    return w;
}

int getDaysInMonth(int month, int year) {
    int daysInMonth = 30;
    switch (month) {
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
