//#include "GreedyMatcher.h"
//
//Dynamic_array<Match*> GreedyMatcher::generateMatches(Dynamic_array<Student*>& students) {
//    Dynamic_array<Match*> matches;
//    bool* used = new bool[students.size()] {}; // Track matched students
//
//    for (int i = 0; i < students.size(); ++i) {
//        Student* requester = students[i];
//        if (used[i]) continue;
//
//        const Dynamic_array<RequestedSkill*>& reqSkills = requester->getRequestedSkills();
//
//        for (int j = 0; j < reqSkills.size(); ++j) {
//            RequestedSkill* req = reqSkills[j];
//            Str reqName = req->getName();
//
//            for (int k = 0; k < students.size(); ++k) {
//                if (i == k || used[k]) continue;
//
//                Student* provider = students[k];
//                const Dynamic_array<OfferedSkill*>& offered = provider->getOfferedSkills();
//
//                for (int l = 0; l < offered.size(); ++l) {
//                    if (offered[l]->getName() == reqName) {
//                        float score = (requester->getReputationPoints() + provider->getReputationPoints()) / 2.0f;
//                        Match* match = new Match(requester, provider, reqName, score);
//                        matches.push(match);
//                        used[i] = used[k] = true;
//                        goto next_requester;
//                    }
//                }
//            }
//        }
//
//    next_requester:
//        continue;
//    }
//
//    delete[] used;
//    return matches;
//}
