#ifndef REQUESTEDSKILL_H
#define REQUESTEDSKILL_H

#include "Str.h"
#include "Skill.h"
#include "DateTime.h"

class RequestedSkill : public Skill {
public:
    DateTime deadline;
    Str status;
    int studentId;

    RequestedSkill();
    RequestedSkill(int id, Str name, Str category, Str description, DateTime deadline, Str status, int studentId);
    RequestedSkill(const RequestedSkill& other);
    RequestedSkill& operator=(const RequestedSkill& other);
    ~RequestedSkill();
    void saveToBinary(ofstream& out) const;
    void loadFromBinary(ifstream& in);
    void setDeadline(DateTime date);
    void updateStatus(Str newStatus);
    int getStudentId();
    void setStudentId(int id);
    DateTime get_deadline();
};

#endif
