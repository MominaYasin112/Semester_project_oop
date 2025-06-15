#ifndef UI_COMPONENTS_H
#define UI_COMPONENTS_H

#include "Dynamic_array.h"
#include "Str.h"
#include "DateTime.h"
#include <functional>
#include "imgui.h"
//Base Class
class UIComponent {
protected:
    int componentId;
    bool isVisible;
public:
    UIComponent(int id, bool visible = true)
        : componentId(id), isVisible(visible) {
    }

    virtual ~UIComponent() {}
    virtual void render() = 0;

    void show() { isVisible = true; }
    void hide() { isVisible = false; }
    bool visible() const { return isVisible; }
    int getId() const { return componentId; }
};
//To add email passsword and other manually input things
class TextBox : public UIComponent {
private:
    Str text;
    Str placeholder;

public:
    TextBox(int id, const Str& placeholder = "")
        : UIComponent(id), placeholder(placeholder), text("") {
    }

    void render() override {
        char buffer[256];
        strncpy_s(buffer, text.return_array(), sizeof(buffer));
        if (ImGui::InputText(placeholder.return_array(), buffer, sizeof(buffer))) {
            text = buffer;
        }
    }

    const Str& getText() const { return text; }
    void setText(const Str& newText) { text = newText; }
};
//Button for different operations
class Button : public UIComponent {
private:
    Str text;
    std::function<void()> onClick;

public:
    Button(int id, const Str& buttonText, std::function<void()> onClickAction)
        : UIComponent(id), text(buttonText), onClick(onClickAction) {
    }

    void render() override {
        if (ImGui::Button(text.return_array())) {
            if (onClick) onClick();
        }
    }
    void click() { if (onClick) onClick(); }
};
//Login Window
class LoginWindow : public UIComponent {
private:
    TextBox emailField;
    TextBox passwordField;
    Button loginButton;
    Str successMessage;
public:
    LoginWindow(int id)
        : UIComponent(id),
        emailField(TextBox(id + 1, "Enter email")),
        passwordField(TextBox(id + 2, "Enter password")),
        loginButton(Button(id + 3, "Login", []() {})) {
    }
    void showSuccessMessage(const Str& message) {
        successMessage = message;
    }

    void render() override {
        if (!isVisible) return;
        ImGui::Begin("Login Window");
        if (!successMessage.empty()) {
            ImGui::TextColored(ImVec4(0, 1, 0, 1), successMessage.return_array());
            successMessage = "";
        }
        emailField.render();
        passwordField.render();
        loginButton.render();
        ImGui::End();
    }

    void setLoginCallback(std::function<void()> cb) {
        loginButton = Button(loginButton.getId(), "Login", cb);
    }

    const Str& getEmail() const { return emailField.getText(); }
    const Str& getPassword() const { return passwordField.getText(); }

    bool validateInput() const {
        return getEmail().length(getEmail().return_array()) > 0 &&
            getPassword().length(getPassword().return_array()) > 0;
    }

    void showErrorMessage(const Str& message) {
        ImGui::TextColored(ImVec4(1, 0, 0, 1), message.return_array());
    }
    void setEmail(const Str& e) {
        emailField.setText(e);
    }

    void setPassword(const Str& p) {
        passwordField.setText(p);
    }
};
//For Chatting
class ChatBoxWidget : public UIComponent {
private:
    TextBox inputField;
    Button sendButton;
    Dynamic_array<Str> messageList;
    Str opponentEmail;

public:
    std::function<void()> onSendCallback;

    ChatBoxWidget(int id)
        : UIComponent(id),
        inputField(id + 1, "Type your message..."),
        sendButton(id + 2, "Send", [this]() {
        if (onSendCallback) onSendCallback();
        else std::cout << "⚠️ Send callback not set!\n";
            })
    {
    }
    void render() {
        if (!isVisible) return;

        ImGui::Begin("Chat Box", &isVisible, ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::BeginChild("ChatHistory", ImVec2(400, 300), true);
        for (int i = 0; i < messageList.size(); i++) {
            ImGui::TextWrapped("%s", messageList[i].return_array());
            if (i < messageList.size() - 1) {
                ImGui::Separator();
            }
        }
        if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
            ImGui::SetScrollHereY(1.0f);
        }
        ImGui::EndChild();
        inputField.render();
        ImGui::SameLine();
        if (ImGui::Button("Send") && onSendCallback) {
            onSendCallback();
        }

        ImGui::End();
    }

