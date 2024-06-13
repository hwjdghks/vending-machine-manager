#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Page.hpp"
#include "Program.hpp"

#include <GLFW/glfw3.h>
#include <thread>
#include <signal.h>

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
        // 저장되어 있는 버퍼를 처리
        program->parse();
        // ImGui 렌더링
        DebugLog::Draw("Detail log");
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // OpenGL 버퍼 스왑
        glfwSwapBuffers(window);
    }
    // 창 종료
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
    signal(SIGPIPE, SIG_IGN);
    Program *program = new Program();

    std::thread windows(drawGui, program);
    windows.join();
    delete program;
    return 0;
}