#ifndef SKILL_EXCHANGE_APPLICATION_H
#define SKILL_EXCHANGE_APPLICATION_H

#include "AuthenticationManager.h"
#include "SkillRepository.h"
#include"ChatManager.h"
#include "DateTime.h"
#include "OfferedSkills.h"
#include "RequestedSkills.h"
#include "MatchingEngine.h"
#include "HungarianMatcher.h"
#include "ExchangeManager.h"
#include"UIComponent.h"


class SkillExchangeApplication {
private:
    AuthenticationManager* authManager;
    LoginWindow loginWindow;
    Dashboard dashboard;
    ChatBoxWidget chatBoxWidget;

    SkillDropdown offeredDropdown;
    SkillDropdown requestedDropdown;
    TimeSlotPicker timeSlotPicker;
    DeadlinePicker deadlinePicker;

    Button registerButton;
    bool isRegistering = false;
    Student* currentStudent = nullptr;

    MatchingEngine engine;
    HungarianMatcher matcher;
    ExchangeManager exchangeManager;
    ChatManager chatManager;
    bool isChatCreating;
    bool showChatSelector;
    bool debugMode = false;
    bool isMatchDropdownVisible = false;
    int selectedMatchIndex = -1;
    Dynamic_array<Match> matchResults;
    Str lastExchangeMessage;
    float messageDisplayTime = 0.0f;
    bool showMessage = false;
    bool chatPopupOpened = false;
    bool showChatBoxWindow = false;
public:
    SkillExchangeApplication();
    void render();
    void handleLogin();
    void handleRegistration();
    void showDashboard();
    void renderStartChatPopup();
    void openChatBox();
    void openPreviousChatBox();
    void handleMessageSending();
    void showMatchResults();
};

#endif
