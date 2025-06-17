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

    std::cout << "DEBUG: Attempting to create bidirectional exchange between "
        << match->getRequester()->getEmail().return_array() << " and "
        << match->getProvider()->getEmail().return_array() << "\n";

    // Skill A: requested by A, offered by B
    Str skillA = match->getSkillA();  // A's requested skill (from B)
    Str skillB = match->getSkillB(); // B's requested skill (from A)

    // Offered skill B -> A
    OfferedSkill* offeredA = nullptr;
    for (auto skill : match->getProvider()->getOfferedSkills()) {
        if (skill->getName() == skillA) {
            offeredA = new OfferedSkill(*skill);
            break;
        }
    }

    // Offered skill A -> B
    OfferedSkill* offeredB = nullptr;
    for (auto skill : match->getRequester()->getOfferedSkills()) {
        if (skill->getName() == skillB) {
            offeredB = new OfferedSkill(*skill);
            break;
        }
    }

    if (!offeredA || !offeredB) {
        std::cout << "DEBUG: Offered skill missing\n";
        delete offeredA;
        delete offeredB;
        return nullptr;
    }

    // Requested skill A (A wants from B)
    RequestedSkill* requestedA = nullptr;
    for (auto skill : match->getRequester()->getRequestedSkills()) {
        if (skill->getName() == skillA) {
            requestedA = new RequestedSkill(*skill);
            break;
        }
    }

    // Requested skill B (B wants from A)
    RequestedSkill* requestedB = nullptr;
    for (auto skill : match->getProvider()->getRequestedSkills()) {
        if (skill->getName() == skillB) {
            requestedB = new RequestedSkill(*skill);
            break;
        }
    }

    if (!requestedA || !requestedB) {
        std::cout << "DEBUG: Requested skill missing\n";
        delete offeredA;
        delete offeredB;
        delete requestedA;
        delete requestedB;
        return nullptr;
    }

    std::cout << "DEBUG: Successfully created bidirectional exchange #" << exchangeId << "\n";

    return new Exchange(
        exchangeId,
        offeredA,
        offeredB,
        requestedA,
        requestedB,
        "Pending",
        DateTime::now()
    );
}
