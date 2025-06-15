#include "ChatBox.h"
#include <iostream>
using namespace std;
ChatBox::ChatBox() {}

ChatBox::ChatBox(const Str& u1, const Str& u2) : user1(u1), user2(u2) {
}

bool ChatBox::involves(const Str& userA, const Str& userB) const {
    return (user1.is_equal(userA) && user2.is_equal(userB) || (user1.is_equal(userB) && user2.is_equal(userA)));
}


void ChatBox::addMessage(const Message& msg) {
    messages.push(msg);
}

void ChatBox::displayChat() const {
    std::cout << "Chat between " << user1 << " and " << user2 << ":\n";
    for (int i = 0; i < messages.size(); i++) {
        messages[i].display();
    }
}

Str ChatBox::getUser1() const {
    return user1;
}

Str ChatBox::getUser2() const {
    return user2;
}
void ChatBox::writeToBinary(std::ofstream& out) const {
    user1.writeToBinary(out);
    user2.writeToBinary(out);
    int count = messages.size();
    out.write(reinterpret_cast<const char*>(&count), sizeof(count));
    for (int i = 0; i < count; ++i) {
        messages[i].writeToBinary(out);
    }
}
void ChatBox::readFromBinary(std::ifstream& in) {
    user1.readFromBinary(in);
    user2.readFromBinary(in);

    int count;
    in.read(reinterpret_cast<char*>(&count), sizeof(count));
    if (count < 0 || count > 10000) { 
        throw std::runtime_error("Invalid message count in chat file");
    }

    messages.clear();
    for (int i = 0; i < count && in; ++i) {
        Message msg;
        msg.readFromBinary(in);
        if (in) {
            messages.push(msg);
        }
    }
}
const Dynamic_array<Message>& ChatBox::getMessages() const {
    return messages;
}
