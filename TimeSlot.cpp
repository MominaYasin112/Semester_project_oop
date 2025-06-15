#include "TimeSlot.h"
#include <iostream>
using namespace std;

TimeSlot::TimeSlot() : isBooked(false) {}

TimeSlot::TimeSlot(DateTime start, DateTime end)
    : startTime(start), endTime(end), isBooked(false) {
}

TimeSlot::TimeSlot(const TimeSlot& other)
    : startTime(other.startTime), endTime(other.endTime), isBooked(other.isBooked) {
}

TimeSlot& TimeSlot::operator=(const TimeSlot& other) {
    if (this != &other) {
        startTime = other.startTime;
        endTime = other.endTime;
        isBooked = other.isBooked;
    }
    return *this;
}

TimeSlot::~TimeSlot() {}

bool TimeSlot::book() {
    if (!isBooked) {
        isBooked = true;
        return true;
    }
    return false;
}

bool TimeSlot::isAvailable() const {
    return !isBooked;
}

bool TimeSlot::isValid() const {
    return endTime > startTime;
}

DateTime TimeSlot::getStartTime() const {
    return startTime;
}

DateTime TimeSlot::getEndTime() const {
    return endTime;
}

bool TimeSlot::getBookingStatus() const {
    return isBooked;
}

void TimeSlot::setStartTime(const DateTime& start) {
    startTime = start;
}

void TimeSlot::setEndTime(const DateTime& end) {
    endTime = end;
}

void TimeSlot::setBookingStatus(bool status) {
    isBooked = status;
}

void TimeSlot::display() const {
    std::cout << "Start: ";
    startTime.dis();
    std::cout << ", End: ";
    endTime.dis();
    std::cout << ", Status: " << (isBooked ? "Booked" : "Available") << std::endl;
}
Str TimeSlot::toString() const {
    return startTime.toString().concat(" - ").concat(endTime.toString());
}
#include <fstream> // for ofstream/ifstream

void TimeSlot::saveToBinary(std::ofstream& out) const {
    startTime.writeToBinary(out);
    endTime.writeToBinary(out);   
    out.write(reinterpret_cast<const char*>(&isBooked), sizeof(isBooked));
}

void TimeSlot::loadFromBinary(std::ifstream& in) {
    startTime.readFromBinary(in); 
    endTime.readFromBinary(in); 
    in.read(reinterpret_cast<char*>(&isBooked), sizeof(isBooked));
}

