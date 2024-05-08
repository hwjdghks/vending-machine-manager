#include "GuiWrapper.hpp"
#include "utils.hpp"
#include <stdio.h>

GuiWrapper::GuiWrapper() {}

GuiWrapper::~GuiWrapper() {}

ViewMode GuiWrapper::_drawMenuID;
bool GuiWrapper::_drawSignIn;

void GuiWrapper::initDrawData()
{
    GuiWrapper::_drawMenuID = ViewMode::SALES;
    GuiWrapper::_drawSignIn = false;
}

ViewMode GuiWrapper::getDrawID()
{
    return GuiWrapper::_drawMenuID;
}

void GuiWrapper::drawSalesWindows()
{
    ImVec2 winPosRatio(0.0f, 0.0f);
    ImVec2 winSizeRatio(0.75f, 1.0f);
    ImVec2 displayBeverageRatio(0.07f, 0.1f);
    ImVec2 paymentRatio(0.07f, 0.6f);
    ImVec2 signInRatio(0.55f, 0.1f);

    __beginDefaultProps("Sales Menu");
    __setWindowProps(winPosRatio, winSizeRatio);
    _addDisplayBeverage(displayBeverageRatio);
    _addPaymentPanel(paymentRatio);
    _drawSignInWindows(signInRatio);
    ImGui::End();
}

void GuiWrapper::drawAdminWindows()
{
    ImVec2 winPosRatio(0.0f, 0.0f);
    ImVec2 winSizeRatio(0.75f, 1.0f);

    __beginDefaultProps("Admin Menu");
    __setWindowProps(winPosRatio, winSizeRatio);
    if (ImGui::Button("판매 페이지 전환", ImVec2(170, 60))) {
        DebugLog::AddLog("판매 페이지 전환 버튼 클릭\n");
        _drawMenuID = ViewMode::SALES;
    }
    ImGui::End();
}

void GuiWrapper::_drawSignInWindows(const ImVec2 &start)
{
    ImGui::SetCursorPos(getVec2(start));
    if (ImGui::Button("관리자 페이지 전환", ImVec2(170, 60))) {
        DebugLog::AddLog("관리자 페이지 전환 버튼 클릭\n");
        _drawSignIn = true;
    }
    if (_drawSignIn)
    {
        ImGui::OpenPopup("Small Window");
        if (ImGui::BeginPopupModal("Small Window", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::Text("This is a small modal window!");
            if (ImGui::Button("Close")) {
                DebugLog::AddLog("팝업 닫힘\n");
                GuiWrapper::_drawSignIn = false;
                GuiWrapper::_drawMenuID = ViewMode::ADMIN;
            }
        }
        ImGui::EndPopup();
    }
}
void GuiWrapper::_addDisplayBeverage(/* each Beverage Data */ const ImVec2 &start)
{
    ImVec2 ratio;

    for (int i = 0; i < 6; i++) {
        ratio = start + ImVec2((i % 3) * 0.15f, (i / 3) * 0.22f);
        __addDisplayPanel("물건 정보", ratio);
        __addDisplayPanel("금액 정보", ratio + ImVec2(0, 0.04f));
        ImGui::PushID(i);
        __addBuyButton(i, ratio + ImVec2(0, 0.08f));
        ImGui::PopID();
    }
}

void GuiWrapper::_addPaymentPanel(const ImVec2 &start)
{
    const char *labels[] = {
        "10원",
        "50원",
        "100원",
        "500원",
        "1000원"
    };
    ImVec2 ratio;
    ImVec2 currentBalanceRatio(0.5f, 0.6f);

    ImGui::SetCursorPos(getVec2(start));
    ImGui::Text("투입할 금액");
    for (int i = 0; i < 5; i++) {
        ratio = start + ImVec2(0.0f, 0.05f) + ImVec2((i % 3) * 0.1f, (i / 3) * 0.12f);
        __addCoinButton(labels[i], ratio);
    }
    ImGui::SetCursorPos(getVec2(currentBalanceRatio));
    ImGui::Text("현재 잔액: ");
    /* Need edit */
    // 잔액 부족 시 추가 출력
    ImGui::SetCursorPos(getVec2(currentBalanceRatio + ImVec2(0.0f, 0.05f)));
    if (ImGui::Button("반환", ImVec2(150, 60))) {
        DebugLog::AddLog("반환 버튼 클릭\n");
    }
}

void GuiWrapper::__addDisplayPanel(const char *label, const ImVec2 &ratio)
{
    ImGui::SetCursorPos(getVec2(ratio));
    ImGui::Text("%s", label);
}

void GuiWrapper::__addBuyButton(int id, const ImVec2 &ratio)
{
    // 한 쌍의 begin-End 내에서 같은 식별자 보유 시 작동 안됨
    char label[256];

    sprintf(label, "구매##%d", id);
    ImGui::SetCursorPos(getVec2(ratio));
    if (ImGui::Button(label, ImVec2(100, 60))) {
        DebugLog::AddLog("구매 버튼 클릭\n");
        /* Function */
    }
}

void GuiWrapper::__addCoinButton(const char *label, const ImVec2 &ratio)
{
    ImGui::SetCursorPos(getVec2(ratio));
    if (ImGui::Button(label, ImVec2(90, 60))) {
        DebugLog::AddLog("%s 동전 버튼 클릭\n", label);
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

void GuiWrapper::__setWindowProps(const ImVec2 &posRatio, const ImVec2 &sizeRatio)
{
    ImGui::SetWindowPos(getVec2(posRatio));
    ImGui::SetWindowSize(getVec2(sizeRatio));
}
