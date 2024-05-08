#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <string>

#include "DebugLog.hpp"
#include "GuiWrapper.hpp"

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
    GuiWrapper::initDrawData();

    // 메인 루프
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // ImGui 프레임 시작
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        if (GuiWrapper::getDrawID() == ViewMode::SALES)
            GuiWrapper::drawSalesWindows();
        else
            GuiWrapper::drawAdminWindows();
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
    return NULL;
}

// 두 번째 함수
void *function2(void *arg) {
    while (true)
    {
        printf("This is function2\n");
        sleep(1);
    }
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    void *(*thread_functions[NUM_THREADS])(void *) = {
        function1,
        function2
    };

    // 스레드 배열 생성 및 실행
    for (int i = 0; i < NUM_THREADS; ++i) {
        if (pthread_create(&threads[i], nullptr, thread_functions[i], nullptr /* data */)) {
            fprintf(stderr, "Error creating thread %d\n", i);
            return 1;
        }
    }

    // 스레드 종료 대기
    for (int i = 0; i < NUM_THREADS; ++i) {
        if (pthread_join(threads[i], nullptr)) {
            fprintf(stderr, "Error joining thread %d\n", i);
            return 2;
        }
    }

    return 0;
}