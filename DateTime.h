#ifndef DATETIME_H
#define DATETIME_H

#include <fstream>
using namespace std;

#include "Str.h"
#include <ctime>

class DateTime {
public:
    int year;
    int month;
    int day;
    int hour;
    int minute;

    DateTime();
    DateTime(int year, int month, int day, int hour, int minute);
    DateTime(const DateTime& other);
    DateTime& operator=(const DateTime& other);
    ~DateTime();

    static DateTime now();
    bool isValid() const;
    void dis() const;

    void readFromBinary(ifstream& in);
    void writeToBinary(ofstream& out) const;
    bool operator<(const DateTime& other) const;
    bool operator>(const DateTime& other) const;
    Str toString() const;
    DateTime addDays(int num)const;
    void setDate(int d, int m, int y);
    void setTime(int h, int m);
    int getDay() const;
    int getMonth() const;
    int getYear() const;
    int getHour() const;
    int getMinute() const;
};

#endif
