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
Str ChatManager::sanitizeForFilename(Str email) {
    Str cleaned = email;
    cleaned.replace('@', '_');
    cleaned.replace('.', '-');
    return cleaned;
}

Str ChatManager::getChatFilename(const Str& user1, const Str& user2) {
    Str email1 = sanitizeForFilename(user1);
    Str email2 = sanitizeForFilename(user2);

    if (email1 > email2) {
        Str temp = email1;
        email1 = email2;
        email2 = temp;
    }

    return Str("chat_").concat(email1).concat("_").concat(email2).concat(".bin");
}
ChatBox* ChatManager::loadOrCreateChat(const Str& user1, const Str& user2) {
    Str filename = getChatFilename(user1, user2);
    std::cout << "Loading or creating chat file: " << filename.return_array() << std::endl;
    std::ifstream in(filename.return_array(), std::ios::binary);

    if (in.good()) {
        try {
            ChatBox* box = new ChatBox(user1, user2);
            box->readFromBinary(in);
            in.close();
            return box;
        }
        catch (...) {
            in.close();
            std::cerr << "Failed to load existing chat, creating new.\n";
        }
    }

    return new ChatBox(user1, user2);
}
void ChatManager::sendMessage(const Str& sender, const Str& receiver, const Str& content) {
    ChatBox* chat = loadOrCreateChat(sender, receiver);
    Message msg(sender, receiver, content);
    chat->addMessage(msg);

    Str filename = getChatFilename(sender, receiver);
    std::ofstream out(filename.return_array(), std::ios::binary); // overwrite!
    if (!out) {
        throw FileException("Failed to open chat file for writing");
    }

    chat->writeToBinary(out);
    out.close();
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
void ChatManager::displayChatUI(Student* currentUser) {
    if (!currentUser) return;

    Str userEmail = currentUser->getEmail();

    ImGui::Text("Your Chats:");
    listAllChatsForUser(userEmail);

    static char opponentEmail[100] = "";
    static char message[256] = "";

    ImGui::InputText("Opponent Email", opponentEmail, IM_ARRAYSIZE(opponentEmail));

    if (ImGui::Button("Load Chat")) {
        loadOrCreateChat(userEmail, Str(opponentEmail));
    }

    ImGui::InputText("Message", message, IM_ARRAYSIZE(message));
    if (ImGui::Button("Send")) {
        sendMessage(userEmail, Str(opponentEmail), Str(message));
        message[0] = '\0';
    }

    ImGui::Separator();
    displayChatBetween(userEmail, Str(opponentEmail));
}

