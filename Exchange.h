#ifndef EXCHANGE_H
#define EXCHANGE_H

#include "OfferedSkills.h"
#include "RequestedSkills.h"
#include "DateTime.h"
#include "Str.h"
#include<iostream>
using namespace std;
class Exchange {
private:
    int exchangeId;
    OfferedSkill* offeredSkill;
    RequestedSkill* requestedSkill;
    Str status;
    DateTime createdDate;
    DateTime completedDate;
    int rating;

public:
    Exchange();
    Exchange(int id, OfferedSkill* offered, RequestedSkill* requested, Str status, DateTime created);
    Exchange(const Exchange& other);
    Exchange& operator=(const Exchange& other);
    ~Exchange();
    int getId() const;
    OfferedSkill* getOfferedSkill() const;
    RequestedSkill* getRequestedSkill() const;
    Str getStatus() const;
    DateTime getCreatedDate() const;
    DateTime getCompletedDate() const;
    int getRating() const;
    void setStatus(Str newStatus);
    void setCompletedDate(DateTime date);
    void setRating(int score);
    bool initiateExchange();
    bool completeExchange(); 
    bool cancelExchange(); 
    void writeToBinary(std::ofstream& out) const;
    void readFromBinary(std::ifstream& in);
    bool involves(const Str& email) const;

};

#endif
