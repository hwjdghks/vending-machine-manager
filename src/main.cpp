#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <string>

#include "DebugLog.hpp"

#define NUM_THREADS 2


void beginDefaultProps(const std::string &title)
{
    ImGui::Begin(title.c_str(), nullptr,
    ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
}

void initWindowProps(const std::string &title, const ImVec2 &size, const ImVec2 &pos)
{
    beginDefaultProps(title);
    ImGui::SetWindowSize(size);
    ImGui::SetWindowPos(pos);
}

void display(DebugLog &mylog)
{
    static bool buttonClicked = false; // 토글은 static 변수 사용
    static bool num = true;
    ImVec2 sizeRatio(0.75f, 1.0f);
    ImVec2 posRatio(0.0f, 0.0f);
    // 단순한 "Hello, World!" 창 생성
    initWindowProps("test", getVec2(sizeRatio), getVec2(posRatio));
    // "Click me!" 버튼 생성
    // 직접적으로 UI 요소 생성
    ImVec2 buttonSize(200, 200); // 버튼 크기 지정
    if (!buttonClicked) {
        ImGui::Text("Hello, world!");
        if (ImGui::Button("Click me!##1", buttonSize)) {
            mylog.AddLog("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaButton 1 Click\n");
            buttonClicked = true;
        }
    }
    // 버튼이 클릭되었는지 확인하여 텍스트 표시
    else if (buttonClicked) {
        ImGui::Text("Button clicked!");
        if (ImGui::Button("return##2", buttonSize)) {
            mylog.AddLog("Button 2 Click\n");
            buttonClicked = false;
        }
        if (num) {
            ImGui::OpenPopup("Small Window");
            if (ImGui::BeginPopupModal("Small Window", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
            {
                ImGui::Text("This is a small modal window!");
                if (ImGui::Button("Close")) {
                    mylog.AddLog("Close button Click\n");
                    num = false;
                }
                ImGui::EndPopup();
            }
        }
    }
    ImGui::End();
}

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
    io.Fonts->AddFontFromFileTTF("./public/NanumGothicBold.ttf", 21); // 나눔 고딕 폰트 사용
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowMenuButtonPosition = ImGuiDir_None;

    DebugLog mylog;
    // 메인 루프
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // ImGui 프레임 시작
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        display(mylog);
        // ImGui 렌더링
        mylog.Draw("debug");
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