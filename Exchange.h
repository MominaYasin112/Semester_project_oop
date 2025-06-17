#ifndef EXCHANGE_H
#define EXCHANGE_H

#include "OfferedSkills.h"
#include "RequestedSkills.h"
#include "DateTime.h"
#include "Str.h"
#include <fstream>

class Exchange {
    int exchangeId;
    OfferedSkill* offeredSkillA;  // From first student
    OfferedSkill* offeredSkillB;  // From second student (for bidirectional)
    RequestedSkill* requestedSkillA;  // From first student
    RequestedSkill* requestedSkillB;  // From second student
    Str status;
    DateTime createdDate;
    DateTime completedDate;
    int ratingA;  // Rating given by student A
    int ratingB;  // Rating given by student B
    bool isBidirectional;

public:
    Exchange();

    // Constructor for unidirectional exchange
    Exchange(int id, OfferedSkill* offered, RequestedSkill* requested,
        Str status, DateTime created);

    // Constructor for bidirectional exchange
    Exchange(int id,
        OfferedSkill* offeredA, OfferedSkill* offeredB,
        RequestedSkill* requestedA, RequestedSkill* requestedB,
        Str status, DateTime created);

    Exchange(const Exchange& other);
    ~Exchange();

    Exchange& operator=(const Exchange& other);

    // Getters
    int getExchangeId() const;
    OfferedSkill* getOfferedSkillA() const;
    OfferedSkill* getOfferedSkillB() const;
    RequestedSkill* getRequestedSkillA() const;
    RequestedSkill* getRequestedSkillB() const;
    Str getStatus() const;
    DateTime getCreatedDate() const;
    DateTime getCompletedDate() const;
    int getRatingA() const;
    int getRatingB() const;
    bool getIsBidirectional() const;

    // Setters
    void setStatus(Str newStatus);
    void setCompletedDate(DateTime date);
    void setRatingA(int score);
    void setRatingB(int score);
    void setIsBidirectional(bool bidirectional);

    // Operations
    bool initiateExchange();
    bool completeExchange();
    bool cancelExchange();

    // Helper methods
    bool involvesStudent(int studentId) const;

    // Debug
    void debugPrint() const;
};

#endif