    void sendMessage(const Str& senderEmail, const Str& receiverEmail, ChatManager& chatManager) {
        Str message = inputField.getText();
        if (message.length(message.return_array()) > 0) {
            chatManager.sendMessage(senderEmail, receiverEmail, message);
            messageList.push(Str("You: ") + message); 
            inputField.setText("");
        }
    }

    void show() {
        isVisible = true;
    }
    void reset() {
        messageList.clear();
    }
    void setOpponentEmail(Str email) { opponentEmail = email; }
    Str getOpponentEmail() const { return opponentEmail; }
    void receiveMessage(const Str& msg) {
        messageList.push(msg);
    }
    void loadPreviousMessages(ChatManager& manager, const Str& sender, const Str& receiver) {
        messageList.clear();
        ChatBox* chat = manager.loadOrCreateChat(sender, receiver);
        const Dynamic_array<Message>& msgs = chat->getMessages();
        for (int i = 0; i < msgs.size(); ++i) {
            const Message& m = msgs[i];
            Str tag = (m.getSender().is_equal(sender)) ? "You: " : "Them: ";
            messageList.push(tag + m.getContent());
        }
    }
    Str getInputText() const {
        return inputField.getText();
    }
    void clearInput() {
        inputField.setText("");
    }
};
//To choose skills from dropdowm instead of input to avoid spelling errors
class SkillDropdown : public UIComponent {
private:
    Dynamic_array<Str> options;
    int selectedIndex;

public:
    SkillDropdown(int id, const Dynamic_array<Str>& options)
        : UIComponent(id), options(options), selectedIndex(0) {
        if (options.size() == 0) {
            selectedIndex = -1;
        }
    }
    void render() override {
        if (!isVisible) return;

        if (options.size() == 0) {
            ImGui::Text("No skills available.");
            return;
        }

        if (ImGui::BeginCombo("Select Skill", options[selectedIndex].return_array())) {
            for (int i = 0; i < options.size(); ++i) {
                bool isSelected = (i == selectedIndex);
                if (ImGui::Selectable(options[i].return_array(), isSelected)) {
                    selectedIndex = i;
                }
                if (isSelected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
    }


    Str getSelectedSkill() const {
        if (selectedIndex < 0 || selectedIndex >= options.size()) {
            return "None";
        }
        return options[selectedIndex];
    }
    void reset() {
        selectedIndex = 0; 
    }
};
//for user to pick 3 different time slots based on his avaliability
class TimeSlotPicker : public UIComponent {
private:
    Dynamic_array<TimeSlot> timeSlots;
    Dynamic_array<int> selectedIndices;

public:
    TimeSlotPicker(int id) : UIComponent(id) {
        DateTime today = DateTime::now();

        for (int hour = 8; hour < 17; ++hour) {
            DateTime start = today;
            start.hour = hour;
            start.minute = 0;

            DateTime end = today;
            end.hour = hour + 1;
            end.minute = 0;

            timeSlots.push(TimeSlot(start, end));
        }
    }


    void render() override {
        if (!isVisible) return;

        ImGui::Text("Select up to 3 Time Slots:");
        for (int i = 0; i < timeSlots.size(); ++i) {
            bool selected = std::find(selectedIndices.begin(), selectedIndices.end(), i) != selectedIndices.end();

            Str label = timeSlots[i].toString();
            if (ImGui::Checkbox(label.return_array(), &selected)) {
                if (selected && selectedIndices.size() < 3)
                    selectedIndices.push(i);
                else if (!selected) {
                    for (int j = 0; j < selectedIndices.size(); ++j) {
                        if (selectedIndices[j] == i) {
                            selectedIndices.removeAt(j);
                            break;
                        }
                    }
                }
            }
        }
    }

    Dynamic_array<TimeSlot> getSelectedTimeSlots() const {
        Dynamic_array<TimeSlot> result;
        for (int i = 0; i < selectedIndices.size(); ++i)
            result.push(timeSlots[selectedIndices[i]]);
        return result;
    }
    void reset() {
        selectedIndices.clear();
    }
};
//To set dealine for 2 weeks 
class DeadlinePicker : public UIComponent {
private:
    DateTime deadline;

public:
    DeadlinePicker(int id) : UIComponent(id) {
        deadline = DateTime::now();
        deadline.day += 14;
    }

    void render() override {
        if (!isVisible) return;
        ImGui::Text("Deadline for Requested Skill:");
        deadline.dis();
    }

    DateTime getDeadline() const {
        return deadline;
    }
};
//To offer and request skill
class SkillPanel : public UIComponent {
private:
    Dynamic_array<Str> offeredSkillsList;
    Dynamic_array<Str> requestedSkillsList;

public:
    SkillPanel(int id)
        : UIComponent(id) {
    }

    void render() override {
        if (!isVisible) return;
        ImGui::Begin("Skill Panel");
        ImGui::Text("Offered Skills:");
        for (int i = 0; i < offeredSkillsList.size(); ++i) {
            ImGui::BulletText(offeredSkillsList[i].return_array());
        }

        ImGui::Text("Requested Skills:");
        for (int i = 0; i < requestedSkillsList.size(); ++i) {
            ImGui::BulletText(requestedSkillsList[i].return_array());
        }
        ImGui::End();
    }

    void addOfferedSkill(const Str& skill) { offeredSkillsList.push(skill); }
    void addRequestedSkill(const Str& skill) { requestedSkillsList.push(skill); }
};
//Main DashBoard
class Dashboard : public UIComponent {
private:
    Button skillMatchBtn;
    Button chatBoxBtn;
    Button startNewChatBtn;
    Button addSkillBtn;
    Button exitBtn;

    // Track button states
    bool chatButtonPressed = false;
    bool matchButtonPressed = false;
    bool newChatButtonPressed = false;
    bool addSkillButtonPressed = false;
    bool exitButtonPressed = false;

    std::function<void()> onStartNewChatClick;

public:
    Dashboard(int id)
        : UIComponent(id),
        skillMatchBtn(Button(id + 1, "Find Matches", [this]() { matchButtonPressed = true; })),
        chatBoxBtn(Button(id + 2, "Open ChatBox", [this]() { chatButtonPressed = true; })),
        startNewChatBtn(Button(id + 3, "Start New Chat", [this]() { newChatButtonPressed = true; })),
        addSkillBtn(Button(id + 4, "Add Skills", [this]() { addSkillButtonPressed = true; })),
        exitBtn(Button(id + 5, "Exit", [this]() { exitButtonPressed = true; })) {
    }

    int reputation = 0;

    // Button state checkers
    bool isChatButtonPressed() {
        bool state = chatButtonPressed;
        chatButtonPressed = false;
        return state;
    }

    bool isMatchButtonPressed() {
        bool state = matchButtonPressed;
        matchButtonPressed = false;
        return state;
    }

    bool isNewChatButtonPressed() {
        bool state = newChatButtonPressed;
        newChatButtonPressed = false;
        return state;
    }

    bool isAddSkillButtonPressed() {
        bool state = addSkillButtonPressed;
        addSkillButtonPressed = false;
        return state;
    }

    bool isExitButtonPressed() {
        bool state = exitButtonPressed;
        exitButtonPressed = false;
        return state;
    }

    void setReputation(int rep) {
        reputation = rep;
    }

    void render() override {
        if (!isVisible) return;

        ImGui::Begin("Main Dashboard", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

        ImGui::Text("Reputation: %d", reputation);
        ImGui::Separator();

        // Render buttons and check clicks
        skillMatchBtn.render();
        chatBoxBtn.render();
        startNewChatBtn.render();
        addSkillBtn.render();
        exitBtn.render();

        ImGui::End();
    }

    // Callback setters
    void setOnMatchClick(std::function<void()> cb) {
        skillMatchBtn = Button(skillMatchBtn.getId(), "Find Matches",
            [this, cb]() { matchButtonPressed = true; if (cb) cb(); });
    }

    void setOnChatClick(std::function<void()> cb) {
        chatBoxBtn = Button(chatBoxBtn.getId(), "Open ChatBox",
            [this, cb]() { chatButtonPressed = true; if (cb) cb(); });
    }

    void setOnAddClick(std::function<void()> cb) {
        addSkillBtn = Button(addSkillBtn.getId(), "Add Skills",
            [this, cb]() { addSkillButtonPressed = true; if (cb) cb(); });
    }

    void setOnExitClick(std::function<void()> cb) {
        exitBtn = Button(exitBtn.getId(), "Exit",
            [this, cb]() { exitButtonPressed = true; if (cb) cb(); });
    }

    void setOnStartNewChatClick(std::function<void()> cb) {
        onStartNewChatClick = cb;
        startNewChatBtn = Button(startNewChatBtn.getId(), "Start New Chat",
            [this]() { newChatButtonPressed = true; if (onStartNewChatClick) onStartNewChatClick(); });
    }
};
#endif // UI_COMPONENTS_H