#include "MatchingEngine.h"
#include <iostream>

Dynamic_array<Match*> MatchingEngine::matchStudents(Dynamic_array<Student*>& students) {
    if (!matcher) {
        std::cout << "DEBUG: No matcher available\n";
        return Dynamic_array<Match*>();
    }
    return matcher->generateMatches(students);
}

Exchange* MatchingEngine::createExchangeFromMatch(Match* match, int exchangeId) {
    if (!match) {
        std::cout << "DEBUG: Null match provided\n";
        return nullptr;
    }

    std::cout << "DEBUG: Attempting to create exchange between "
        << match->getRequester()->getEmail().return_array() << " and "
        << match->getProvider()->getEmail().return_array() << "\n";

    // Get offered skill (make a copy)
    OfferedSkill* offered = nullptr;
    for (auto skill : match->getProvider()->getOfferedSkills()) {
        if (skill->getName() == match->getSkill()) {
            offered = new OfferedSkill(*skill); // Create copy
            break;
        }
    }

    if (!offered) {
        std::cout << "DEBUG: Failed to find offered skill '"
            << match->getSkill().return_array() << "'\n";
        return nullptr;
    }

    // Get requested skill (make a copy)
    RequestedSkill* requested = nullptr;
    for (auto skill : match->getRequester()->getRequestedSkills()) {
        if (skill->getName() == match->getSkill()) {
            requested = new RequestedSkill(*skill); // Create copy
            break;
        }
    }

    if (!requested) {
        std::cout << "DEBUG: Failed to find requested skill '"
            << match->getSkill().return_array() << "'\n";
        delete offered; \
        return nullptr;
    }

    std::cout << "DEBUG: Successfully created exchange #" << exchangeId << "\n";
    return new Exchange(
        exchangeId,
        offered,
        requested,
        "Pending",
        DateTime::now()
    );
}