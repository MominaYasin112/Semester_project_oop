#include "RequestedSkills.h"
#include<iostream>
using namespace std;
RequestedSkill::RequestedSkill() : Skill(), deadline(), status(""), studentId(0) {}

RequestedSkill::RequestedSkill(int id, Str name, Str category, Str description, DateTime deadline, Str status, int studentId)
    : Skill(), deadline(deadline), status(status), studentId(studentId) {
    setSkillId(id);          
    setName(name);               
    setCategory(category);     
    setDescription(description);
}

RequestedSkill::RequestedSkill(const RequestedSkill& other)
    : Skill(other), deadline(other.deadline), status(other.status), studentId(other.studentId) {
}

RequestedSkill& RequestedSkill::operator=(const RequestedSkill& other) {
    if (this != &other) {
        Skill::operator=(other);
        deadline = other.deadline;
        status = other.status;
        studentId = other.studentId;
    }
    return *this;
}

RequestedSkill::~RequestedSkill() {}

void RequestedSkill::setDeadline(DateTime date) {
    deadline = date;
}

void RequestedSkill::updateStatus(Str newStatus) {
    status = newStatus;
}
void RequestedSkill::saveToBinary(std::ofstream& out) const {
    name.writeToBinary(out);
    category.writeToBinary(out);
    description.writeToBinary(out);
    deadline.writeToBinary(out);
}

void RequestedSkill::loadFromBinary(std::ifstream& in) {
    name.readFromBinary(in);
    category.readFromBinary(in);
    description.readFromBinary(in);
    deadline.readFromBinary(in);
}
int RequestedSkill::getStudentId()
{
    return studentId;
}
void  RequestedSkill::setStudentId(int id)
{
    studentId = id;
}
DateTime RequestedSkill::get_deadline()
{
    return deadline;
}