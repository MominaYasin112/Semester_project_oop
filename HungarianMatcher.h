#ifndef HUNGARIAN_MATCHER_H
#define HUNGARIAN_MATCHER_H

#include "Student.h"
#include "Dynamic_array.h"
#include "Match.h"
#include "Matcher.h"

class HungarianMatcher : public Matcher {
public:
    Dynamic_array<Match*> generateMatches(Dynamic_array<Student*>& students) override;

private:
    int findBidirectionalSkillScore(Student* a, Student* b);
    Str findCommonSkill(Student* a, Student* b);
    unsigned simpleHash(const Str& str);
};

#endif