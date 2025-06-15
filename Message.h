#ifndef MESSAGE_H
#define MESSAGE_H

#include "Str.h"
#include "DateTime.h"
using namespace std;

class Message {
private:
    Str sender;
    Str receiver;
    Str content;
    DateTime timestamp;  // <-- Add this member to store time

public:
    // Default constructor
    Message();

    // Full constructor
    Message(const Str& sender, const Str& receiver, const Str& content, const DateTime& timestamp)
        : sender(sender), receiver(receiver), content(content), timestamp(timestamp) {
    }

    // Constructor without timestamp (uses current time)
    Message(const Str& sender, const Str& receiver, const Str& content);

    // Copy constructor
    Message(const Message& other);

    // Assignment operator
    Message& operator=(const Message& other);

    // Getters
    Str getSender() const;
    Str getReceiver() const;
    Str getContent() const;
    DateTime getTimestamp() const;
    void writeToBinary(ofstream& out) const;
    void readFromBinary(ifstream& in);
   void display() const;
};

#endif // MESSAGE_H
