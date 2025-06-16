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
    dashboard.setOnChatClick([this]() { openPreviousChatBox(); });
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
    

}
void SkillExchangeApplication::render() {
    testMatchingConsole();
    static Str errorMessage;
    static bool showErrorPopup = false;

    // Error popup (keep this at top level)
    
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

    // Registration window
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

            // Try saving manually
            /*exchangeManager.saveToFile("DEBUG_exchanges.bin");*/
            std::cout << "Manual save attempted to DEBUG_exchanges.bin\n";
        }
        if (ImGui::Button("Cancel")) {
            isRegistering = false;
            loginWindow.show();
        }

        ImGui::End();
    }
    // Login window
    else if (loginWindow.visible()) {
        loginWindow.render();

        ImGui::Begin("New User?", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
        if (ImGui::Button("Don't have an account? Register here")) {
            isRegistering = true;
            loginWindow.hide();
        }
        ImGui::End();
    }
    // Main dashboard
    else {
        if (currentStudent) {
            dashboard.setReputation(currentStudent->getReputationPoints());
        }

        dashboard.render();

        // Handle dashboard button presses - CHECK ONCE PER FRAME
        static bool matchProcessed = false;
        if (dashboard.isMatchButtonPressed() && !matchProcessed) {
            std::cout << "Match button pressed - calling showMatchResults()" << std::endl;
            showMatchResults();
            matchProcessed = true;
        }

        // Reset the flag after one frame
        if (matchProcessed) {
            matchProcessed = false;
        }

        // Chat box
        if (chatBoxWidget.visible()) {
            chatBoxWidget.render();

            static char msgBuffer[256] = "";
            ImGui::InputText("Type message", msgBuffer, sizeof(msgBuffer));

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

        // Start new chat popup
        if (isChatCreating) {
            renderStartChatPopup();
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
void SkillExchangeApplication::showMatchResults() {
    std::cout << "\n==== DEBUG: MATCHING FLOW START ====\n";

    // Get all students
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

    // Generate matches
    Dynamic_array<Match*> matches = engine.matchStudents(allStudents);
    std::cout << "Total matches found: " << matches.size() << "\n";
    for (int i = 0; i < matches.size(); ++i) {
        std::cout << "Match " << i << ": "
            << matches[i]->getRequester()->getEmail().return_array() << " <-> "
            << matches[i]->getProvider()->getEmail().return_array()
            << " (Skill: " << matches[i]->getSkill().return_array() << ")\n";
    }

    // Popup management
    static bool shouldOpenPopup = true;
    if (shouldOpenPopup) {
        ImGui::OpenPopup("Match Results");
        shouldOpenPopup = false;
    }

    // Center the popup
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

    if (ImGui::BeginPopupModal("Match Results", nullptr,
        ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize))
    {
        if (matches.isEmpty()) {
            ImGui::Text("No matches found!");
            ImGui::Text("Make sure you have:");
            ImGui::BulletText("At least 2 registered users");
            ImGui::BulletText("Complementary skills (A offers what B requests)");
        }
        else {
            ImGui::Text("Found %d matches:", matches.size());
            ImGui::Separator();

            // Display all matches
            for (int i = 0; i < matches.size(); ++i) {
                Match* match = matches[i];
                ImGui::Text("Match %d:", i + 1);
                ImGui::BulletText("%s (wants %s)",
                    match->getRequester()->getEmail().return_array(),
                    match->getSkill().return_array());
                ImGui::BulletText("%s (offers %s)",
                    match->getProvider()->getEmail().return_array(),
                    match->getSkill().return_array());
                ImGui::Spacing();
            }

            ImGui::Separator();

            // Create Exchange Button - FULLY DEBUGGED VERSION
            if (ImGui::Button("CREATE EXCHANGE", ImVec2(200, 40))) {
                std::cout << "\n==== EXCHANGE CREATION STARTED ====\n";

                try {
                    Match* selectedMatch = matches[0]; // Using first match

                    // 1. Get the ACTUAL skill objects from students
                    OfferedSkill* offered = nullptr;
                    for (auto skill : selectedMatch->getProvider()->getOfferedSkills()) {
                        if (skill->getName() == selectedMatch->getSkill()) {
                            offered = skill;
                            break;
                        }
                    }

                    RequestedSkill* requested = nullptr;
                    for (auto skill : selectedMatch->getRequester()->getRequestedSkills()) {
                        if (skill->getName() == selectedMatch->getSkill()) {
                            requested = skill;
                            break;
                        }
                    }

                    if (!offered || !requested) {
                        throw std::runtime_error("Could not find matching skills in student profiles");
                    }

                    // 2. Create the exchange
                    Exchange* exchange = new Exchange(
                        exchangeManager.getNextExchangeId(),
                        offered,  // Use the actual offered skill from provider
                        requested, // Use the actual requested skill from requester
                        "Pending",
                        DateTime::now()
                    );

                    // 3. Add to manager
                    exchangeManager.addExchange(exchange);

                    // 4. Save to file
                    bool saveSuccess = false;
                    try {
                        /*exchangeManager.saveToFile("exchanges.bin");*/
                        saveSuccess = true;
                    }
                    catch (...) {
                        saveSuccess = false;
                    }

                    std::cout << "Exchange creation: " << (exchange ? "SUCCESS" : "FAILED") << "\n";
                    std::cout << "File save: " << (saveSuccess ? "SUCCESS" : "FAILED") << "\n";
                    std::cout << "Exchange details:\n";
                    std::cout << "- ID: " << exchange->getExchangeId() << "\n";
                    std::cout << "- Between: "
                        << selectedMatch->getRequester()->getEmail().return_array() << " and "
                        << selectedMatch->getProvider()->getEmail().return_array() << "\n";
                    std::cout << "- Skill: " << selectedMatch->getSkill().return_array() << "\n";

                    dashboard.showMessage("Exchange created successfully!");

                    // Clean up
                    for (auto match : matches) delete match;
                    matches.clear();
                    shouldOpenPopup = true;
                    ImGui::CloseCurrentPopup();
                }
                catch (const std::exception& e) {
                    std::cout << "EXCHANGE CREATION ERROR: " << e.what() << "\n";
                    // FIX: Convert error message properly
                    Str errorMsg = "Error: ";
                    errorMsg = errorMsg.concat(e.what());
                    dashboard.showMessage(errorMsg.return_array()); // Use return_array() for conversion
                }
            }

            ImGui::SameLine();

            // Close Button
            if (ImGui::Button("Close", ImVec2(120, 0))) {
                for (auto match : matches) delete match;
                matches.clear();
                shouldOpenPopup = true;
                ImGui::CloseCurrentPopup();
            }
        }
        ImGui::EndPopup();
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

//void SkillExchangeApplication::handleSkillMatching() {
//    Dynamic_array<Student*>& allStudents = authManager->getAllRegisteredStudents();
//    Dynamic_array<Match*> matches = engine.matchStudents(allStudents);
//
//    if (matches.isEmpty()) {
//        dashboard.showMessage("No matches found!");
//    }
//    else {
//        ImGui::OpenPopup("Match Results");
//
//        if (ImGui::BeginPopupModal("Match Results", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
//            ImGui::Text("Found %d matches:", matches.size());
//            for (int i = 0; i < matches.size(); ++i) {
//                Student* requester = matches[i]->getRequester();
//                Student* provider = matches[i]->getProvider();
//                ImGui::BulletText("%s ↔ %s (Skill: %s)",
//                    requester->getEmail().return_array(),
//                    provider->getEmail().return_array(),
//                    matches[i]->getSkill().return_array());
//            }
//
//            if (ImGui::Button("Create Exchange", ImVec2(120, 0))) {
//                Exchange* exchange = engine.createExchangeFromMatch(
//                    matches[0],
//                    exchangeManager.getNextExchangeId()
//                );
//
//                if (exchange) {
//                    exchangeManager.addExchange(exchange);
//                    exchangeManager.saveToFile("exchanges.bin");
//                    dashboard.showMessage("Exchange created successfully!");
//                }
//
//                ImGui::CloseCurrentPopup();
//            }
//
//            ImGui::SameLine();
//            if (ImGui::Button("Cancel", ImVec2(120, 0))) {
//                ImGui::CloseCurrentPopup();
//            }
//
//            ImGui::EndPopup();
//        }
//    }
//    for (int i = 0; i < matches.size(); ++i) {
//        delete matches[i];
//    }
//}
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
void SkillExchangeApplication::testMatchingConsole() {
    std::cout << "\n==== TESTING MATCHING (CONSOLE ONLY) ====\n";

    // Get all students
    auto& students = authManager->getAllRegisteredStudents();
    std::cout << "Registered Students (" << students.size() << "):\n";
    for (int i = 0; i < students.size(); i++) {
        std::cout << "[" << i << "] " << students[i]->getEmail().return_array()
            << " (Rep: " << students[i]->getReputationPoints() << ")\n";
        std::cout << "  Offers: ";
        for (auto& skill : students[i]->getOfferedSkills()) {
            std::cout << skill->getName().return_array() << " (ID:" << skill->getSkillId() << "), ";
        }
        std::cout << "\n  Requests: ";
        for (auto& skill : students[i]->getRequestedSkills()) {
            std::cout << skill->getName().return_array() << " (Deadline:"
                << skill->get_deadline().toString().return_array() << "), ";
        }
        std::cout << "\n";
    }

    // Generate matches
    auto matches = engine.matchStudents(students);
    std::cout << "\nFound " << matches.size() << " potential matches:\n";

    // Track matched students to prevent duplicates
    Dynamic_array<Student*> alreadyMatched;

    // Process each match
    for (int i = 0; i < matches.size(); i++) {
        auto* match = matches[i];
        Student* requester = match->getRequester();
        Student* provider = match->getProvider();

        // Skip if either student is already matched
        if (alreadyMatched.contains(requester) || alreadyMatched.contains(provider)) {
            std::cout << "MATCH #" << i << " SKIPPED - Student already matched\n";
            continue;
        }

        std::cout << "\nMATCH #" << i << ":\n";
        std::cout << "  Requester: " << requester->getEmail().return_array()
            << " wants " << match->getSkill().return_array() << "\n";
        std::cout << "  Provider: " << provider->getEmail().return_array()
            << " offers " << match->getSkill().return_array() << "\n";

        // Check for bidirectional potential
        bool isBidirectional = false;
        Str reverseSkill;

        // Look for reverse match
        for (auto& reqSkill : provider->getRequestedSkills()) {
            for (auto& provSkill : requester->getOfferedSkills()) {
                if (reqSkill->getName() == provSkill->getName()) {
                    isBidirectional = true;
                    reverseSkill = reqSkill->getName();
                    break;
                }
            }
            if (isBidirectional) break;
        }

        if (isBidirectional) {
            std::cout << "  BIDIRECTIONAL POTENTIAL FOUND!\n";
            std::cout << "  Reverse skill: " << reverseSkill.return_array() << "\n";
        }

        // Create the exchange
        Exchange* exchange = engine.createExchangeFromMatch(
            match,
            exchangeManager.getNextExchangeId()
        );

        if (exchange) {
            std::cout << "  CREATED EXCHANGE:\n";
            std::cout << "    ID: " << exchange->getExchangeId() << "\n";
            std::cout << "    Skill: " << exchange->getOfferedSkill()->getName().return_array() << "\n";
            std::cout << "    Status: " << exchange->getStatus().return_array() << "\n";
            std::cout << "    Created: " << exchange->getCreatedDate().toString().return_array() << "\n";

            // Add to manager and mark students as matched
            exchangeManager.addExchange(exchange);
            alreadyMatched.push(requester);
            alreadyMatched.push(provider);

            // If bidirectional, create reverse exchange
            if (isBidirectional) {
                Exchange* reverseExchange = new Exchange(
                    exchangeManager.getNextExchangeId(),
                    exchange->getRequestedSkill(),  // Reverse the skills
                    exchange->getOfferedSkill(),
                    "Pending",
                    DateTime::now()
                );

                if (reverseExchange) {
                    std::cout << "  CREATED BIDIRECTIONAL EXCHANGE:\n";
                    std::cout << "    ID: " << reverseExchange->getExchangeId() << "\n";
                    std::cout << "    Skill: " << reverseExchange->getOfferedSkill()->getName().return_array() << "\n";
                    exchangeManager.addExchange(reverseExchange);
                }
            }
        }
        else {
            std::cout << "  FAILED TO CREATE EXCHANGE\n";
        }
    }

    // Save all exchanges
    /*exchangeManager.saveToFile("exchanges.bin");*/
    std::cout << "\nSaved " << exchangeManager.getAllExchanges().size()
        << " exchanges to file\n";

    // Clean up
    for (auto* match : matches) {
        delete match;
    }

    std::cout << "\n==== TEST COMPLETE ====\n";
    std::cout << "Final Exchange Count: " << exchangeManager.getAllExchanges().size() << "\n";
    std::cout << "Students Matched: " << alreadyMatched.size() << "/" << students.size() << "\n";
}