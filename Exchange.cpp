#include "Exchange.h"
Exchange::Exchange()
    : exchangeId(0), offeredSkill(nullptr), requestedSkill(nullptr),
    status("pending"), createdDate(DateTime()), completedDate(DateTime()), rating(0) {
}
Exchange::Exchange(int id, OfferedSkill* offered, RequestedSkill* requested, Str status, DateTime created)
    : exchangeId(id), offeredSkill(offered), requestedSkill(requested),
    status(status), createdDate(created), completedDate(DateTime()), rating(0) {
}
Exchange::Exchange(const Exchange& other)
    : exchangeId(other.exchangeId), offeredSkill(other.offeredSkill),
    requestedSkill(other.requestedSkill), status(other.status),
    createdDate(other.createdDate), completedDate(other.completedDate),
    rating(other.rating) {
}
Exchange& Exchange::operator=(const Exchange& other) {
    if (this != &other) {
        exchangeId = other.exchangeId;
        offeredSkill = other.offeredSkill;
        requestedSkill = other.requestedSkill;
        status = other.status;
        createdDate = other.createdDate;
        completedDate = other.completedDate;
        rating = other.rating;
    }
    return *this;
}
Exchange::~Exchange() {
}
int Exchange::getId() const { return exchangeId; }
OfferedSkill* Exchange::getOfferedSkill() const { return offeredSkill; }
RequestedSkill* Exchange::getRequestedSkill() const { return requestedSkill; }
Str Exchange::getStatus() const { return status; }
DateTime Exchange::getCreatedDate() const { return createdDate; }
DateTime Exchange::getCompletedDate() const { return completedDate; }
int Exchange::getRating() const { return rating; }
void Exchange::setStatus(Str newStatus) {
    status = newStatus;
}

void Exchange::setCompletedDate(DateTime date) {
    completedDate = date;
}

void Exchange::setRating(int score) {
    if (score >= 1 && score <= 5)
        rating = score;
}
bool Exchange::initiateExchange() {
    if (status == "Pending" && offeredSkill->getName() == requestedSkill->getName()) {
        status = "In Progress";
        return true;
    }
    return false;
}


bool Exchange::completeExchange() {
    if (status == "in_progress") {
        status = "completed";
        completedDate = DateTime();
        return true;
    }
    return false;
}

bool Exchange::cancelExchange() {
    if (status == "pending" || status == "in_progress") {
        status = "cancelled";
        return true;
    }
    return false;
}
void Exchange::writeToBinary(std::ofstream& out) const {
    offeredSkill->saveToBinary(out); 
    requestedSkill->saveToBinary(out); 
    createdDate.writeToBinary(out); 
    completedDate.writeToBinary(out);
}

void Exchange::readFromBinary(std::ifstream& in) {
    offeredSkill = new OfferedSkill();
    offeredSkill->loadFromBinary(in);

    requestedSkill = new RequestedSkill();
    requestedSkill->loadFromBinary(in);

    createdDate.readFromBinary(in);
    completedDate.readFromBinary(in);
}
