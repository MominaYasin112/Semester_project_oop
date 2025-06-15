#include <iostream>
#include "OfferedSkills.h"
#include "AppException.h"
using namespace std;

OfferedSkill::OfferedSkill() : Skill(), studentId(0) {}

OfferedSkill::OfferedSkill(int id, Str name, Str category, Str description, Dynamic_array<TimeSlot> slots, int studentId, bool matched)
    : Skill(), studentId(studentId), availableSlots(slots) {
    setSkillId(id);
    setName(name);
    setCategory(category);
    setDescription(description);
}
OfferedSkill::OfferedSkill(const OfferedSkill& other)
    : Skill(other), availableSlots(other.availableSlots), studentId(other.studentId) {
}

OfferedSkill& OfferedSkill::operator=(const OfferedSkill& other) {
    if (this != &other) {
        Skill::operator=(other);
        availableSlots = other.availableSlots;
        studentId = other.studentId;
    }
    return *this;
}

OfferedSkill::~OfferedSkill() {}

bool OfferedSkill::addTimeSlot(DateTime start, DateTime end) {
    TimeSlot slot(start, end);
    if (!slot.isValid()) {
        std::cerr << "Invalid time slot!\n";
        return false;
    }
    availableSlots.push(slot);
    return true;
}

bool OfferedSkill::isAvailable() {
    for (int i = 0; i < availableSlots.size(); ++i) {
        if (availableSlots[i].isAvailable())
            return true;
    }
    return false;
}

void OfferedSkill::displayTimeSlots() const {
    std::cout << "Time Slots for Skill: " << getName() << std::endl;
    for (int i = 0; i < availableSlots.size(); ++i) {
        std::cout << i + 1 << ": ";
        availableSlots[i].display();
    }
}
void OfferedSkill::saveToBinary(ofstream& out) const {
    Skill::saveToBinary(out);
    out.write((char*)&studentId, sizeof(studentId));
    out.write((char*)&isMatched, sizeof(isMatched));

    int slotCount = availableSlots.size();
    out.write((char*)&slotCount, sizeof(slotCount));
    for (int i = 0; i < slotCount; ++i) {
        availableSlots[i].saveToBinary(out);
    }
}

void OfferedSkill::loadFromBinary(ifstream& in) {
    Skill::loadFromBinary(in);
    in.read((char*)&studentId, sizeof(studentId));
    in.read((char*)&isMatched, sizeof(isMatched));

    int slotCount;
    in.read((char*)&slotCount, sizeof(slotCount));
    availableSlots.clear();
    for (int i = 0; i < slotCount; ++i) {
        TimeSlot ts;
        ts.loadFromBinary(in);
        availableSlots.push(ts);
    }
}
int OfferedSkill::getStudentId()
{
    return studentId;
}
void OfferedSkill::setStudentId(int id)
{
    studentId = id;
}