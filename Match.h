#ifndef MATCH_H
#define MATCH_H


#include"Student.h"
#include"Str.h"

class Match {
private:
    Student* requester;
    Student* provider;
    Str skillRequestedByRequester; 
    Str skillRequestedByProvider; 
    float score;

public:
    Match();
    Match(Student* requester, Student* provider,
        Str skillRequestedByRequester, Str skillRequestedByProvider,
        float score);

    Student* getRequester() const;
    Student* getProvider() const;
    Str getSkillA() const; // skillRequestedByRequester
    Str getSkillB() const; // skillRequestedByProvider
    float getScore() const;
    Student* getOtherStudent(Student* current) const {
        if (current == requester) return provider;
        if (current == provider) return requester;
        return nullptr; // or throw exception
    }

};
#endif // !MATCH_H