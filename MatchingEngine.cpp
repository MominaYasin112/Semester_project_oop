#include "MatchingEngine.h"
MatchingEngine::MatchingEngine(Matcher* strategy) {
    this->strategy = strategy;
}
void MatchingEngine::setStrategy(Matcher* strategy) {
    this->strategy = strategy;
}
Dynamic_array<Match*> MatchingEngine::matchStudents(Dynamic_array<Student*>& students) {
    return strategy->generateMatches(students);
}
Exchange* MatchingEngine::createExchangeFromMatch(Match* match, int exchangeId) {
    OfferedSkill* offered = nullptr;
    RequestedSkill* requested = nullptr;

    // Find the correct offered skill by skill name
    for (int i = 0; i < match->getProvider()->getOfferedSkills().size(); i++) {
        if (match->getProvider()->getOfferedSkills()[i]->getName().is_equal(match->getSkill())) {
            offered = match->getProvider()->getOfferedSkills()[i];
            break;
        }
    }
    for (int i = 0; i < match->getRequester()->getRequestedSkills().size(); i++) {
        if (match->getRequester()->getRequestedSkills()[i]->getName().is_equal(match->getSkill())) {
            requested = match->getRequester()->getRequestedSkills()[i];
            break;
        }
    }
    DateTime a(20, 02, 1, 23, 1);
    if (offered != nullptr && requested != nullptr) {
        return new Exchange(exchangeId, offered, requested, Str("pending"), DateTime::now());
    }

    return nullptr;
}
