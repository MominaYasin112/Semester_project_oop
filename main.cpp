#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include "SkillExchangeApplication.h"


#include <fstream>
#include "Str.h" 

void createTestChatFile() {
    std::ofstream out("chat_momina@gmail.com_bilal@gmail.com.bin", std::ios::binary);

    Str sender1("momina@gmail.com");
    Str receiver1("bilal@gmail.com");
    Str message1("Hi Bilal, you there?");

    Str sender2("bilal@gmail.com");
    Str receiver2("momina@gmail.com");
    Str message2("Yes, I am here!");

    Str sender3("momina@gmail.com");
    Str receiver3("bilal@gmail.com");
    Str message3("Great, let's begin.");

    sender1.writeToBinary(out);
    receiver1.writeToBinary(out);
    message1.writeToBinary(out);

    sender2.writeToBinary(out);
    receiver2.writeToBinary(out);
    message2.writeToBinary(out);

    sender3.writeToBinary(out);
    receiver3.writeToBinary(out);
    message3.writeToBinary(out);

    out.close();
}


int main() {
    // Initialize GLFW
    if (!glfwInit()) return -1;
      createTestChatFile();
    // Set OpenGL version (for compatibility)
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    // Create a GLFW window
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Skill Exchange Platform", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    SkillExchangeApplication app;
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        app.render();
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
//#include "ChatManager.h"
//#include "Student.h"
//#include <iostream>
//using namespace std;
//
//int main() {
//    ChatManager chatManager;
//
//    // Simulate two users
//    Student alice("alice@example.com", "pass",40);
//    Student bob("bob@example.com", "pass",40);
//
//    Str aliceEmail = alice.getEmail();
//    Str bobEmail = bob.getEmail();
//
//    cout << "=== Sending messages from Alice to Bob ===\n";
//    chatManager.sendMessage(aliceEmail, bobEmail, Str("Hi Bob! How are you?"));
//    chatManager.sendMessage(aliceEmail, bobEmail, Str("Let's schedule our skill exchange."));
//
//    cout << "\n=== Sending reply from Bob to Alice ===\n";
//    chatManager.sendMessage(bobEmail, aliceEmail, Str("Hey Alice! Sounds good. I'm available tomorrow."));
//
//    cout << "\n=== Displaying final chat ===\n";
//    ChatBox* chat = chatManager.loadOrCreateChat(aliceEmail, bobEmail);
//    chat->displayChat();
//
//    delete chat; // clean up if loadOrCreateChat returns a heap-allocated object
//
//    return 0;
//}
