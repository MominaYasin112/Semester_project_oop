#include "HungarianMatcher.h"
#include <limits>
int HungarianMatcher::findBidirectionalSkillScore(Student* a, Student* b) {
    bool a_requests_b = false;
    bool b_requests_a = false;
    Str commonSkill;

    std::cout << "[MATCH CHECK] " << a->getEmail().return_array()
        << " vs " << b->getEmail().return_array() << std::endl;

    for (int i = 0; i < a->getRequestedSkills().size(); ++i) {
        for (int j = 0; j < b->getOfferedSkills().size(); ++j) {
            std::cout << "    " << a->getRequestedSkills()[i]->getName().return_array()
                << " ?== " << b->getOfferedSkills()[j]->getName().return_array() << std::endl;
            if (a->getRequestedSkills()[i]->getName().is_equal(b->getOfferedSkills()[j]->getName())) {
                a_requests_b = true;
                commonSkill = a->getRequestedSkills()[i]->getName();
                std::cout << " a requests skill offered by b: " << commonSkill.return_array() << std::endl;
                break;
            }
        }
        if (a_requests_b) break;
    }

    for (int i = 0; i < b->getRequestedSkills().size(); ++i) {
        for (int j = 0; j < a->getOfferedSkills().size(); ++j) {
            std::cout << "    " << b->getRequestedSkills()[i]->getName().return_array()
                << " ?== " << a->getOfferedSkills()[j]->getName().return_array() << std::endl;
            if (b->getRequestedSkills()[i]->getName().is_equal(a->getOfferedSkills()[j]->getName())) {
                b_requests_a = true;
                std::cout << " b requests skill offered by a" << std::endl;
                break;
            }
        }
        if (b_requests_a) break;
    }

    if (a_requests_b && b_requests_a) {
        std::cout << "Bidirectional Match Found Between "
            << a->getEmail().return_array() << " and "
            << b->getEmail().return_array() << " for "
            << commonSkill.return_array() << std::endl;
    }

    return (a_requests_b && b_requests_a) ?
        (a->getReputationPoints() + b->getReputationPoints()) / 2 :
        0;
}

int** HungarianMatcher::createCostMatrix(const Dynamic_array<Student*>& students, int& size) {
    size = students.size();
    int** costMatrix = new int* [size];
    for (int i = 0; i < size; ++i) {
        costMatrix[i] = new int[size];
        for (int j = 0; j < size; ++j) {
            if (i == j) {
                costMatrix[i][j] = std::numeric_limits<int>::max();
            }
            else {
                costMatrix[i][j] = -findBidirectionalSkillScore(students[i], students[j]);
            }
        }
    }
    return costMatrix;
}
void HungarianMatcher::freeCostMatrix(int** matrix, int size) {
    for (int i = 0; i < size; ++i) {
        delete[] matrix[i];
    }
    delete[] matrix;
}
Dynamic_array<int> HungarianMatcher::hungarianAlgorithm(int** costMatrix, int size) {
    const int INF = std::numeric_limits<int>::max();
    Dynamic_array<int> u(size + 1, 0);
    Dynamic_array<int> v(size + 1, 0);
    Dynamic_array<int> p(size + 1, 0);
    Dynamic_array<int> way(size + 1, 0);

    for (int i = 1; i <= size; ++i) {
        p[0] = i;
        int j0 = 0;
        Dynamic_array<int> minv(size + 1, INF);
        Dynamic_array<bool> used(size + 1, false);

        do {
            used[j0] = true;
            int i0 = p[j0], delta = INF, j1 = 0;
            for (int j = 1; j <= size; ++j) {
                if (!used[j]) {
                    int cur = costMatrix[i0 - 1][j - 1] - u[i0] - v[j];
                    if (cur < minv[j]) {
                        minv[j] = cur;
                        way[j] = j0;
                    }
                    if (minv[j] < delta) {
                        delta = minv[j];
                        j1 = j;
                    }
                }
            }

            for (int j = 0; j <= size; ++j) {
                if (used[j]) {
                    u[p[j]] += delta;
                    v[j] -= delta;
                }
                else {
                    minv[j] -= delta;
                }
            }
            j0 = j1;
        } while (p[j0] != 0);
        do {
            int j1 = way[j0];
            p[j0] = p[j1];
            j0 = j1;
        } while (j0 != 0);
    }
    Dynamic_array<int> result(size, -1);
    for (int j = 1; j <= size; ++j) {
        if (p[j] != 0 && p[j] - 1 != j - 1) {
            result[p[j] - 1] = j - 1;
        }
    }

    return result;
}

Dynamic_array<Match*> HungarianMatcher::generateOptimizedMatches(Dynamic_array<Student*>& students) {
    int size;
    int** costMatrix = createCostMatrix(students, size);
    Dynamic_array<int> matches = hungarianAlgorithm(costMatrix, size);
    std::cout << "=== Final Match Pairs ===" << std::endl;
    for (int i = 0; i < size; ++i) {
        std::cout << "Student[" << i << "] (" << students[i]->getEmail().return_array() << ") matched with "
            << "Student[" << matches[i] << "]" << std::endl;
    }

    Dynamic_array<Match*> result;
    for (int i = 0; i < size; ++i) {
        int j = matches[i];
        if (j != -1 && i != j) {
            Str matchSkill = findCommonSkill(students[i], students[j]);
            int score = findBidirectionalSkillScore(students[i], students[j]);
            result.push(new Match(students[i], students[j], matchSkill, score));
        }
    }

    freeCostMatrix(costMatrix, size);
    return result;
}
Dynamic_array<Match*> HungarianMatcher::generateMatches(Dynamic_array<Student*>& students) {
    return generateOptimizedMatches(students);
}
Str HungarianMatcher::findCommonSkill(Student* a, Student* b) {
    for (int i = 0; i < a->getRequestedSkills().size(); ++i) {
        for (int j = 0; j < b->getOfferedSkills().size(); ++j) {
            if (a->getRequestedSkills()[i]->getName().is_equal(b->getOfferedSkills()[j]->getName())) {
                return a->getRequestedSkills()[i]->getName();
            }
        }
    }
    return Str("");
}
