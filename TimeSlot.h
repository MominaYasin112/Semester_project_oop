#ifndef TIMESLOT_H
#define TIMESLOT_H

#include "DateTime.h"

class TimeSlot {
private:
    DateTime startTime;
    DateTime endTime;
    bool isBooked;

public:
    TimeSlot();
    TimeSlot(DateTime start, DateTime end);
    TimeSlot(const TimeSlot& other);
    TimeSlot& operator=(const TimeSlot& other);
    ~TimeSlot();
    bool book();
    bool isAvailable() const;
    bool isValid() const;
    DateTime getStartTime() const;
    DateTime getEndTime() const;
    bool getBookingStatus() const;
    void setStartTime(const DateTime& start);
    void setEndTime(const DateTime& end);
    void setBookingStatus(bool status);
    void display() const;
    Str toString() const;
    void saveToBinary(std::ofstream& out) const;
    void loadFromBinary(std::ifstream& in);
};

#endif

