#ifndef MATCHING_ENGINE_H
#define MATCHING_ENGINE_H

#include "Matcher.h"
#include "Match.h"
#include "Exchange.h"
#include "DateTime.h"
#include "Student.h"
#include "Dynamic_array.h"

class MatchingEngine {
    Matcher* strategy;

public:
    MatchingEngine(Matcher* strategy);
    void setStrategy(Matcher* strategy);
    Dynamic_array<Match*> matchStudents(Dynamic_array<Student*>& students);
    Exchange* createExchangeFromMatch(Match* match, int exchangeId);
};

#endif

