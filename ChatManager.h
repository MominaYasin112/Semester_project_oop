// ChatManager.h
#ifndef CHATMANAGER_H
#define CHATMANAGER_H

#include "ChatBox.h"
#include "Str.h"
#include "Dynamic_array.h"

class ChatManager {
private:
    Dynamic_array<ChatBox*> chatBoxes;

public:
    ChatManager();
    ~ChatManager();

    ChatBox* loadOrCreateChat(const Str& u1, const Str& u2);
    void sendMessage(const Str& sender, const Str& receiver, const Str& content);
    void showChat(const Str& user1, const Str& user2) const;
    void displayChatBetween(const Str& userA, const Str& userB) const;
    void saveChats(const char* filename);
    void loadChats(const char* filename);
    const Dynamic_array<ChatBox*>& getChatBoxes() const;
    void listAllChatsForUser(const Str& user) const;
};

#endif
