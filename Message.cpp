#include "Message.h"
#include"AppException.h"
using namespace std;
Message::Message() : sender(""), receiver(""), content(""), timestamp(DateTime::now()) {}

Message::Message(const Str& s, const Str& r, const Str& c) {
    if (s.empty() || r.empty()) {
        std::cout << "Sender: " << sender.return_array() << ", Receiver: " << receiver.return_array() << std::endl;
        throw NetworkException("Message requires both sender and receiver");
    }
    sender = s;
    receiver = r;
    content = c;
    timestamp = DateTime::now();
}
Message::Message(const Message& other)
    : sender(other.sender), receiver(other.receiver), content(other.content), timestamp(other.timestamp) {
}

Message& Message::operator=(const Message& other) {
    if (this != &other) {
        sender = other.sender;
        receiver = other.receiver;
        content = other.content;
        timestamp = other.timestamp;
    }
    return *this;
}

Str Message::getSender() const { return sender; }
Str Message::getReceiver() const { return receiver; }
Str Message::getContent() const { return content; }
DateTime Message::getTimestamp() const { return timestamp; }

void Message::display() const {
    std::cout << "----------------------------------------\n";
    std::cout << " Message from: " << sender << "\n";
    std::cout << " Sent at: ";
    timestamp.dis();
    cout<< "\n";
    std::cout << " Content: " << content << "\n";
    std::cout << "----------------------------------------\n";
}
void Message::writeToBinary(ofstream& out) const {
    sender.writeToBinary(out);
    receiver.writeToBinary(out);
    content.writeToBinary(out);
    timestamp.writeToBinary(out);
}

void Message::readFromBinary(ifstream& in) {
    sender.readFromBinary(in);
    receiver.readFromBinary(in);
    content.readFromBinary(in);
    timestamp.readFromBinary(in);
}
