#ifndef HUNGARIANMATCHER_H
#define HUNGARIANMATCHER_H

#include "Dynamic_array.h"
#include "Student.h"
#include "Match.h"
#include "Matcher.h" 

class HungarianMatcher : public Matcher { 
private:
    int** createCostMatrix(const Dynamic_array<Student*>& students, int& size);
    void freeCostMatrix(int** matrix, int size);
    Dynamic_array<int> hungarianAlgorithm(int** costMatrix, int size);
    int findBidirectionalSkillScore(Student* a, Student* b);

public:
    Dynamic_array<Match*> generateMatches(Dynamic_array<Student*>& students) override;
    Dynamic_array<Match*> generateOptimizedMatches(Dynamic_array<Student*>& students);
    Str findCommonSkill(Student* a, Student* b);
};

#endif
