#ifndef MATCHING_ENGINE_H
#define MATCHING_ENGINE_H

#include "Matcher.h"
#include "ExchangeManager.h"

class MatchingEngine {
private:
    Matcher* matcher;
    ExchangeManager* exchangeManager;

public:
    MatchingEngine(Matcher* matcher) : matcher(matcher), exchangeManager(nullptr) {}
    MatchingEngine(Matcher* matcher, ExchangeManager* exchangeManager)
        : matcher(matcher), exchangeManager(exchangeManager) {
    }
    Dynamic_array<Match*> matchStudents(Dynamic_array<Student*>& students);
    Exchange* createExchangeFromMatch(Match* match, int exchangeId);
};

#endif