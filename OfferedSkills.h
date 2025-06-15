#ifndef OFFEREDSKILL_H
#define OFFEREDSKILL_H

#include "Dynamic_array.h"
#include "Skill.h"
#include "TimeSlot.h"
#include<iostream>
using namespace std;
class OfferedSkill : public Skill {
    Dynamic_array<TimeSlot> availableSlots;
    int studentId;
    bool isMatched;
public:
    OfferedSkill();
    OfferedSkill(int id, Str name, Str category, Str description, Dynamic_array<TimeSlot> slots, int studentId, bool matched = false);
    OfferedSkill(const OfferedSkill& other);
    OfferedSkill& operator=(const OfferedSkill& other);
    ~OfferedSkill();
    void displayTimeSlots() const;
    bool addTimeSlot(DateTime start, DateTime end);
    bool isAvailable();
    void saveToBinary(ofstream& out) const;
    void loadFromBinary(ifstream& in);
    int getStudentId();
    void setStudentId(int id);

};

#endif