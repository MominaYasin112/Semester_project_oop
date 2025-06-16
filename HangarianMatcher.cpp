#include "HungarianMatcher.h"

unsigned HungarianMatcher::simpleHash(const Str& str) {
    unsigned hash = 0;
    const char* s = str.return_array();
    while (*s) {
        hash = hash * 31 + *s++;
    }
    return hash;
}

int HungarianMatcher::findBidirectionalSkillScore(Student* a, Student* b) {
    bool a_requests_b = false;
    bool b_requests_a = false;
    Str commonSkill;

    // Check if a requests something b offers
    for (int i = 0; i < a->getRequestedSkills().size(); ++i) {
        for (int j = 0; j < b->getOfferedSkills().size(); ++j) {
            if (a->getRequestedSkills()[i]->getName() == b->getOfferedSkills()[j]->getName()) {
                a_requests_b = true;
                commonSkill = a->getRequestedSkills()[i]->getName();
                break;
            }
        }
        if (a_requests_b) break;
    }
    for (int i = 0; i < b->getRequestedSkills().size(); ++i) {
        for (int j = 0; j < a->getOfferedSkills().size(); ++j) {
            if (b->getRequestedSkills()[i]->getName() == a->getOfferedSkills()[j]->getName()) {
                b_requests_a = true;
                break;
            }
        }
        if (b_requests_a) break;
    }

    return (a_requests_b && b_requests_a) ?
        (a->getReputationPoints() + b->getReputationPoints()) :
        0;
}

Dynamic_array<Match*> HungarianMatcher::generateMatches(Dynamic_array<Student*>& students) {
  
    std::cout << "DEBUG: Generating matches between " << students.size() << " students\n";

    Dynamic_array<Match*> possibleMatches;

    for (int i = 0; i < students.size(); ++i) {
        for (int j = i + 1; j < students.size(); ++j) {
            // Check both directions
            Str skill1 = findCommonSkill(students[i], students[j]);
            Str skill2 = findCommonSkill(students[j], students[i]);

            if (!skill1.empty()) {
                possibleMatches.push(new Match(
                    students[i],
                    students[j],
                    skill1,
                    students[i]->getReputationPoints() + students[j]->getReputationPoints()
                ));
            }

            if (!skill2.empty()) {
                possibleMatches.push(new Match(
                    students[j],
                    students[i],
                    skill2,
                    students[i]->getReputationPoints() + students[j]->getReputationPoints()
                ));
            }
        }
    }

    Dynamic_array<Match*> finalMatches;

    if (!possibleMatches.isEmpty()) {
        // Select the match with highest score
        int maxScore = 0;
        for (int i = 0; i < possibleMatches.size(); ++i) {
            if (possibleMatches[i]->getScore() > maxScore) {
                maxScore = possibleMatches[i]->getScore();
            }
        }

        // Collect all matches with max score
        Dynamic_array<Match*> bestMatches;
        for (int i = 0; i < possibleMatches.size(); ++i) {
            if (possibleMatches[i]->getScore() == maxScore) {
                bestMatches.push(possibleMatches[i]);
            }
        }

        // Select one match (deterministically) if multiple have same score
        if (!bestMatches.isEmpty()) {
            unsigned bestHash = 0;
            int bestIndex = 0;

            for (int i = 0; i < bestMatches.size(); ++i) {
                Str combinedEmail = bestMatches[i]->getRequester()->getEmail() +
                    bestMatches[i]->getProvider()->getEmail();
                unsigned currentHash = simpleHash(combinedEmail);

                if (currentHash > bestHash) {
                    bestHash = currentHash;
                    bestIndex = i;
                }
            }
            finalMatches.push(bestMatches[bestIndex]);
        }

        // Clean up unused matches
        for (int i = 0; i < possibleMatches.size(); ++i) {
            bool isUsed = false;
            for (int j = 0; j < finalMatches.size(); ++j) {
                if (possibleMatches[i] == finalMatches[j]) {
                    isUsed = true;
                    break;
                }
            }
            if (!isUsed) {
                delete possibleMatches[i];
            }
        }
    }

    return finalMatches;
}

Str HungarianMatcher::findCommonSkill(Student* requester, Student* provider) {
    for (auto reqSkill : requester->getRequestedSkills()) {
        for (auto provSkill : provider->getOfferedSkills()) {
            if (reqSkill->getName() == provSkill->getName()) {
                std::cout << "Correct match: "
                    << requester->getEmail().return_array() << " wants "
                    << reqSkill->getName().return_array() << " which "
                    << provider->getEmail().return_array() << " offers\n";
                return reqSkill->getName();
            }
        }
    }
    return Str("");
}