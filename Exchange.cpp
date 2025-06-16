#include "Exchange.h"
#include <iostream>

Exchange::Exchange()
    : exchangeId(0), offeredSkill(nullptr), requestedSkill(nullptr),
    status("Pending"), createdDate(), completedDate(), rating(0) {
}

Exchange::Exchange(int id, OfferedSkill* offered, RequestedSkill* requested,
    Str status, DateTime created)
    : exchangeId(id), offeredSkill(offered), requestedSkill(requested),
    status(status), createdDate(created), completedDate(), rating(0) {
}

Exchange::Exchange(const Exchange& other)
    : exchangeId(other.exchangeId),
    offeredSkill(new OfferedSkill(*other.offeredSkill)),
    requestedSkill(new RequestedSkill(*other.requestedSkill)),
    status(other.status),
    createdDate(other.createdDate),
    completedDate(other.completedDate),
    rating(other.rating) {
}

Exchange::~Exchange() {
    delete offeredSkill;
    delete requestedSkill;
}

Exchange& Exchange::operator=(const Exchange& other) {
    if (this != &other) {
        exchangeId = other.exchangeId;

        delete offeredSkill;
        offeredSkill = new OfferedSkill(*other.offeredSkill);

        delete requestedSkill;
        requestedSkill = new RequestedSkill(*other.requestedSkill);

        status = other.status;
        createdDate = other.createdDate;
        completedDate = other.completedDate;
        rating = other.rating;
    }
    return *this;
}

void Exchange::setStatus(Str newStatus) {
    status = newStatus;
}

void Exchange::setCompletedDate(DateTime date) {
    completedDate = date;
}

void Exchange::setRating(int score) {
    if (score >= 1 && score <= 5) {
        rating = score;
    }
}

bool Exchange::initiateExchange() {
    if (status == "Pending") {
        status = "In Progress";
        return true;
    }
    return false;
}

bool Exchange::completeExchange() {
    if (status == "In Progress") {
        status = "Completed";
        completedDate = DateTime::now();
        return true;
    }
    return false;
}

bool Exchange::cancelExchange() {
    if (status == "Pending" || status == "In Progress") {
        status = "Cancelled";
        return true;
    }
    return false;
}

//void Exchange::writeToBinary(std::ofstream& out) const {
//    // Write primitive types
//    out.write(reinterpret_cast<const char*>(&exchangeId), sizeof(exchangeId));
//
//    // Write skills
//    if (offeredSkill) offeredSkill->saveToBinary(out);
//    if (requestedSkill) requestedSkill->saveToBinary(out);
//
//    // Write status
//    int statusLen = status.length(status.return_array());
//    out.write(reinterpret_cast<const char*>(&statusLen), sizeof(statusLen));
//    out.write(status.return_array(), statusLen);
//
//    // Write dates
//    createdDate.writeToBinary(out);
//    completedDate.writeToBinary(out);
//
//    // Write rating
//    out.write(reinterpret_cast<const char*>(&rating), sizeof(rating));
//}
//
//void Exchange::readFromBinary(std::ifstream& in) {
//    // Read primitive types
//    in.read(reinterpret_cast<char*>(&exchangeId), sizeof(exchangeId));
//
//    // Read skills
//    delete offeredSkill;
//    offeredSkill = new OfferedSkill();
//    offeredSkill->loadFromBinary(in);
//
//    delete requestedSkill;
//    requestedSkill = new RequestedSkill();
//    requestedSkill->loadFromBinary(in);
//
//    // Read status
//    int statusLen;
//    in.read(reinterpret_cast<char*>(&statusLen), sizeof(statusLen));
//    char* statusBuf = new char[statusLen + 1];
//    in.read(statusBuf, statusLen);
//    statusBuf[statusLen] = '\0';
//    status = Str(statusBuf);
//    delete[] statusBuf;
//
//    // Read dates
//    createdDate.readFromBinary(in);
//    completedDate.readFromBinary(in);
//
//    // Read rating
//    in.read(reinterpret_cast<char*>(&rating), sizeof(rating));
//}
//
//void Exchange::debugPrint() const {
//    std::cout << "=== Exchange Debug ===\n";
//    std::cout << "ID: " << exchangeId << "\n";
//    std::cout << "Status: " << status.return_array() << "\n";
//    std::cout << "Created: ";
//    createdDate.dis();
//    std::cout << "\n";
//
//    if (offeredSkill) {
//        std::cout << "Offered Skill: " << offeredSkill->getName().return_array() << "\n";
//    }
//    else {
//        std::cout << "Offered Skill: NULL\n";
//    }
//
//    if (requestedSkill) {
//        std::cout << "Requested Skill: " << requestedSkill->getName().return_array() << "\n";
//    }
//    else {
//        std::cout << "Requested Skill: NULL\n";
//    }
//
//    std::cout << "=====================\n";
//}