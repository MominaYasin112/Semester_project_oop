#include "Exchange.h"
#include <iostream>

Exchange::Exchange()
    : exchangeId(0), offeredSkillA(nullptr), offeredSkillB(nullptr),
    requestedSkillA(nullptr), requestedSkillB(nullptr), status("Pending"),
    ratingA(0), ratingB(0), isBidirectional(false) {
}

Exchange::Exchange(int id, OfferedSkill* offered, RequestedSkill* requested,
    Str status, DateTime created)
    : exchangeId(id), offeredSkillA(offered), offeredSkillB(nullptr),
    requestedSkillA(requested), requestedSkillB(nullptr), status(status),
    createdDate(created), ratingA(0), ratingB(0), isBidirectional(false) {
}

Exchange::Exchange(int id,
    OfferedSkill* offeredA, OfferedSkill* offeredB,
    RequestedSkill* requestedA, RequestedSkill* requestedB,
    Str status, DateTime created)
    : exchangeId(id), offeredSkillA(offeredA), offeredSkillB(offeredB),
    requestedSkillA(requestedA), requestedSkillB(requestedB), status(status),
    createdDate(created), ratingA(0), ratingB(0), isBidirectional(true) {
}

Exchange::Exchange(const Exchange& other) {
    exchangeId = other.exchangeId;
    offeredSkillA = other.offeredSkillA ? new OfferedSkill(*other.offeredSkillA) : nullptr;
    offeredSkillB = other.offeredSkillB ? new OfferedSkill(*other.offeredSkillB) : nullptr;
    requestedSkillA = other.requestedSkillA ? new RequestedSkill(*other.requestedSkillA) : nullptr;
    requestedSkillB = other.requestedSkillB ? new RequestedSkill(*other.requestedSkillB) : nullptr;
    status = other.status;
    createdDate = other.createdDate;
    completedDate = other.completedDate;
    ratingA = other.ratingA;
    ratingB = other.ratingB;
    isBidirectional = other.isBidirectional;
}

Exchange::~Exchange() {
    // No deletion here as we don't own the skill objects
    // They are owned by the Student objects
}

Exchange& Exchange::operator=(const Exchange& other) {
    if (this != &other) {
        exchangeId = other.exchangeId;
        offeredSkillA = other.offeredSkillA;
        offeredSkillB = other.offeredSkillB;
        requestedSkillA = other.requestedSkillA;
        requestedSkillB = other.requestedSkillB;
        status = other.status;
        createdDate = other.createdDate;
        completedDate = other.completedDate;
        ratingA = other.ratingA;
        ratingB = other.ratingB;
        isBidirectional = other.isBidirectional;
    }
    return *this;
}

// Getters implementation
int Exchange::getExchangeId() const { return exchangeId; }
OfferedSkill* Exchange::getOfferedSkillA() const { return offeredSkillA; }
OfferedSkill* Exchange::getOfferedSkillB() const { return offeredSkillB; }
RequestedSkill* Exchange::getRequestedSkillA() const { return requestedSkillA; }
RequestedSkill* Exchange::getRequestedSkillB() const { return requestedSkillB; }
Str Exchange::getStatus() const { return status; }
DateTime Exchange::getCreatedDate() const { return createdDate; }
DateTime Exchange::getCompletedDate() const { return completedDate; }
int Exchange::getRatingA() const { return ratingA; }
int Exchange::getRatingB() const { return ratingB; }
bool Exchange::getIsBidirectional() const { return isBidirectional; }

// Setters implementation
void Exchange::setStatus(Str newStatus) { status = newStatus; }
void Exchange::setCompletedDate(DateTime date) { completedDate = date; }
void Exchange::setRatingA(int score) { ratingA = score; }
void Exchange::setRatingB(int score) { ratingB = score; }
void Exchange::setIsBidirectional(bool bidirectional) { isBidirectional = bidirectional; }

// Operations implementation
bool Exchange::initiateExchange() {
    if (status == "Pending") {
        status = "Active";
        return true;
    }
    return false;
}

bool Exchange::completeExchange() {
    if (status == "Active") {
        status = "Completed";
        completedDate = DateTime::now();
        return true;
    }
    return false;
}

bool Exchange::cancelExchange() {
    if (status != "Completed") {
        status = "Cancelled";
        return true;
    }
    return false;
}

// Helper methods implementation
bool Exchange::involvesStudent(int studentId) const {
    return (offeredSkillA && offeredSkillA->getStudentId() == studentId) ||
        (offeredSkillB && offeredSkillB->getStudentId() == studentId) ||
        (requestedSkillA && requestedSkillA->getStudentId() == studentId) ||
        (requestedSkillB && requestedSkillB->getStudentId() == studentId);
}

// Debug implementation
void Exchange::debugPrint() const {
    std::cout << "Exchange ID: " << exchangeId << "\n";
    std::cout << "Status: " << status.return_array() << "\n";
    std::cout << "Created: " << createdDate.toString().return_array() << "\n";

    if (offeredSkillA) {
        std::cout << "Student A (" << offeredSkillA->getStudentId()
            << ") offers: " << offeredSkillA->getName().return_array() << "\n";
    }

    if (requestedSkillA) {
        std::cout << "Student A requests: " << requestedSkillA->getName().return_array() << "\n";
    }

    if (isBidirectional) {
        if (offeredSkillB) {
            std::cout << "Student B (" << offeredSkillB->getStudentId()
                << ") offers: " << offeredSkillB->getName().return_array() << "\n";
        }

        if (requestedSkillB) {
            std::cout << "Student B requests: " << requestedSkillB->getName().return_array() << "\n";
        }
    }

    if (completedDate.isValid()) {
        std::cout << "Completed: " << completedDate.toString().return_array() << "\n";
    }

    std::cout << "Ratings: " << ratingA << " (A), " << ratingB << " (B)\n";
}