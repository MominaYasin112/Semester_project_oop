#ifndef MATCHER_H
#define MATCHER_H

#include "Match.h"
#include "Student.h"
#include "Dynamic_array.h"

class Matcher {
public:
    virtual Dynamic_array<Match*> generateMatches(Dynamic_array<Student*>& students) = 0;

    virtual ~Matcher() {}
};

#endif
