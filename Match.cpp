#include"Match.h"

Match::Match(Student* requester, Student* provider,
    Str skillRequestedByRequester, Str skillRequestedByProvider,
    float score)
    : requester(requester),
    provider(provider),
    skillRequestedByRequester(skillRequestedByRequester),
    skillRequestedByProvider(skillRequestedByProvider),
    score(score) {
}
Match::Match() {
    requester = nullptr;
    provider = nullptr;
    skillRequestedByProvider = "";
    skillRequestedByRequester = "";
}
Student* Match::getRequester() const { return requester; }
Student* Match::getProvider() const { return provider; }
Str Match::getSkillA() const { return skillRequestedByRequester; }
Str Match::getSkillB() const { return skillRequestedByProvider; }
float Match::getScore() const { return score; }
