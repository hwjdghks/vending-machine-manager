#include <stdio.h>
#include <unistd.h>
#include <thread>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <string>

#include "DebugLog.hpp"
#include "Page.hpp"
#include "VendingMachine.hpp"

#define NUM_THREADS 2

// 첫 번째 함수
void *function1(void *arg)
{
    if (!glfwInit())
        return nullptr;
    // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(1280, 720, "ImGui Example", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    // ImGui 초기화
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = nullptr; // 설정 저장 안함
    io.Fonts->AddFontFromFileTTF("./public/NanumGothicBold.ttf", 20.0f, nullptr, io.Fonts->GetGlyphRangesKorean()); // 나눔 고딕 폰트 사용
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowMenuButtonPosition = ImGuiDir_None;

    /* Custom Section */
    DebugLog::Clear();
    Page::initDrawData();
    VendingMachine *machine = static_cast<VendingMachine *>(arg);
    // 메인 루프
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // ImGui 프레임 시작
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        if (Page::getDrawID() == ViewMode::SALES)
            Page::drawSalesWindows(*machine);
        else
            Page::drawAdminWindows();
        // ImGui 렌더링
        DebugLog::Draw("testing Log");
        ImGui::Render();
        // int display_w, display_h;
        // glfwGetFramebufferSize(window, &display_w, &display_h);
        // glViewport(0, 0, display_w, display_h);
        // glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
        // glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // OpenGL 버퍼 스왑
        glfwSwapBuffers(window);
    }
    // ImGui 정리
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // GLFW 정리
    glfwDestroyWindow(window);
    glfwTerminate();

    // 창 종료
    /* Need Edit */
    return nullptr;
}

// 두 번째 함수
void *function2(void *arg) {
    VendingMachine *machine = static_cast<VendingMachine *>(arg);

    return nullptr;
}

int main() {
    VendingMachine *machine = new VendingMachine();

    std::thread windows(function1, machine);
    windows.join();
    delete machine;
    return 0;
}