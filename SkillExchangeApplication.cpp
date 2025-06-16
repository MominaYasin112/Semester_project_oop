#include "SkillExchangeApplication.h"
#include "SkillRepository.h"
#include "AppException.h"
SkillRepository skillRepo;
#include<iostream>
using namespace std;
#include <filesystem> 
#include"UIComponent.h"
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
    engine(&matcher),
    isChatCreating(false)
{
    authManager->loadFromFile("students.bin");
    loginWindow.setLoginCallback([this]() { handleLogin(); });
    dashboard.setOnMatchClick([this]() { handleSkillMatching(); });
    dashboard.setOnChatClick([this]() { openPreviousChatBox(); });
    dashboard.setOnAddClick([this]() {
        isRegistering = true;
        offeredDropdown.reset();
        requestedDropdown.reset();
        timeSlotPicker.reset();
        });
    dashboard.setOnStartNewChatClick([this]() { isChatCreating = true; });
    dashboard.setOnExitClick([]() { exit(0); });
    dashboard.setOnMatchClick([this]() {
        this->handleSkillMatching();
        });
    loginWindow.show();
    exchangeManager.loadFromFile("exchanges.bin");

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
        if (currentStudent)
            dashboard.setReputation(currentStudent->getReputationPoints());

        dashboard.render();
        if (dashboard.isChatButtonPressed()) {
            openPreviousChatBox();
        }
        if (chatBoxWidget.visible()) {
            chatBoxWidget.render();
            ImGui::Separator();

            static char msgBuffer[256] = "";
            ImGui::InputText("Type message", msgBuffer, sizeof(msgBuffer));

            if (ImGui::Button("Send")) {
                try {
                    if (!currentStudent) {
                        throw AuthException(Str("You must be logged in to send messages"));
                    }

                    Str sender = currentStudent->getEmail();
                    Str receiver = chatBoxWidget.getOpponentEmail();

                    if (sender.empty() || receiver.empty()) {
                        throw NetworkException(Str("Cannot send message - missing sender or receiver"));
                    }

                    if (strlen(msgBuffer) == 0) {
                        throw NetworkException(Str("Message cannot be empty"));
                    }

                    // Set the callback if not already set
                    if (!chatBoxWidget.onSendCallback) {
                        chatBoxWidget.onSendCallback = [this, sender, receiver]() {
                            Str message = chatBoxWidget.getInputText();
                            if (!message.empty()) {
                                chatManager.sendMessage(sender, receiver, message);
                                chatBoxWidget.receiveMessage(message);
                                chatBoxWidget.clearInput();
                            }
                            };
                    }
                    if (chatBoxWidget.onSendCallback) {
                        chatBoxWidget.onSendCallback();
                        msgBuffer[0] = '\0';
                    }
                }
                catch (const AppException& e) {
                    errorMessage = e.getMessage();
                    showErrorPopup = true;
                }
            }
        }

        if (isChatCreating) {
            renderStartChatPopup();
        }
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

void SkillExchangeApplication::handleSkillMatching() {
    std::cout << "[DEBUG] handleSkillMatching() called!" << std::endl;

    Dynamic_array<Student*>& allStudents = authManager->getAllRegisteredStudents();
    std::cout << "[DEBUG] Total students: " << allStudents.size() << std::endl;

    for (int i = 0; i < allStudents.size(); ++i) {
        std::cout << "Student: " << allStudents[i]->getEmail().return_array() << std::endl;
        auto offered = allStudents[i]->getOfferedSkills();
        auto requested = allStudents[i]->getRequestedSkills();

        std::cout << "  Offered: " << offered.size() << " | Requested: " << requested.size() << std::endl;
    }

    Dynamic_array<Match*> matches = engine.matchStudents(allStudents);
    static bool matchesCreated = false;
    ImGui::OpenPopup("Skill Matching");
    static Dynamic_array<Exchange*> newExchanges;

    if (ImGui::BeginPopup("Skill Matching")) {
        if (matches.size() == 0) {
            ImGui::Text("No valid matches found.");
        }
        else {
            if (!matchesCreated) {
                newExchanges.clear();
                for (int i = 0; i < matches.size(); ++i) {
                    Exchange* ex = engine.createExchangeFromMatch(matches[i], i + 1);

                    if (ex != nullptr) {
                        newExchanges.push(ex);
                        exchangeManager.addExchange(ex);

                        // Setup Chat if current student is part of match
                        Student* requester = authManager->getStudentById(ex->getRequestedSkill()->getStudentId());
                        Student* provider = authManager->getStudentById(ex->getOfferedSkill()->getStudentId());

                        if (currentStudent != nullptr &&
                            (currentStudent == requester || currentStudent == provider)) {

                            Str currentEmail = currentStudent->getEmail();
                            Str opponentEmail = (currentStudent == requester) ? provider->getEmail() : requester->getEmail();

                            // Load or create chat
                            ChatBox* box = chatManager.loadOrCreateChat(currentEmail, opponentEmail);
                            chatBoxWidget = ChatBoxWidget(3);
                            chatBoxWidget.setOpponentEmail(opponentEmail);
                            chatBoxWidget.show();

                            // Show previous messages
                            for (int j = 0; j < box->getMessages().size(); ++j)
                                chatBoxWidget.receiveMessage(box->getMessages()[j].getContent());

                            // Set callback for sending
                            chatBoxWidget.onSendCallback = [&]() {
                                Str sender = currentStudent ? currentStudent->getEmail() : Str("");
                                Str receiver = chatBoxWidget.getOpponentEmail();

                                if (sender.empty() || receiver.empty()) {
                                    std::cout << "Cannot send message: sender or receiver is empty.\n";
                                    return;
                                }
                                chatBoxWidget.sendMessage(sender, receiver, chatManager);
                                };
                        }
                    }
                }
                exchangeManager.saveToFile("exchanges.bin");
                matchesCreated = true;
            }

            ImGui::Text("Match Results:");
            for (int i = 0; i < newExchanges.size(); ++i) {
                Student* req = authManager->getStudentById(newExchanges[i]->getRequestedSkill()->getStudentId());
                Student* prov = authManager->getStudentById(newExchanges[i]->getOfferedSkill()->getStudentId());

                if (req == nullptr || prov == nullptr) {
                    std::cout << "[ERROR] Missing student during rendering!" << std::endl;
                    continue;
                }

                Str skill = newExchanges[i]->getOfferedSkill()->getName();
                ImGui::BulletText("Requester: %s | Provider: %s | Skill: %s",
                    req->getEmail().return_array(),
                    prov->getEmail().return_array(),
                    skill.return_array());
                ImGui::Indent();
                ImGui::Text("Exchange ID: %d", newExchanges[i]->getId());
                ImGui::Text("Status: %s", newExchanges[i]->getStatus().return_array());
                ImGui::Text("Deadline: %s", newExchanges[i]->getRequestedSkill()->get_deadline().toString().return_array());
                ImGui::Unindent();
                ImGui::Separator();
            }

            if (ImGui::Button("OK")) {
                matchesCreated = false;
                ImGui::CloseCurrentPopup();
            }
        }
        ImGui::EndPopup();
    }
}
void SkillExchangeApplication::renderStartChatPopup() {
    static char opponentEmail[100] = ""; 
    static bool showErrorPopup = false;
    static Str errorMessage;
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

    ImGui::Begin("Start New Chat", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::InputText("Opponent Email", opponentEmail, sizeof(opponentEmail));

    if (ImGui::Button("Start Chat")) {
        try {
            if (!currentStudent) {
                throw AuthException(Str("Not logged in!"));
            }

            Str opponent(opponentEmail);
            if (opponent.empty()) {
                throw NetworkException(Str("Please enter opponent's email"));
            }

            if (opponent == currentStudent->getEmail()) {
                throw NetworkException(Str("Cannot chat with yourself"));
            }

            if (!authManager->isEmailRegistered(opponent)) {
                throw AuthException(Str("No user exists with this email"));
            }

            chatBoxWidget = ChatBoxWidget(3);
            chatBoxWidget.setOpponentEmail(opponent);
            chatBoxWidget.onSendCallback = [this]() {
                if (!currentStudent) return;

                Str message = chatBoxWidget.getInputText();
                if (!message.empty()) {
                    chatManager.sendMessage(
                        currentStudent->getEmail(),
                        chatBoxWidget.getOpponentEmail(),
                        message
                    );
                    chatBoxWidget.receiveMessage(message);
                    chatBoxWidget.clearInput();
                }
                };
            chatBoxWidget.show();
            isChatCreating = false;
            opponentEmail[0] = '\0';  // Clear input field
        }
        catch (const AppException& e) {
            errorMessage = Str("Error: ").concat(e.getMessage());
            showErrorPopup = true;
        }
        catch (...) {
            errorMessage = Str("Error: Failed to create chat");
            showErrorPopup = true;
        }
    }

    if (ImGui::Button("Cancel")) {
        isChatCreating = false;
        opponentEmail[0] = '\0';
    }

    ImGui::End();
}
void SkillExchangeApplication::openPreviousChatBox() {
    if (!currentStudent) return;

    ImGui::Begin("Previous Chats", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

    Str currentEmail = currentStudent->getEmail();
    Dynamic_array<Str> chatPartners;
    for (const auto& entry : fs::directory_iterator(".")) {
        Str filename(entry.path().filename().string().c_str());
        if (filename.length(filename.return_array()) < 10 ||
            !filename.is_greater("chat_") ||
            !filename.is_less(".bin")) {
            continue;
        }
        Str middle = filename.substring(5, filename.length(filename.return_array()) - 4);
        int partCount = 0;
        Str* parts = middle.split('_', partCount);
        if (partCount != 2) {
            delete[] parts;
            continue;
        }

        Str user1 = parts[0].trim();
        Str user2 = parts[1].trim();
        delete[] parts;

        if (user1.is_equal(currentEmail)) {
            chatPartners.push(user2);
        }
        else if (user2.is_equal(currentEmail)) {
            chatPartners.push(user1);
        }
    }
    if (chatPartners.isEmpty()) {
        ImGui::Text("No previous chats found.");
    }
    else {
        static int selected = -1;
        for (int i = 0; i < chatPartners.size(); i++) {
            if (ImGui::Selectable(chatPartners[i].return_array(), selected == i)) {
                selected = i;
            }
        }

        if (selected >= 0 && ImGui::Button("Open Chat")) {
            try {
                ChatBox* chat = chatManager.loadOrCreateChat(
                    currentEmail,
                    chatPartners[selected]
                );
                chatBoxWidget = ChatBoxWidget(3);
                chatBoxWidget.setOpponentEmail(chatPartners[selected]);
                for (int i = 0; i < chat->getMessages().size(); i++) {
                    const Message& m = chat->getMessages()[i];
                    Str prefix = (m.getSender().is_equal(currentEmail)) ? Str("You: ") : Str("Them: ");
                    chatBoxWidget.receiveMessage(prefix.concat(m.getContent()));
                }

                chatBoxWidget.show();
                selected = -1; 
            }
            catch (...) {
                std::cerr << "Failed to load chat" << std::endl;
            }
        }
    }

    ImGui::End();
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
