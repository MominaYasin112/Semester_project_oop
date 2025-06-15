// ChatBox.h
#ifndef CHATBOX_H
#define CHATBOX_H

#include "Message.h"
#include "Str.h"
#include "Dynamic_array.h"
#include<iostream>
using namespace std;
class ChatBox {
private:
    Str user1;
    Str user2;
    Dynamic_array<Message> messages;

public:
    ChatBox();
    ChatBox(const Str& u1, const Str& u2);
    const Dynamic_array<Message>& getMessages() const;
    bool involves(const Str& userA, const Str& userB) const;
    void addMessage(const Message& msg);
    void displayChat() const;
    Str getUser1() const;
    Str getUser2() const;
    void writeToBinary(ofstream& out) const;
    void readFromBinary(ifstream& in);
};

#endif // CHATBOX_H
