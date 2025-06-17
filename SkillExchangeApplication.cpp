#include "SkillExchangeApplication.h"
#include "SkillRepository.h"
#include "AppException.h"
SkillRepository skillRepo;
#include<iostream>
using namespace std;
#include <filesystem> 
#include"UIComponent.h"
#include "HungarianMatcher.h"
namespace fs = filesystem; 
SkillExchangeApplication::SkillExchangeApplication()
    : authManager(AuthenticationManager::getInstance()),
    loginWindow(1),
    dashboard(2),
    chatBoxWidget(3),
    offeredDropdown(4, skillRepo.getAllSkillNames()),
    requestedDropdown(5, skillRepo.getAllSkillNames()),
    timeSlotPicker(6),
    deadlinePicker(7),
    registerButton(8, "Register", [this]() { handleRegistration(); }),
    engine(new HungarianMatcher(), &exchangeManager),
    isChatCreating(false)
{
    authManager->loadFromFile("students.bin");
    loginWindow.setLoginCallback([this]() { handleLogin(); });
    dashboard.setOnChatClick([this]() { showChatBoxWindow = true; });
    dashboard.setOnAddClick([this]() {
        isRegistering = true;
        offeredDropdown.reset();
        requestedDropdown.reset();
        timeSlotPicker.reset();
        });
    dashboard.setOnStartNewChatClick([this]() { isChatCreating = true; });
    dashboard.setOnExitClick([]() { exit(0); });
    /*dashboard.setOnMatchClick([this]() {
        this->showMatchResults();
        });*/
    loginWindow.show();
   /* exchangeManager.loadFromFile("exchanges.bin");*/
    lastExchangeMessage = "";
    messageDisplayTime = 0.0f;
    showMessage = false;
}
void SkillExchangeApplication::render() {
    static Str errorMessage;
    static bool showErrorPopup = false;
    if (showErrorPopup) {
        ImGui::OpenPopup("Error");
        if (ImGui::BeginPopupModal("Error", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::Text("%s", errorMessage.return_array());
            if (ImGui::Button("OK")) {
                showErrorPopup = false;
                ImGui::CloseCurrentPopup();
                errorMessage.clear();
            }
            ImGui::EndPopup();
        }
    }
    if (isRegistering) {
        ImGui::Begin("Register Skills", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

        static char emailBuf[100] = "";
        static char passBuf[100] = "";

        ImGui::InputText("Email", emailBuf, sizeof(emailBuf));
        ImGui::InputText("Password", passBuf, sizeof(passBuf), ImGuiInputTextFlags_Password);

        ImGui::Text("Select Offered Skill:");
        offeredDropdown.render();

        ImGui::Text("Select Requested Skill:");
        requestedDropdown.render();

        ImGui::Text("Select Available Time Slots:");
        timeSlotPicker.render();

        ImGui::Text("Requested skill deadline is fixed to 14 days from today.");

        if (ImGui::Button("Register")) {
            try {

                loginWindow.setEmail(emailBuf);
                loginWindow.setPassword(passBuf);
                handleRegistration();
                isRegistering = false;
                loginWindow.show();

            }
            catch (const AppException& e) {
                errorMessage = e.getMessage();
                showErrorPopup = true;
            }
        }
        if (ImGui::Button("EMERGENCY DEBUG")) {
            std::cout << "\n==== MANUAL DEBUG ====\n";
            std::cout << "ExchangeManager has "
                << exchangeManager.getAllExchanges().size()
                << " exchanges\n";
        }
        if (ImGui::Button("Cancel")) {
            isRegistering = false;
            loginWindow.show();
        }

        ImGui::End();
    }
    else if (loginWindow.visible()) {
        loginWindow.render();

        ImGui::Begin("New User?", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
        if (ImGui::Button("Don't have an account? Register here")) {
            isRegistering = true;
            loginWindow.hide();
        }
        ImGui::End();
    }
    else {
        if (currentStudent) {
            dashboard.setReputation(currentStudent->getReputationPoints());
        }

        dashboard.render();
        static bool matchProcessed = false;
        if (dashboard.isMatchButtonPressed() && !matchProcessed) {
            std::cout << "Match button pressed - calling showMatchResults()" << std::endl;
            showMatchResults();
            matchProcessed = true;
        }
        if (lastExchangeMessage.length(lastExchangeMessage.return_array()) > 0) {
            ImGui::Text("%s", lastExchangeMessage.return_array());
        }
        if (matchProcessed) {
            matchProcessed = false;
        }
        if (chatBoxWidget.visible() && chatBoxWidget.getChatBox() != nullptr) {
            chatBoxWidget.render();

            static char msgBuffer[256] = "";
            ImGui::InputText("Type message...", msgBuffer, sizeof(msgBuffer));

            if (ImGui::Button("Send")) {
                try {
                    if (!currentStudent) throw AuthException("Not logged in");

                    Str sender = currentStudent->getEmail();
                    Str receiver = chatBoxWidget.getOpponentEmail();
                    Str message(msgBuffer);

                    if (message.empty()) throw NetworkException("Message empty");

                    chatManager.sendMessage(sender, receiver, message);
                    chatBoxWidget.receiveMessage(Str("You: ").concat(message));
                    msgBuffer[0] = '\0';
                }
                catch (const AppException& e) {
                    errorMessage = e.getMessage();
                    showErrorPopup = true;
                }
            }
        }
        if (isChatCreating && !chatPopupOpened) {
            ImGui::OpenPopup("Start New Chat");
            chatPopupOpened = true;
        }
        if (isChatCreating) {
            renderStartChatPopup();
        }
        if (ImGui::Button("Open ChatBox")) {
            showChatBoxWindow = true;
        }
        if (showChatBoxWindow) {
            ImGui::Begin("Previous Chats", &showChatBoxWindow, ImGuiWindowFlags_AlwaysAutoResize);
            openPreviousChatBox();
            ImGui::End();
        }

        if (debugMode) {
            ImGui::Begin("Debug Info");
            ImGui::Text("Students registered: %d", authManager->getAllRegisteredStudents().size());
            if (currentStudent) {
                ImGui::Text("Current user skills: %d offered, %d requested",
                    currentStudent->getOfferedSkills().size(),
                    currentStudent->getRequestedSkills().size());
            }
            ImGui::End();
        }
    }
}
void SkillExchangeApplication::handleRegistration() {
    Str email = loginWindow.getEmail();
    Str pass = loginWindow.getPassword();

    if (!authManager->isEmailAvailable(email)) {
        loginWindow.showErrorMessage("Email already in use.");
        return;
    }

    try {
        Str offeredName = offeredDropdown.getSelectedSkill();
        Str requestedName = requestedDropdown.getSelectedSkill();

        if (offeredName.empty() || requestedName.empty()) {
            loginWindow.showErrorMessage("Please select both skills.");
            return;
        }

        const Skill& offeredSkill = skillRepo.findSkillByName(offeredName);
        const Skill& requestedSkill = skillRepo.findSkillByName(requestedName);

        Dynamic_array<TimeSlot> slots = timeSlotPicker.getSelectedTimeSlots();
        if (slots.size() == 0) {
            loginWindow.showErrorMessage("Select at least one time slot.");
            return;
        }

        DateTime today = DateTime::now();
        DateTime deadline = today.addDays(14);
        currentStudent = authManager->registerUser(email, pass);
        OfferedSkill* offered = new OfferedSkill(
            offeredSkill.getSkillId(), offeredSkill.getName(),
            offeredSkill.getCategory(), offeredSkill.getDescription(),
            slots, currentStudent->getUserId()
        );

        RequestedSkill* requested = new RequestedSkill(
            requestedSkill.getSkillId(), requestedSkill.getName(),
            requestedSkill.getCategory(), requestedSkill.getDescription(),
            deadline, "Pending", currentStudent->getUserId()
        );

        offered->setStudentId(currentStudent->getUserId());
        requested->setStudentId(currentStudent->getUserId());
        currentStudent->addRequestedSkill(requested);
        currentStudent->addOfferedSkill(offered);
        authManager->saveToFile("students.bin");
        offeredDropdown.reset();
        requestedDropdown.reset();
        timeSlotPicker.reset();

        isRegistering = false;
        loginWindow.show();
        loginWindow.showSuccessMessage("Registration successful! Please log in.");
    }
    catch (const AppException& ex) {
        loginWindow.showErrorMessage(ex.getMessage());
    }
}
void SkillExchangeApplication::handleLogin() {
    if (!loginWindow.validateInput()) return;

    Str email = loginWindow.getEmail();
    Str pass = loginWindow.getPassword();

    try {
        Student* s = authManager->verifyLogin(email, pass);
        if (s) {
            currentStudent = s;
            loginWindow.hide();
            showDashboard();
        }
        else {
            loginWindow.showErrorMessage("Login failed.");
        }
    }
    catch (const AuthException& ex) {
        loginWindow.showErrorMessage(ex.getMessage());
    }
}

void SkillExchangeApplication::showMatchResults() {
    std::cout << "\n==== DEBUG: MATCHING FLOW START ====\n";

    Dynamic_array<Student*>& allStudents = authManager->getAllRegisteredStudents();
    std::cout << "Total registered students: " << allStudents.size() << "\n";

    for (int i = 0; i < allStudents.size(); ++i) {
        std::cout << "Student " << i << ": " << allStudents[i]->getEmail().return_array() << "\n";
        std::cout << "  Offered skills: ";
        for (auto& skill : allStudents[i]->getOfferedSkills()) {
            std::cout << skill->getName().return_array() << " ";
        }
        std::cout << "\n  Requested skills: ";
        for (auto& skill : allStudents[i]->getRequestedSkills()) {
            std::cout << skill->getName().return_array() << " ";
        }
        std::cout << "\n";
    }

    Dynamic_array<Match*> matches = engine.matchStudents(allStudents);
    std::cout << "Total matches found: " << matches.size() << "\n";

    for (int i = 0; i < matches.size(); ++i) {
        std::cout << "Match " << i << ": "
            << matches[i]->getRequester()->getEmail().return_array() << " <-> "
            << matches[i]->getProvider()->getEmail().return_array()
            << " | A wants: " << matches[i]->getSkillA().return_array()
            << " | B wants: " << matches[i]->getSkillB().return_array()
            << " | Score: " << matches[i]->getScore() << "\n";
    }

    if (!matches.isEmpty()) {
        Match* selectedMatch = matches[0];
        std::cout << "Selected Match: "
            << selectedMatch->getRequester()->getEmail().return_array() << " <-> "
            << selectedMatch->getProvider()->getEmail().return_array()
            << " (Skills: " << selectedMatch->getSkillA().return_array() << " ↔ "
            << selectedMatch->getSkillB().return_array() << ")\n";
        OfferedSkill* offeredA = nullptr;
        OfferedSkill* offeredB = nullptr;
        RequestedSkill* requestedA = nullptr;
        RequestedSkill* requestedB = nullptr;
        for (auto skill : selectedMatch->getProvider()->getOfferedSkills()) {
            if (skill->getName() == selectedMatch->getSkillA()) {
                offeredB = skill;
                break;
            }
        }
        for (auto skill : selectedMatch->getRequester()->getRequestedSkills()) {
            if (skill->getName() == selectedMatch->getSkillA()) {
                requestedA = skill;
                break;
            }
        }
        for (auto skill : selectedMatch->getRequester()->getOfferedSkills()) {
            if (skill->getName() == selectedMatch->getSkillB()) {
                offeredA = skill;
                break;
            }
        }
        for (auto skill : selectedMatch->getProvider()->getRequestedSkills()) {
            if (skill->getName() == selectedMatch->getSkillB()) {
                requestedB = skill;
                break;
            }
        }

        if (offeredA && offeredB && requestedA && requestedB) {
            Exchange* exchange = new Exchange(
                exchangeManager.getNextExchangeId(),
                offeredA, offeredB,
                requestedA, requestedB,
                "Pending",
                DateTime::now()
            );
            exchange->setIsBidirectional(true);
            exchangeManager.addExchange(exchange);

            Str emailA = selectedMatch->getRequester()->getEmail();
            Str emailB = selectedMatch->getProvider()->getEmail();
            Str skillA = selectedMatch->getSkillA();
            Str skillB = selectedMatch->getSkillB();

            lastExchangeMessage = "Exchange created between \n";
            lastExchangeMessage = lastExchangeMessage + emailA + " (wants: " + skillA + ", offers: " + skillB + ") and \n";
            lastExchangeMessage = lastExchangeMessage + emailB + " (wants: " + skillB + ", offers: " + skillA + "). \n";
            lastExchangeMessage = lastExchangeMessage + "Good luck!";

            showMessage = true;
            messageDisplayTime = ImGui::GetTime();


        }
        else {
            lastExchangeMessage = "❌ Exchange creation failed.";
            showMessage = true;
            messageDisplayTime = ImGui::GetTime();


        }
    }
    else {
        std::cout << "⚠️ No valid matches to form an exchange.\n";
        lastExchangeMessage = "❌ Exchange creation failed.";
        showMessage = true;
        messageDisplayTime = ImGui::GetTime();

    }

    std::cout << "==== DEBUG: MATCHING FLOW END ====\n";
}


void SkillExchangeApplication::showDashboard() {
    dashboard.show();
    chatBoxWidget.hide();
}
void SkillExchangeApplication::openChatBox() {
    static Str selectedOpponent;
    static int selectedIndex = 0;
    static Dynamic_array<Str> previousChatPartners;
    static bool chatSelected = false;

    previousChatPartners.clear();

    Str currentEmail = currentStudent->getEmail();

    for (const auto& entry : fs::directory_iterator(".")) {
        Str filename(entry.path().filename().string().c_str());

        // Manual check: starts with "chat_"
        if (filename.length(filename.return_array()) < 10) continue;

        bool startsWithChat = true;
        const char* fname = filename.return_array();
        const char prefix[] = "chat_";
        for (int i = 0; i < 5; ++i) {
            if (fname[i] != prefix[i]) {
                startsWithChat = false;
                break;
            }
        }
        if (!startsWithChat) continue;
        int len = filename.length(fname);
        if (!(fname[len - 4] == '.' && fname[len - 3] == 'b' && fname[len - 2] == 'i' && fname[len - 1] == 'n'))
            continue;
        int middleLen = len - 9;
        char middle[200] = {};
        for (int i = 0; i < middleLen; ++i) {
            middle[i] = fname[5 + i];
        }
        middle[middleLen] = '\0';
        Str middleStr(middle);
        int partCount = 0;
        Str* users = middleStr.split('_', partCount);
        if (partCount == 2) {
            if (users[0] == currentEmail)
                previousChatPartners.push(users[1]);
            else if (users[1] == currentEmail)
                previousChatPartners.push(users[0]);
        }
        delete[] users;
    }
    ImGui::Begin("Select Chatbox", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

    if (previousChatPartners.size() == 0) {
        ImGui::Text("You have no previous chats.");
    }
    else {
        ImGui::Text("Select a user to open chat with:");

        const char* labels[100];
        for (int i = 0; i < previousChatPartners.size(); ++i) {
            labels[i] = previousChatPartners[i].return_array();
        }

        ImGui::ListBox("Chat With", &selectedIndex, labels, previousChatPartners.size());

        if (ImGui::Button("Open Chat")) {
            selectedOpponent = previousChatPartners[selectedIndex];

            try {
                ChatBox* box = chatManager.loadOrCreateChat(currentEmail, selectedOpponent);
                chatBoxWidget = ChatBoxWidget(3);
                chatBoxWidget.setOpponentEmail(selectedOpponent);
                chatBoxWidget.onSendCallback = [&]() {
                    Str sender = currentStudent ? currentStudent->getEmail() : Str("");
                    Str receiver = chatBoxWidget.getOpponentEmail();
                    if (sender.empty() || receiver.empty()) return;
                    chatBoxWidget.sendMessage(sender, receiver, chatManager);
                    };
                for (int i = 0; i < box->getMessages().size(); ++i) {
                    const Message& msg = box->getMessages()[i];
                    Str prefix = (msg.getSender().is_equal(currentEmail)) ? Str("You: ") : Str("Them: ");
                    chatBoxWidget.receiveMessage(prefix.concat(msg.getContent()));
                }
                chatBoxWidget.show();

                chatBoxWidget.show();
                chatSelected = true;
            }
            catch (...) {
                ImGui::Text("Error: Could not load chat.");
            }
        }
    }
    ImGui::Separator();
    ImGui::Text("Start New Chat (by email):");

    static char emailInput[100] = "";
    ImGui::InputText("Email", emailInput, sizeof(emailInput));

    if (ImGui::Button("Start Chat")) {
        selectedOpponent = Str(emailInput);

        try {
            ChatBox* box = chatManager.loadOrCreateChat(currentEmail, selectedOpponent);
            chatBoxWidget = ChatBoxWidget(3);
            chatBoxWidget.onSendCallback = [&]() {
                Str sender = currentStudent ? currentStudent->getEmail() : Str("");
                Str receiver = chatBoxWidget.getOpponentEmail();

                if (sender.empty() || receiver.empty()) {
                    std::cout << " Cannot send message: sender or receiver is empty.\n";
                    return;
                }

                chatBoxWidget.sendMessage(sender, receiver, chatManager);
                };

            for (int i = 0; i < box->getMessages().size(); ++i) {
                chatBoxWidget.receiveMessage(box->getMessages()[i].getContent());
            }

            chatBoxWidget.show();
            chatSelected = true;
        }
        catch (...) {
            ImGui::Text("Error: Could not load chat.");
        }
    }
    ImGui::End();
    if (chatSelected && chatBoxWidget.visible()) {
        chatBoxWidget.render();
    }
}
void SkillExchangeApplication::renderStartChatPopup() {
    if (ImGui::BeginPopupModal("Start New Chat", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        static char emailBuffer[256] = "";
        ImGui::InputText("Opponent Email", emailBuffer, sizeof(emailBuffer));

        if (ImGui::Button("Start")) {
            if (currentStudent) {
                Str sender = currentStudent->getEmail();
                Str receiver(emailBuffer);

                ChatBox* chat = chatManager.loadOrCreateChat(sender, receiver);

                chatBoxWidget = ChatBoxWidget(3);
                chatBoxWidget.setChatBox(chat);
                chatBoxWidget.show();

                isChatCreating = false;
                chatPopupOpened = false;
                emailBuffer[0] = '\0'; 
                ImGui::CloseCurrentPopup();
            }
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel")) {
            isChatCreating = false;
            chatPopupOpened = false;
            emailBuffer[0] = '\0';
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}
void SkillExchangeApplication::openPreviousChatBox() {
    if (!currentStudent) return;

    Str currentEmail = currentStudent->getEmail();
    Dynamic_array<Str> chatPartners;

    for (const auto& entry : fs::directory_iterator(".")) {
        Str filename(entry.path().filename().string().c_str());
        if (!filename.contains("chat_") || !filename.contains(".bin")) continue;

        int prefixLen = 5;
        int suffixLen = 4;
        int totalLen = filename.length(filename.return_array());
        if (totalLen <= prefixLen + suffixLen) continue;

        Str middle = filename.substring(prefixLen, totalLen - (prefixLen + suffixLen));
        int partCount = 0;
        Str* parts = middle.split('_', partCount);

        if (partCount == 2) {
            Str user1 = parts[0].trim(), user2 = parts[1].trim();
            if (user1 == currentEmail) chatPartners.push(user2);
            else if (user2 == currentEmail) chatPartners.push(user1);
        }
        delete[] parts;
    }

    static int selected = -1;

    if (chatPartners.isEmpty()) {
        ImGui::Text("No previous chats found.");
    }
    else {
        for (int i = 0; i < chatPartners.size(); i++) {
            if (ImGui::Selectable(chatPartners[i].return_array(), selected == i)) {
                selected = i;
            }
        }

        if (selected >= 0 && ImGui::Button("Open Chat")) {
            try {
                ChatBox* chat = chatManager.loadOrCreateChat(currentEmail, chatPartners[selected]);

                chatBoxWidget = ChatBoxWidget(3);
                chatBoxWidget.setChatBox(chat);
                chatBoxWidget.setOpponentEmail(chatPartners[selected]);
                chatBoxWidget.setChatBox(chat);
                chatBoxWidget.loadPreviousMessages(chatManager, currentEmail, chatPartners[selected]);
                chatBoxWidget.onSendCallback = [this]() {
                    chatBoxWidget.sendMessage(currentStudent->getEmail(), chatBoxWidget.getOpponentEmail(), chatManager);
                    };
                chatBoxWidget.show();
                for (int i = 0; i < chat->getMessages().size(); ++i) {
                    const Message& m = chat->getMessages()[i];
                    Str prefix = m.getSender().is_equal(currentEmail) ? Str("You: ") : Str("Them: ");
                    chatBoxWidget.receiveMessage(prefix.concat(m.getContent()));
                }
                chatBoxWidget.show();
                selected = -1;
            }
            catch (...) {
                std::cerr << "Failed to load chat.\n";
            }
        }
    }
}


void SkillExchangeApplication::handleMessageSending() {
    Str sender = currentStudent->getEmail();
    Str receiver = chatBoxWidget.getOpponentEmail();
    Str message = chatBoxWidget.getInputText();

    if (!message.empty()) {
        chatManager.sendMessage(sender, receiver, message);
        chatBoxWidget.receiveMessage(Str("You: ").concat(message));
        chatBoxWidget.clearInput();
    }
}
