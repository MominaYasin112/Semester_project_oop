#include "Student.h"
#include <iostream>
#include"AppException.h"
Student::Student() : User(), reputationPoints(0) {
}
Student::Student(Str email, Str passwordHash, int reputationPoints)
    : User(email, passwordHash), reputationPoints(reputationPoints) {
}
Student::Student(const Student& other) : User(other), reputationPoints(other.reputationPoints) {
    for (int i = 0; i < other.offeredSkills.size(); i++) {
        OfferedSkill* skill = new OfferedSkill(*(other.offeredSkills[i]));
        offeredSkills.push(skill);
    }
    for (int i = 0; i < other.requestedSkills.size(); i++) {
        RequestedSkill* skill = new RequestedSkill(*(other.requestedSkills[i]));
        requestedSkills.push(skill);
    }
}
Student& Student::operator=(const Student& other) {
    if (this != &other) {
        User::operator=(other);
        for (int i = 0; i < offeredSkills.size(); i++) {
            delete offeredSkills[i];
        }
        offeredSkills.clear();

        for (int i = 0; i < requestedSkills.size(); i++) {
            delete requestedSkills[i];
        }
        requestedSkills.clear();
        reputationPoints = other.reputationPoints;
        for (int i = 0; i < other.offeredSkills.size(); i++) {
            OfferedSkill* skill = new OfferedSkill(*(other.offeredSkills[i]));
            offeredSkills.push(skill);
        }
        for (int i = 0; i < other.requestedSkills.size(); i++) {
            RequestedSkill* skill = new RequestedSkill(*(other.requestedSkills[i]));
            requestedSkills.push(skill);
        }
    }
    return *this;
}
Student::~Student() {
    for (int i = 0; i < offeredSkills.size(); i++) {
        delete offeredSkills[i];
    }

    for (int i = 0; i < requestedSkills.size(); i++) {
        delete requestedSkills[i];
    }
}
int Student::getReputationPoints() const {
    return reputationPoints;
}

const Dynamic_array<OfferedSkill*>& Student::getOfferedSkills() const {
    return offeredSkills;
}

const Dynamic_array<RequestedSkill*>& Student::getRequestedSkills() const {
    return requestedSkills;
}
void Student::setReputationPoints(int points) {
    reputationPoints = points;
}
bool Student::canRegister() {
    return (reputationPoints >= 0 && isLoggedIn);
}

bool Student::addOfferedSkill(OfferedSkill* skill) {
    if (skill == nullptr) return false;

    try {
        skill->setName(skill->getName());
        skill->setDescription(skill->getDescription());
    }
    catch (SkillValidationException& ex) {
        std::cout << "Error adding offered skill: " << ex.getMessage() << std::endl;
        return false;
    }

    offeredSkills.push(skill);
    return true;
}

bool Student::addRequestedSkill(RequestedSkill* skill) {
    if (skill == nullptr) return false;

    try {
        skill->setName(skill->getName());
        skill->setDescription(skill->getDescription());
    }
    catch (SkillValidationException& ex) {
        std::cout << "Error adding requested skill: " << ex.getMessage() << std::endl;
        return false;
    }

    requestedSkills.push(skill);
    return true;
}
void Student::saveToBinary(ofstream& out) const {
    User::saveToBinary(out);
    out.write((char*)&reputationPoints, sizeof(reputationPoints));

    int offerCount = offeredSkills.size();
    out.write((char*)&offerCount, sizeof(offerCount));
    for (int i = 0; i < offerCount; ++i)
        offeredSkills[i]->saveToBinary(out);

    int reqCount = requestedSkills.size();
    out.write((char*)&reqCount, sizeof(reqCount));
    for (int i = 0; i < reqCount; ++i)
        requestedSkills[i]->saveToBinary(out);

    int exchCount = exchanges.size();
    out.write((char*)&exchCount, sizeof(exchCount));
    for (int i = 0; i < exchCount; ++i) {
        int id = exchanges[i]->getId();
        out.write((char*)&id, sizeof(id));
    }
}

void Student::loadFromBinary(ifstream& in) {
    User::loadFromBinary(in);
    in.read((char*)&reputationPoints, sizeof(reputationPoints));

    int offerCount;
    in.read((char*)&offerCount, sizeof(offerCount));
    offeredSkills.clear();
    for (int i = 0; i < offerCount; ++i) {
        OfferedSkill* skill = new OfferedSkill();
        skill->loadFromBinary(in);
        offeredSkills.push(skill);
    }

    int reqCount;
    in.read((char*)&reqCount, sizeof(reqCount));
    requestedSkills.clear();
    for (int i = 0; i < reqCount; ++i) {
        RequestedSkill* skill = new RequestedSkill();
        skill->loadFromBinary(in);
        requestedSkills.push(skill);
    }

    int exchCount;
    in.read((char*)&exchCount, sizeof(exchCount));
    for (int i = 0; i < exchCount; ++i) {
        int eid;
        in.read((char*)&eid, sizeof(eid));
    }
}
bool Student::addExchange(Exchange* ex) {
    exchanges.push(ex);
    return true;
}

const Dynamic_array<Exchange*>& Student::getExchanges() const {
    return exchanges;
}