#include <stdio.h>
#include <unistd.h>
#include <thread>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <string>

#include "Page.hpp"
#include "Program.hpp"

#define NUM_THREADS 2

// 자판기 gui를 그리는 함수
void *drawGui(void *arg)
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
    // 설정 저장 안함(ini 파일 생성 안함)
    io.IniFilename = nullptr;
    // 나눔고딕 볼드체 폰트 사용
    io.Fonts->AddFontFromFileTTF("./public/NanumGothicBold.ttf", 20.0f, nullptr, io.Fonts->GetGlyphRangesKorean()); // 나눔 고딕 폰트 사용
    // 기본 색상
    ImGui::StyleColorsDark();

    // 프로그램 창 초기화
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    /* Custom Section */
    DebugLog::Clear();
    Page::initDrawData();
    Program *program = static_cast<Program *>(arg);
    // 메인 루프
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // ImGui 프레임 시작
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        if (Page::getDrawID() == ViewMode::SALES)
            Page::drawSalesWindows(*program);
        else
            Page::drawAdminWindows(*program);
        std::string sock_log = program->getClient().getFromRead();
        if (!sock_log.empty())
            DebugLog::AddLog("%s", sock_log.c_str());
        // ImGui 렌더링
        DebugLog::Draw("Detail log");
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
    // 창 종료
    /* Need Edit */
    program->getClient().closeConnect();

    // ImGui 정리
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // GLFW 정리
    glfwDestroyWindow(window);
    glfwTerminate();
    return nullptr;
}

int main() {
    Program *program = new Program();

    std::thread windows(drawGui, program);
    windows.join();
    delete program;
    return 0;
}