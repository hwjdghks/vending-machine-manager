#include "GuiWrapper.hpp"
#include <stdio.h>

GuiWrapper::GuiWrapper() {}

GuiWrapper::~GuiWrapper() {}

void GuiWrapper::drawSalesWindows()
{
    ImVec2 winPosRatio(0.0f, 0.0f);
    ImVec2 winSizeRatio(0.75f, 1.0f);

    __beginDefaultProps("Sales Menu");
    __setWindowProps(winPosRatio, winSizeRatio);

    ImVec2 displayPos(100, 100);
    for(int i = 0; i < 6; i++) {
        _addDisplayBeverage(displayPos + ImVec2((i % 3) * 220, (i / 3) * 150));
    }

    ImVec2 panelPos(100, 450);
    _addPaymentPanel(panelPos);

    ImVec2 swapAdminPagePos(800, 80);
    ImGui::SetCursorPos(swapAdminPagePos);
    if (ImGui::Button("    change\nAdmin Page"))
        global_Menu = 0;

    ImGui::End();
}

void GuiWrapper::drawAdminWindows()
{
    ImVec2 winPosRatio(0.0f, 0.0f);
    ImVec2 winSizeRatio(0.75f, 1.0f);

    __beginDefaultProps("Admin Menu");
    __setWindowProps(winPosRatio, winSizeRatio);
    if (ImGui::Button("Change Menu"))
        global_Menu = 1;
    ImGui::End();
}

void GuiWrapper::_addDisplayBeverage(/* each Beverage Data */ const ImVec2 &pos)
{
    __addDisplayPanel(pos);
    __addDisplayPanel(pos + ImVec2(0, 30));
    __addBuyButton(pos + ImVec2(0, 60));
}

void GuiWrapper::_addPaymentPanel(const ImVec2 &pos)
{
    const char *labels[] = {
        "10원",
        "50원",
        "100원",
        "500원",
        "1000원"
    };
    for (int i = 0; i < 5; i++)
        __addCoinButton(labels[i], pos + ImVec2((i % 3) * 150, (i / 3) * 100));

    ImVec2 current(650, 450);
    ImGui::SetCursorPos(current);
    ImGui::Text("현재 잔액: ");
    /* Need edit */
    // 잔액 부족 시 추가 출력
    ImGui::SetCursorPos(current + ImVec2(0, 50));
    ImGui::Button("반환", ImVec2(150, 60));
}

void GuiWrapper::__addDisplayPanel(const ImVec2 &pos)
{
    ImGui::SetCursorPos(pos);
    ImGui::Text("Test!");
}

void GuiWrapper::__addBuyButton(const ImVec2 &pos)
{
    ImGui::SetCursorPos(pos);
    if (ImGui::Button("Buy Purchase", ImVec2(100, 60))) {
        /* Function */
    }
}

void GuiWrapper::__addCoinButton(const char *label, const ImVec2 &pos)
{
    ImGui::SetCursorPos(pos);
    if (ImGui::Button(label, ImVec2(90, 60))) {
        /* Function */
    }
}

/**
 *
 * Utility functions
 *
 **/

void GuiWrapper::__beginDefaultProps(const char *title)
{
    ImGui::Begin(title, nullptr,
    ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
}

void GuiWrapper::__setWindowProps(const ImVec2 &pos, const ImVec2 &size)
{
    ImGui::SetWindowPos(getVec2(pos));
    ImGui::SetWindowSize(getVec2(size));
}
