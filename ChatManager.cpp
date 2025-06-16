#include "ChatManager.h"
#include "Message.h"
#include <iostream>
#include<fstream>
#include"AppException.h"
using namespace std;

ChatManager::ChatManager() {}

ChatManager::~ChatManager() {
    for (int i = 0; i < chatBoxes.size(); i++) {
        delete chatBoxes[i];
    }
}
Str ChatManager::getChatFilename(const Str& user1, const Str& user2) {
    Str email1 = user1;
    Str email2 = user2;
    if (email1 > email2) {
        Str temp = email1;
        email1 = email2;
        email2 = temp;
    }
    Str filename = "chat_";
    filename = filename.concat(email1);
    filename = filename.concat("_");
    filename = filename.concat(email2);
    filename = filename.concat(".bin");

    return filename;
}
ChatBox* ChatManager::loadOrCreateChat(const Str& user1, const Str& user2) {
    Str filename = getChatFilename(user1, user2);
    if (filename.length(filename.return_array()) < 10) {
        throw NetworkException("Invalid chat filename");
    }

    std::ifstream in(filename.return_array(), std::ios::binary);
    if (in.good()) {
        try {
            ChatBox* box = new ChatBox();
            box->readFromBinary(in);
            in.close();
            return box;
        }
        catch (...) {
            in.close();
            return new ChatBox(user1, user2);
        }
    }
    return new ChatBox(user1, user2);
}
void ChatManager::sendMessage(const Str& sender, const Str& receiver, const Str& content) {
    ChatBox* chat = loadOrCreateChat(sender, receiver);
    Message msg(sender, receiver, content);
    chat->addMessage(msg);
    Str filename1 = Str("chat_").concat(sender).concat("_").concat(receiver).concat(".bin");
    Str filename2 = Str("chat_").concat(receiver).concat("_").concat(sender).concat(".bin");
    ofstream out1(filename1.return_array(), ios::binary | ios::app);
    msg.writeToBinary(out1);
    out1.close();
    ofstream out2(filename2.return_array(), ios::binary | ios::app);
    msg.writeToBinary(out2);
    out2.close();
}


void ChatManager::showChat(const Str& user1, const Str& user2) const {
    for (int i = 0; i < chatBoxes.size(); i++) {
        if (chatBoxes[i]->involves(user1, user2)) {
            chatBoxes[i]->displayChat();
            return;
        }
    }
    cout << "No conversation found between " << user1 << " and " << user2 << ".\n";
}
void ChatManager::displayChatBetween(const Str& userA, const Str& userB) const {
    for (int i = 0; i < chatBoxes.size(); ++i) {
        if (chatBoxes[i]->involves(userA, userB)) {
            chatBoxes[i]->displayChat();
            return;
        }
    }
    std::cout << "No chat found between " << userA << " and " << userB << ".\n";
}
void ChatManager::saveChats(const char* filename) {
    ofstream out(filename, ios::binary);
    int chatCount = chatBoxes.size();
    out.write((char*)&chatCount, sizeof(chatCount));
    for (int i = 0; i < chatCount; ++i) {
        chatBoxes[i]->writeToBinary(out);
    }
    out.close();
}
void ChatManager::loadChats(const char* filename) {
    ifstream in(filename, ios::binary);
    if (!in) return;

    int chatCount;
    in.read((char*)&chatCount, sizeof(chatCount));
    for (int i = 0; i < chatCount; ++i) {
        ChatBox* box = new ChatBox();
        box->readFromBinary(in);
        chatBoxes.push(box);
    }
    in.close();
}
const Dynamic_array<ChatBox*>& ChatManager::getChatBoxes() const {
    return chatBoxes;
}
void ChatManager::listAllChatsForUser(const Str& user) const {
    std::cout << "Chat history for " << user << ":\n";
    for (int i = 0; i < chatBoxes.size(); ++i) {
        if (chatBoxes[i]->getUser1().is_equal(user) || chatBoxes[i]->getUser2().is_equal(user)) {
            std::cout << "- " << chatBoxes[i]->getUser1() << " <--> " << chatBoxes[i]->getUser2() << "\n";
        }
    }
}

