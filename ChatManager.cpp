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

ChatBox* ChatManager::loadOrCreateChat(const Str& u1, const Str& u2) {
    // Check existing chats first
    for (auto* chat : chatBoxes) {
        if (chat->involves(u1, u2)) {
            return chat;
        }
    }
    ChatBox* newBox = new ChatBox(u1, u2);
    Str filename1 = Str("chat_").concat(u1).concat("_").concat(u2).concat(".bin");
    Str filename2 = Str("chat_").concat(u2).concat("_").concat(u1).concat(".bin");

    ifstream in;
    in.open(filename1.return_array(), ios::binary);
    if (!in) {
        in.open(filename2.return_array(), ios::binary);
    }

    if (in) {
        try {
            newBox->readFromBinary(in);
        }
        catch (const std::exception& e) {
            std::cerr << "Warning: Failed to load chat - " << e.what() << "\n";
        }
        in.close();
    }
    chatBoxes.push(newBox);
    return newBox;
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

