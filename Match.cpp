#include "Match.h"

Match::Match(Student* requester, Student* provider, Str skill, float score)
    : requester(requester), provider(provider), skill(skill), score(score) {
}

Student* Match::getRequester() const {
    return requester;
}

Student* Match::getProvider() const {
    return provider;
}

Str Match::getSkill() const {
    return skill;
}

float Match::getScore() const {
    return score;
}
