#ifndef MATCH_H
#define MATCH_H

#include "Student.h"
#include "Str.h"

class Match {
private:
    Student* requester;
    Student* provider;
    Str skill;
    float score; 

public:
    Match(Student* requester, Student* provider, Str skill, float score);
    Student* getRequester() const;
    Student* getProvider() const;
    Str getSkill() const;
    float getScore() const;
};

#endif

