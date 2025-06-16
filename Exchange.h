#ifndef EXCHANGE_H
#define EXCHANGE_H

#include "OfferedSkills.h"
#include "RequestedSkills.h"
#include "DateTime.h"
#include "Str.h"
#include <fstream>

class Exchange {
    int exchangeId;
    OfferedSkill* offeredSkill;
    RequestedSkill* requestedSkill;
    Str status;
    DateTime createdDate;
    DateTime completedDate;
    int rating;

public:
    Exchange();
    Exchange(int id, OfferedSkill* offered, RequestedSkill* requested,
        Str status, DateTime created);
    Exchange(const Exchange& other);
    ~Exchange();

    Exchange& operator=(const Exchange& other);

    // Getters
    int getExchangeId() const { return exchangeId; }
    OfferedSkill* getOfferedSkill() const { return offeredSkill; }
    RequestedSkill* getRequestedSkill() const { return requestedSkill; }
    Str getStatus() const { return status; }
    DateTime getCreatedDate() const { return createdDate; }
    DateTime getCompletedDate() const { return completedDate; }
    int getRating() const { return rating; }

    // Setters
    void setStatus(Str newStatus);
    void setCompletedDate(DateTime date);
    void setRating(int score);

    // Operations
    bool initiateExchange();
    bool completeExchange();
    bool cancelExchange();

    // Serialization
  /*  void writeToBinary(std::ofstream& out) const;
    void readFromBinary(std::ifstream& in);*/

    // Debug
 /*   void debugPrint() const;*/
};

#endif