#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>

#define NUM_THREADS 2
// 첫 번째 함수
void *function1(void *arg)
{
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(800, 600, "ImGui Example", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    // ImGui 초기화
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = nullptr;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");
    bool buttonClicked = false;
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowMenuButtonPosition = ImGuiDir_None;
    // 메인 루프
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // ImGui 프레임 시작
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // 단순한 "Hello, World!" 창 생성
        ImGui::Begin("Hello, world!", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
        ImGui::SetWindowSize(ImVec2(800, 600));
        // "Click me!" 버튼 생성
        // 직접적으로 UI 요소 생성
        ImVec2 buttonSize(1000, 500); // 버튼 크기 지정
        if (!buttonClicked) {
            ImGui::Text("Hello, world!");
            if (ImGui::Button("Click me!", buttonSize))
                buttonClicked = true;
        }

        // 버튼이 클릭되었는지 확인하여 텍스트 표시
        if (buttonClicked) {
            ImGui::Text("Button clicked!");
            if (ImGui::Button("return"))
                buttonClicked = false;
        }
        ImGui::End();
        // ImGui 렌더링
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);
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
    void *(*thread_functions[NUM_THREADS])(void *) = {function1, function2};

    // 스레드 배열 생성 및 실행
    for (int i = 0; i < NUM_THREADS; ++i) {
        if (pthread_create(&threads[i], NULL, thread_functions[i], NULL)) {
            fprintf(stderr, "Error creating thread %d\n", i);
            return 1;
        }
    }

    // 스레드 종료 대기
    for (int i = 0; i < NUM_THREADS; ++i) {
        if (pthread_join(threads[i], NULL)) {
            fprintf(stderr, "Error joining thread %d\n", i);
            return 2;
        }
    }

    return 0;
}