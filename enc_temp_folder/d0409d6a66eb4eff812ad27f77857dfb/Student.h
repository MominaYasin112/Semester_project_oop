#ifndef STUDENT_H
#define STUDENT_H

#include "User.h"
#include "Dynamic_array.h"
#include "OfferedSkills.h"
#include "RequestedSkills.h"
#include"Exchange.h"
#include<iostream>
using namespace std;
class Student : public User {
private:
    int reputationPoints;
    Dynamic_array<OfferedSkill*> offeredSkills;
    Dynamic_array<RequestedSkill*> requestedSkills;
    Dynamic_array<Exchange*> exchanges;
public:
    Student();
    Student(Str email, Str passwordHash, int reputationPoints);
    Student(const Student& other);
    Student& operator=(const Student& other);
    ~Student();

    int getReputationPoints() const;
    const Dynamic_array<OfferedSkill*>& getOfferedSkills() const;
    const Dynamic_array<RequestedSkill*>& getRequestedSkills() const;
    void setReputationPoints(int points);
    bool canRegister();
    bool addOfferedSkill(OfferedSkill* skill);
    bool addRequestedSkill(RequestedSkill* skill);
    void saveToBinary(ofstream& out) const;
    void loadFromBinary(ifstream& in);
    const Dynamic_array<Exchange*>& getExchanges() const;
    bool addExchange(Exchange* ex);

};

#endif
