#include "HungarianMatcher.h"
#include <iostream>

unsigned HungarianMatcher::simpleHash(const Str& str) {
    unsigned hash = 0;
    const char* s = str.return_array();
    while (*s) {
        hash = hash * 31 + *s++;
    }
    return hash;
}

// Check if both students request what the other offers
int HungarianMatcher::findBidirectionalSkillScore(Student* a, Student* b) {
    bool a_requests_b = false;
    bool b_requests_a = false;

    for (int i = 0; i < a->getRequestedSkills().size(); ++i) {
        for (int j = 0; j < b->getOfferedSkills().size(); ++j) {
            if (a->getRequestedSkills()[i]->getName() == b->getOfferedSkills()[j]->getName()) {
                a_requests_b = true;
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

    return (a_requests_b && b_requests_a) ? (a->getReputationPoints() + b->getReputationPoints()) : 0;
}

Str HungarianMatcher::findCommonSkill(Student* requester, Student* provider) {
    for (int i = 0; i < requester->getRequestedSkills().size(); ++i) {
        for (int j = 0; j < provider->getOfferedSkills().size(); ++j) {
            if (requester->getRequestedSkills()[i]->getName() == provider->getOfferedSkills()[j]->getName()) {
                std::cout << "Correct match: "
                    << requester->getEmail().return_array() << " wants "
                    << requester->getRequestedSkills()[i]->getName().return_array() << " which "
                    << provider->getEmail().return_array() << " offers\n";
                return requester->getRequestedSkills()[i]->getName();
            }
        }
    }
    return Str("");
}

Dynamic_array<Match*> HungarianMatcher::generateMatches(Dynamic_array<Student*>& students) {
    std::cout << "DEBUG: Generating matches between " << students.size() << " students\n";
    Dynamic_array<Match*> finalMatches;

    Match* bestMatch = nullptr;
    int maxScore = -1;

    for (int i = 0; i < students.size(); ++i) {
        for (int j = 0; j < students.size(); ++j) {
            if (i == j) continue;

            Student* a = students[i];
            Student* b = students[j];

            bool a_requests_b = false;
            bool b_requests_a = false;

            // Check A wants B
            Str skillA;
            for (int m = 0; m < a->getRequestedSkills().size(); ++m) {
                for (int n = 0; n < b->getOfferedSkills().size(); ++n) {
                    if (a->getRequestedSkills()[m]->getName() == b->getOfferedSkills()[n]->getName()) {
                        skillA = a->getRequestedSkills()[m]->getName();
                        a_requests_b = true;
                        break;
                    }
                }
                if (a_requests_b) break;
            }

            // Check B wants A
            Str skillB;
            for (int m = 0; m < b->getRequestedSkills().size(); ++m) {
                for (int n = 0; n < a->getOfferedSkills().size(); ++n) {
                    if (b->getRequestedSkills()[m]->getName() == a->getOfferedSkills()[n]->getName()) {
                        skillB = b->getRequestedSkills()[m]->getName();
                        b_requests_a = true;
                        break;
                    }
                }
                if (b_requests_a) break;
            }

            if (a_requests_b && b_requests_a) {
                int combinedScore = a->getReputationPoints() + b->getReputationPoints();
                std::cout << "Bidirectional match: " << a->getEmail().return_array() << " <-> " << b->getEmail().return_array() << " | Score: " << combinedScore << "\n";

                if (combinedScore > maxScore) {
                    delete bestMatch;
                    bestMatch = new Match(a, b, skillA, skillB, combinedScore);
                    maxScore = combinedScore;
                }
            }
        }
    }

    if (bestMatch) {
        finalMatches.push(bestMatch);
    }

    return finalMatches;
}
