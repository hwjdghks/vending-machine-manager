#include "Page.hpp"
#include "utils.hpp"
#include <stdio.h>

Page::Page() {}

Page::~Page() {}

ViewMode Page::_drawMenuID;
bool Page::_drawSignIn;

void Page::initDrawData()
{
    Page::_drawMenuID = ViewMode::SALES;
    Page::_drawSignIn = false;
}

ViewMode Page::getDrawID()
{
    return Page::_drawMenuID;
}

void Page::drawSalesWindows(VendingMachine &machine)
{
    ImVec2 winPosRatio(0.0f, 0.0f);
    ImVec2 winSizeRatio(0.75f, 1.0f);
    ImVec2 displayBeverageRatio(0.07f, 0.1f);
    ImVec2 paymentRatio(0.07f, 0.6f);
    ImVec2 signInRatio(0.55f, 0.1f);

    __beginDefaultProps("Sales Menu");
    __setWindowProps(winPosRatio, winSizeRatio);
    _addDisplayBeverage(machine, displayBeverageRatio);
    _addPaymentPanel(machine, paymentRatio);
    _drawSignInWindows(signInRatio);
    ImGui::End();
}

void Page::drawAdminWindows()
{
    ImVec2 winPosRatio(0.0f, 0.0f);
    ImVec2 winSizeRatio(0.75f, 1.0f);

    __beginDefaultProps("Admin Menu");
    __setWindowProps(winPosRatio, winSizeRatio);
    if (ImGui::Button("판매 페이지 전환", ImVec2(170, 60))) {
        DebugLog::AddLog("판매 페이지 전환 버튼 클릭");
        _drawMenuID = ViewMode::SALES;
    }
    ImGui::End();
}

void Page::_drawSignInWindows(const ImVec2 &start)
{
    ImGui::SetCursorPos(getVec2(start));
    if (ImGui::Button("관리자 페이지 전환", ImVec2(170, 60))) {
        DebugLog::AddLog("관리자 페이지 전환 버튼 클릭");
        _drawSignIn = true;
    }
    if (_drawSignIn)
    {
        ImGui::OpenPopup("Small Window");
        if (ImGui::BeginPopupModal("Small Window", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::Text("This is a small modal window!");
            if (ImGui::Button("Close")) {
                DebugLog::AddLog("팝업 닫힘");
                Page::_drawSignIn = false;
                Page::_drawMenuID = ViewMode::ADMIN;
            }
        }
        ImGui::EndPopup();
    }
}
void Page::_addDisplayBeverage(VendingMachine &machine, const ImVec2 &start)
{
    ImVec2 ratio;

    for (int i = 0; i < 6; i++) {
        Shelf &rack = machine.getRack(i);
        std::string menuLabel("상품명: " + rack.getLabel());
        std::string priceLabel("가격: " + std::to_string(rack.getPrice()));

        ratio = start + ImVec2((i % 3) * 0.15f, (i / 3) * 0.22f);
        __addDisplayPanel(menuLabel.c_str(), ratio);
        __addDisplayPanel(priceLabel.c_str(), ratio + ImVec2(0, 0.04f));
        __addBuyButton(machine, rack, ratio + ImVec2(0, 0.08f));
    }
}

void Page::_addPaymentPanel(VendingMachine &machine, const ImVec2 &start)
{
    ImVec2 ratio;
    ImVec2 currentBalanceRatio(0.5f, 0.6f);

    ImGui::SetCursorPos(getVec2(start));
    ImGui::Text("투입할 금액");
    for (int i = 0; i < 5; i++) {
        CashTray &tray = machine.getCashBox(i);
        ratio = start + ImVec2(0.0f, 0.05f) + ImVec2((i % 3) * 0.1f, (i / 3) * 0.12f);
        __addCoinButton(machine, tray, ratio);
    }
    ImGui::SetCursorPos(getVec2(currentBalanceRatio));
    ImGui::Text("현재 잔액: ");
    ImGui::SameLine();
    ImGui::Text("%d원", machine.getBalance());
    /* Need edit */
    // 잔액 부족 시 추가 출력
    ImGui::SetCursorPos(getVec2(currentBalanceRatio + ImVec2(0.0f, 0.05f)));
    if (ImGui::Button("반환", ImVec2(150, 60))) {
        DebugLog::AddLog("반환 버튼 클릭");
    }
}

void Page::__addDisplayPanel(const char *label, const ImVec2 &ratio)
{
    ImGui::SetCursorPos(getVec2(ratio));
    ImGui::Text("%s", label);
}

void Page::__addBuyButton(VendingMachine &machine, Shelf &rack, const ImVec2 &ratio)
{
    
    std::string label("구매##" + std::to_string(rack.getID()));

    ImGui::SetCursorPos(getVec2(ratio)); // 버튼 위치 지정
    ImGui::PushID(rack.getID()); // 버튼 구별을 위한 식별자 추가
    if (ImGui::Button(label.c_str(), ImVec2(100, 60))) {
        DebugLog::AddLog("%s 구매 버튼 클릭", rack.getLabelCstring());
        try
        {
            printf("%s 구매버튼 클릭\n", rack.getLabelCstring());
            machine.sell(rack.getID()); // 물품 판매. 재고 없을 시 에러 발생
        }
        catch (const std::logic_error& e)
        {
            printf("에러 발생! %s\n", e.what());
            DebugLog::AddLog("%s", e.what());
        }
        DebugLog::AddLog("%s 남은 수량 : %ld", rack.getLabelCstring(), rack.getAmount());
    }
    ImGui::PopID(); // 식별자 해제
}

void Page::__addCoinButton(VendingMachine &machine, CashTray &tray, const ImVec2 &ratio)
{
    std::string label(std::to_string(tray.getLabel())+ "원");

    ImGui::SetCursorPos(getVec2(ratio));
    if (ImGui::Button(label.c_str(), ImVec2(90, 60))) {
        DebugLog::AddLog("%s 동전 버튼 클릭", label.c_str());
        /* Function */
        machine.deposit(tray.getID());
    }
}

/**
 *
 * Utility functions
 *
 **/

void Page::__beginDefaultProps(const char *title)
{
    ImGui::Begin(title, nullptr,
    ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
}

void Page::__setWindowProps(const ImVec2 &posRatio, const ImVec2 &sizeRatio)
{
    ImGui::SetWindowPos(getVec2(posRatio));
    ImGui::SetWindowSize(getVec2(sizeRatio));
}
