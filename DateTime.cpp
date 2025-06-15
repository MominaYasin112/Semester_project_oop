#include "DateTime.h"
#include<iostream>
#include"fstream"
#include <ctime>
using namespace std;

DateTime::DateTime() : year(0), month(0), day(0), hour(0), minute(0) {}

DateTime::DateTime(int y, int m, int d, int h, int min)
    : year(y), month(m), day(d), hour(h), minute(min) {
}

DateTime::DateTime(const DateTime& other)
    : year(other.year), month(other.month), day(other.day),
    hour(other.hour), minute(other.minute) {
}

DateTime& DateTime::operator=(const DateTime& other) {
    if (this != &other) {
        year = other.year;
        month = other.month;
        day = other.day;
        hour = other.hour;
        minute = other.minute;
    }
    return *this;
}

DateTime::~DateTime() {}

bool DateTime::isValid() const {
    return (year > 0 && month > 0 && month <= 12 && day > 0 && day <= 31 &&
        hour >= 0 && hour < 24 && minute >= 0 && minute < 60);
}

DateTime DateTime::now() {
    std::time_t t = std::time(nullptr);
    std::tm local;
    localtime_s(&local, &t);
    return DateTime(local.tm_year + 1900,
        local.tm_mon + 1,
        local.tm_mday,
        local.tm_hour,
        local.tm_min);
}
void DateTime::dis()const{
    cout << year << " " << month << " " << day << " " << hour << " " << minute<<" ";
}
void DateTime::writeToBinary(ofstream& out) const {
    out.write(reinterpret_cast<const char*>(&day), sizeof(day));
    out.write(reinterpret_cast<const char*>(&month), sizeof(month));
    out.write(reinterpret_cast<const char*>(&year), sizeof(year));
    out.write(reinterpret_cast<const char*>(&hour), sizeof(hour));
    out.write(reinterpret_cast<const char*>(&minute), sizeof(minute));
}

void DateTime::readFromBinary(ifstream& in) {
    in.read(reinterpret_cast<char*>(&day), sizeof(day));
    in.read(reinterpret_cast<char*>(&month), sizeof(month));
    in.read(reinterpret_cast<char*>(&year), sizeof(year));
    in.read(reinterpret_cast<char*>(&hour), sizeof(hour));
    in.read(reinterpret_cast<char*>(&minute), sizeof(minute));
}
bool DateTime::operator<(const DateTime& other) const {
    if (year != other.year) return year < other.year;
    if (month != other.month) return month < other.month;
    if (day != other.day) return day < other.day;
    if (hour != other.hour) return hour < other.hour;
    return minute < other.minute;
}

bool DateTime::operator>(const DateTime& other) const {
    if (year != other.year) return year > other.year;
    if (month != other.month) return month > other.month;
    if (day != other.day) return day > other.day;
    if (hour != other.hour) return hour > other.hour;
    return minute > other.minute;
}
Str DateTime::toString() const {
    char buffer[50];
    snprintf(buffer, sizeof(buffer), "%02d:%02d", hour, minute);
    return Str(buffer);
}
DateTime DateTime::addDays(int num) const {
    static const int daysInMonth[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };

    auto isLeapYear = [](int y) {
        return (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
        };

    DateTime result = *this;

    while (num > 0) {
        int dim = daysInMonth[result.month - 1];
        if (result.month == 2 && isLeapYear(result.year)) dim = 29;

        if (result.day + num <= dim) {
            result.day += num;
            return result;
        }
        else {
            num -= (dim - result.day + 1);
            result.day = 1;
            result.month++;

            if (result.month > 12) {
                result.month = 1;
                result.year++;
            }
        }
    }
    while (num < 0) {
        result.day--;
        if (result.day < 1) {
            result.month--;
            if (result.month < 1) {
                result.month = 12;
                result.year--;
            }

            int dim = daysInMonth[result.month - 1];
            if (result.month == 2 && isLeapYear(result.year)) dim = 29;

            result.day = dim;
        }
        num++;
    }

    return result;
}
void DateTime::setDate(int d, int m, int y) {
    if (d >= 1 && d <= 31 && m >= 1 && m <= 12 && y >= 1900) {
        day = d;
        month = m;
        year = y;
    }
}

void DateTime::setTime(int h, int m) {
    if (h >= 0 && h < 24 && m >= 0 && m < 60) {
        hour = h;
        minute = m;
    }
}
int DateTime::getDay() const { return day; }
int DateTime::getMonth() const { return month; }
int DateTime::getYear() const { return year; }
int DateTime::getHour() const { return hour; }
int DateTime::getMinute() const { return minute; }
