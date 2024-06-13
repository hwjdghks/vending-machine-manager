#include "Page.hpp"
#include "utils.hpp"
#include <stdio.h>

Page::Page() {}

Page::~Page() {}

ViewMode Page::_drawMenuID;

/*
 * static 데이터 초기화
 */
void Page::initDrawData()
{
    Page::_drawMenuID = ViewMode::ADMIN;
}

/*
 * 현재 그리는 창의 ID를 반환
 */
ViewMode Page::getDrawID()
{
    return Page::_drawMenuID;
}

/*
 * 판매 페이지 창을 그리는 함수
 */
void Page::drawSalesWindows(Program &program)
{
    ImVec2 winPosRatio(0.0f, 0.0f);
    ImVec2 winSizeRatio(0.70f, 1.0f);
    ImVec2 displayBeverageRatio(0.05f, 0.1f);
    ImVec2 paymentRatio(0.05f, 0.6f);
    ImVec2 signInRatio(0.55f, 0.1f);

    // 창 시작시 기본 옵션 지정
    __beginDefaultProps("Sales Menu");
    __setWindowProps(winPosRatio, winSizeRatio);
    // 상품 UI
    _addDisplayBeverage(program, displayBeverageRatio);
    // 화폐 관련 UI
    _addPaymentPanel(program, paymentRatio);
    // 관리자 페이지 전환을 위한 UI
    _drawSignInWindows(program, signInRatio);
    // 창의 끝
    ImGui::End();
}

/*
 * 관리자 페이지 창을 그리는 함수
 */
void Page::drawAdminWindows(Program &program)
{
    ImVec2 winPosRatio(0.0f, 0.0f);
    ImVec2 winSizeRatio(0.70f, 1.0f);
    ImVec2 beverageInfoRatio(0.05f, 0.1f);
    ImVec2 currencyinfoRatio(0.05f, 0.63f);
    ImVec2 returnSalesRatio(0.55f, 0.1f);
    ImVec2 changePasswdRatio(0.55f, 0.2f);
    ImVec2 changeInfoRatio(0.55f, 0.3f);
    ImVec2 connectServerRatio(0.55f, 0.4f);
    // 창 시작시 기본 옵션 지정
    __beginDefaultProps("Admin Menu");
    __setWindowProps(winPosRatio, winSizeRatio);

    _addBeveragesInfo(program, beverageInfoRatio);
    _addCurrencyInfo(program, currencyinfoRatio);
    _addChangePasswd(program, changePasswdRatio);
    _addChangeProductInfo(program, changeInfoRatio);
    _addconnectServer(program, connectServerRatio);
    ImGui::SetCursorPos(getVec2(returnSalesRatio));
    if (ImGui::Button("판매 페이지 전환", BASIC_BUTTON_SIZE)) {
        DebugLog::AddLog("판매 페이지 전환 버튼 클릭");
        _drawMenuID = ViewMode::SALES;
    }
    ImGui::End();
}

/*
 * 음료의 정보를 출력하는 단위 함수
 * 상품명, 가격, 구매버튼을 출력한다.
 */
void Page::_addDisplayBeverage(Program &program, const ImVec2 &start)
{
    Client &client = program.getClient();
    VendingMachine &machine = program.getMachine();
    ImVec2 ratio;

    for (int i = 0; i < 6; i++) {
        Shelf &rack = machine.getRack(i);
        std::string menuLabel("상품명: " + rack.getLabel());
        std::string priceLabel("가격: " + std::to_string(rack.getPrice()));

        ratio = start + ImVec2((i % 3) * 0.15f, (i / 3) * 0.22f);
        // 상품명 추가
        __addDisplayPanel(menuLabel.c_str(), ratio);
        // 가격 추가
        __addDisplayPanel(priceLabel.c_str(), ratio + ImVec2(0, 0.04f));
        // 구매버튼 추가
        __addBuyButton(client, machine, rack, ratio + ImVec2(0, 0.08f));
    }
}

/*
 * 금액 계산을 위한 UI 생성 함수
 * 화폐 버튼, 반환 버튼 등 버튼 추가
 * 투입된 금액, 잔액 부족 시 텍스트 알림
 */
void Page::_addPaymentPanel(Program &program, const ImVec2 &start)
{
    VendingMachine &machine = program.getMachine();
    ImVec2 ratio;
    ImVec2 currentBalanceRatio(0.5f, 0.6f);

    // 화폐 버튼 추가
    ImGui::SetCursorPos(getVec2(start));
    ImGui::Text("투입할 금액");
    for (int i = 0; i < 5; i++) {
        CashTray &tray = machine.getCashBox(i);
        ratio = start + ImVec2(0.0f, 0.05f) + ImVec2((i % 3) * 0.1f, (i / 3) * 0.12f);
        __addCoinButton(machine, tray, ratio);
    }
    // 잔액 UI 그리기
    ImGui::SetCursorPos(getVec2(currentBalanceRatio));
    ImGui::Text("현재 잔액: ");
    ImGui::SameLine();
    ImGui::Text("%d원", machine.getBalance());
    // 거스름돈 부족시 텍스트 추가
    if (machine.getAlert()) {
        ImGui::SetCursorPos(getVec2(currentBalanceRatio + ImVec2(0, -0.05f)));
        ImGui::Text("거스름돈 부족");
    }
    // 반환 버튼 추가
    ImGui::SetCursorPos(getVec2(currentBalanceRatio + ImVec2(0.0f, 0.05f)));
    if (ImGui::Button("반환", ImVec2(150, 60))) {
        DebugLog::AddLog("반환 버튼 클릭");
        if (machine.getBalance() > 0) {
            try
            {
                // 반환 함수 호출
                machine.returnChange();
            }
            catch (const std::logic_error &e)
            {
                DebugLog::AddLog("%s", e.what());\
                // 서버로 알람 전송
                Client &client = program.getClient();
                client.addToWrite("ALERT BALANCE");
            }
        }
    }
}

/*
 * 관리자 페이지로 넘어가기 위한 UI를 그리는 함수
 */
void Page::_drawSignInWindows(Program &program, const ImVec2 &start)
{
    static bool signInFlag = false;
    static bool wrongPassFlag = false;
    static char inputPass[100]; // 비밀번호 입력용 버퍼

    // 버퍼 초기화
    if (signInFlag == false)
        memset(inputPass, 0, sizeof(inputPass));
    // 판매 화면에 페이지 전환 버튼 추가
    ImGui::SetCursorPos(getVec2(start));
    if (ImGui::Button("관리자 페이지 전환", BASIC_BUTTON_SIZE)) {
        DebugLog::AddLog("관리자 페이지 전환 버튼 클릭");
        signInFlag = true;
    }
    // 버튼 클릭하지 않으면 여기서 그리기 종료
    if (signInFlag == false)
        return ;

    // 로그인 팝업 창 구현
    ImGui::SetNextWindowSize(ImVec2(300, 150));
    ImGui::OpenPopup("관리자 페이지 로그인");
    if (ImGui::BeginPopupModal("관리자 페이지 로그인", nullptr, ImGuiWindowFlags_NoResize)) {
        ImGui::Text("Password: ");
        ImGui::SameLine();
        // 비밀번호 입력 폼 생성
        // 엔터키 입력 시 true 반환
        bool enter = ImGui::InputText("##password", inputPass, IM_ARRAYSIZE(inputPass),
                ImGuiInputTextFlags_Password
                | ImGuiInputTextFlags_CharsNoBlank
                | ImGuiInputTextFlags_EnterReturnsTrue);
        // 닫힘 버튼 추가
        ImGui::SetCursorPos(ImVec2(100, 100));
        if (ImGui::Button("Close##2", ImVec2(70, 30)))
        {
            DebugLog::AddLog("로그인 팝업 닫힘");
            signInFlag = false;
            wrongPassFlag = false;
        }
        // 로그인 버튼 추가
        ImGui::SetCursorPos(ImVec2(200, 100));
        if (enter || ImGui::Button("로그인##1", ImVec2(70, 30))) {
            DebugLog::AddLog("로그인 버튼 클릭");
            DebugLog::AddLog("입력한 비밀번호: %s", inputPass);
            // 비밀번호 일치 시 관리자 페이지로 전환
            if (program.equalPassword(inputPass)) {
                wrongPassFlag = false;
                signInFlag = false;
                Page::_drawMenuID = ViewMode::ADMIN;
            }
            else {
                wrongPassFlag = true;
            }
        }
        // 비밀번호 틀릴 시 창이 닫힐때까지 메세지 출력
        if (wrongPassFlag) {
            ImGui::SetCursorPos(ImVec2(5, 70));
            ImGui::Text("잘못된 비밀번호");
        }
    }
    // 팝업 그리기 종료
    ImGui::EndPopup();
}

void Page::_addBeveragesInfo(Program &program, const ImVec2 &start)
{
    VendingMachine &machine = program.getMachine();
    ImVec2 ratio;

    for (int i = 0; i < 6; i++) {
        Shelf &rack = machine.getRack(i);
        ratio = start + ImVec2((i % 3) * 0.15f, (i / 3) * 0.25f);
        __addBeverageInfo(rack, ratio);
        __addStockController(rack, ratio + ImVec2(0, 0.12f));
    }
}

void Page::_addCurrencyInfo(Program &program, const ImVec2 &start)
{
    VendingMachine &machine = program.getMachine();
    ImVec2 ratio;

    for (int i = 0; i < 5; i++) {
        CashTray &tray = machine.getCashBox(i);
        std::string label(std::to_string(tray.getLabel()) + "원 재고: "
                        + std::to_string(tray.getAmount()) + "개");
        ratio = start + ImVec2(0, i * 0.04f);
        __addDisplayPanel(label.c_str(), ratio);
    }
    ImGui::SetCursorPos(getVec2(start + ImVec2(0, 0.22f)));
    if (ImGui::Button("수금", BASIC_BUTTON_SIZE)) {
        DebugLog::AddLog("수금 버튼 클릭");
        machine.settlement();
    }
}

void Page::_addChangePasswd(Program &program, const ImVec2 &start)
{
    static bool changePwFlag = false;
    static bool wrongPassFlag[4] = { false, false, false, false };
    static bool press = false;

    static char currentPw[100];
    static char newPw[100];
    static char confirmPw[100];

    if (changePwFlag == false) {
        std::memset(currentPw, 0, sizeof(currentPw));
        std::memset(newPw, 0, sizeof(newPw));
        std::memset(confirmPw, 0, sizeof(confirmPw));
    }

    ImGui::SetCursorPos(getVec2(start));
    if (ImGui::Button("비밀번호 변경", BASIC_BUTTON_SIZE)) {
        changePwFlag = true;
    }
    if (changePwFlag == false)
        return ;

    ImGui::SetNextWindowSize(ImVec2(400, 205));
    ImGui::OpenPopup("비밀번호 변경");
    if (ImGui::BeginPopupModal("비밀번호 변경", nullptr, ImGuiWindowFlags_NoResize)) {
        ImGui::Text("현재 비밀번호: ");
        ImGui::SameLine();

        // 비밀번호 입력 폼 생성
        // 엔터키 입력 시 true 반환
        ImGui::InputText("##current", currentPw, IM_ARRAYSIZE(currentPw),
                ImGuiInputTextFlags_CharsNoBlank
                | ImGuiInputTextFlags_EnterReturnsTrue);
        ImGui::Text("현재 비밀번호: ");
        ImGui::SameLine();

        // 비밀번호 입력 폼 생성
        // 엔터키 입력 시 true 반환
        ImGui::InputText("##change", newPw, IM_ARRAYSIZE(newPw),
                ImGuiInputTextFlags_CharsNoBlank
                | ImGuiInputTextFlags_EnterReturnsTrue);
        ImGui::Text("현재 비밀번호: ");
        ImGui::SameLine();

        // 비밀번호 입력 폼 생성
        // 엔터키 입력 시 true 반환
        ImGui::InputText("##confirm", confirmPw, IM_ARRAYSIZE(confirmPw),
                ImGuiInputTextFlags_CharsNoBlank
                | ImGuiInputTextFlags_EnterReturnsTrue);
        // 닫힘 버튼 추가
        ImGui::SetCursorPos(ImVec2(200, 155));
        if (ImGui::Button("Close##2", ImVec2(70, 30)))
        {
            DebugLog::AddLog("비밀번호 변경 팝업 닫힘");
            changePwFlag = false;
            press = false;
            wrongPassFlag[0] = wrongPassFlag[1] = false;
        }
        // 비밀번호 변경 버튼 추가
        ImGui::SetCursorPos(ImVec2(300, 155));
        if (ImGui::Button("변경##1", ImVec2(70, 30))) {
            press = true;
            if (!program.equalPassword(currentPw))
                wrongPassFlag[0] = true;
            else
                wrongPassFlag[0] = false;
            if (!program.validatePassword(newPw))
                wrongPassFlag[1] = true;
            else
                wrongPassFlag[1] = false;
            if (std::string(newPw) != std::string(confirmPw))
                wrongPassFlag[2] = true;
            else
                wrongPassFlag[2] = false;
            if (program.getPassword() == std::string(newPw))
                wrongPassFlag[3] = true;
            else {
                wrongPassFlag[3] = false;
                try
                {
                    program.changePassword(newPw);
                }
                catch (const std::exception &e)
                {
                    DebugLog::AddLog("비밀번호 불일치");
                    // 비밀번호 규칙에 어긋나므로 2번 플래그 활성화
                    wrongPassFlag[2] = true;
                }
            }
        }
        // 비밀번호 틀릴 시 창이 닫힐때까지 메세지 출력
        ImGui::SetCursorPos(ImVec2(10, 130));
        if (wrongPassFlag[0] == true) {
            ImGui::Text("현재 비밀번호가 잘못 되었습니다");
        }
        else if (wrongPassFlag[1] == true) {
            ImGui::Text("비밀번호 규칙을 지켜야 합니다");
        }
        else if (wrongPassFlag[2] == true) {
            ImGui::Text("비밀번호를 다시 확인하세요");
        }
        else if (wrongPassFlag[3] == true) {
            ImGui::Text("이전 비밀번호와 달라야 합니다");
        }
        else if (press == true) {
            ImGui::Text("비밀번호 변경 완료");
        }
    }
    // 팝업 그리기 종료
    ImGui::EndPopup();
}

void Page::_addChangeProductInfo(Program &program, const ImVec2 &start)
{
    VendingMachine &machine = program.getMachine();
    Client &client = program.getClient();
    // 버튼 클릭 여부
    static bool changeInfoFlag = false;
    // 변경 시도
    static bool tryFlag = false;
    // 변경 성공여부
    static bool failedFlag = false;

    static int rackID = -1;
    static char labelBuf[1024];
    static int priceBuf;

    if (changeInfoFlag == false) {
        std::memset(labelBuf, 0, sizeof(labelBuf));
        priceBuf = 0;
    }
    ImGui::SetCursorPos(getVec2(start));
    if (ImGui::Button("상품 정보 변경", BASIC_BUTTON_SIZE)) {
        changeInfoFlag = true;
    }
    if (changeInfoFlag == false)
        return ;
    ImGui::OpenPopup("상품 정보 변경");
    ImGui::SetNextWindowSize(ImVec2(500, 350));
    if (ImGui::BeginPopupModal("상품 정보 변경", nullptr, ImGuiWindowFlags_NoResize)) {
        // 상품 진열대 별 동작 구현
        for (int i = 0; i < 6; i++) {
            Shelf &rack = machine.getRack(i);
            std::string label("슬롯 " + std::to_string(i + 1));
            // 슬롯별 위치 지정
            ImVec2 pos(30 + (i % 2) * 100, 50 + (i / 2) * 100);
            ImGui::SetCursorPos(pos);
            // 슬롯별 버튼 생성
            if (ImGui::Button(label.c_str(), ImVec2(70, 70))) {
                rackID = i;
                tryFlag = false;
                failedFlag = false;
                // 누른 슬롯의 정보로 초기화
                std::strcpy(labelBuf, rack.getLabelCstring());
                priceBuf = rack.getPrice();
            }
        }
        // 클릭한 진열대의 기존 정보 변경
        ImGui::SetCursorPos(ImVec2(230, 50));
        ImGui::Text("상품 정보 변경");
        if (rackID > -1) {
            Shelf &rack = machine.getRack(rackID);

            // 상품명 출력및 수정칸. 공백 삽입 불가
            ImGui::SetCursorPos(ImVec2(230, 95));
            ImGui::Text("상품명: ");
            ImGui::SameLine();
            ImGui::InputText("inputLabel", labelBuf, IM_ARRAYSIZE(labelBuf),
                    ImGuiInputTextFlags_CharsNoBlank);
            // 상품 가격 출력및 수정칸. int 범위의 정수만 입력 가능
            ImGui::SetCursorPos(ImVec2(230, 135));
            ImGui::Text("   가격: ");
            ImGui::SameLine();
            ImGui::InputInt("inputPrice", &priceBuf, sizeof(&priceBuf));
            // 변경사항 적용 결과 출력
            ImGui::SetCursorPos(ImVec2(230, 170));
            if (tryFlag == true && failedFlag == true) {
                ImGui::Text("형식이 잘못 되었습니다.\n입력 값을 다시 확인하세요.");
            }
            else if (tryFlag == true) {
                ImGui::Text("변경사항 적용 성공");
            }
            // 변경사항 적용 시도
            ImGui::SetCursorPos(ImVec2(300, 220));
            if (ImGui::Button("변경사항 적용", ImVec2(120, 40))) {
                tryFlag = true;
                // 잘못된 형식일 경우 원래 정보로 되돌린다
                std::string backupLabel(rack.getLabel());
                int backupPrice = rack.getPrice();
                try
                {
                    // 변경사항 적용 시도
                    // 상품명 변경
                    rack.setLabel(labelBuf);
                    // 가격 변경
                    rack.setPrice(priceBuf);
                    // 플래그 변경
                    failedFlag = false;
                }
                catch(const std::logic_error& e)
                {
                    // 적용 실패시 백업본으로 되돌림
                    DebugLog::AddLog("%s", e.what());
                    failedFlag = true;
                    rack.setLabel(backupLabel);
                    rack.setPrice(backupPrice);
                }
                // 서버로 로그 전송
                std::string msg;
                msg = concatenate("UPDATE LABEL", ' ', labelBuf, '\n');
                client.addToWrite(msg);
                msg = concatenate("UPDATE PRICE", ' ', priceBuf, '\n');
                client.addToWrite(msg);
            }
        }
        // 닫기 버튼
        ImGui::SetCursorPos(ImVec2(400, 300));
        if (ImGui::Button("Close", ImVec2(70, 30))) {
            DebugLog::AddLog("상품 정보 변경 팝업 닫힘");
            changeInfoFlag = false;
            failedFlag = false;
            tryFlag = false;
            rackID = -1;
        }
    }
    ImGui::EndPopup();
}

/*
 * 관리 서버에 연결을 시도하는 함수
 * 연결에 성공하면 연결 종료 버튼으로 바뀐다
 */
void Page::_addconnectServer(Program &program, const ImVec2 &start)
{
    Client &client = program.getClient();
    ImGui::SetCursorPos(getVec2(start));
    // 서버와 연결되지 않았을 때
    if (!client.isConnected()) {
        if (ImGui::Button("서버 연결", BASIC_BUTTON_SIZE)) {
            DebugLog::AddLog("서버 연결 버튼 클릭");
            program.getClient().tryConnect(); // 연결 시도
        }
    }
    // 서버와 연결 중일 때
    else {
        if (ImGui::Button("서버 연결 종료", BASIC_BUTTON_SIZE)) {
            DebugLog::AddLog("서버 연결 종료 버튼 클릭");
            program.getClient().closeConnect(); // 연결 종료
        }
    }
}

/*
 * 단순 텍스트 출력 함수
 */
void Page::__addDisplayPanel(const char *label, const ImVec2 &ratio)
{
    ImGui::SetCursorPos(getVec2(ratio));
    ImGui::Text("%s", label);
}

/*
 * 구매 버튼 추가 함수
 */
void Page::__addBuyButton(Client &client, VendingMachine &machine, Shelf &rack, const ImVec2 &ratio)
{
    // 버튼 위치 지정
    ImGui::SetCursorPos(getVec2(ratio));
    // 예외 상황시 구매 버튼 스킵
    if (rack.getAmount() == 0) {
        ImGui::Button("매진", ImVec2(100, 60));
        return ;
    }
    else if (machine.getBalance() < rack.getPrice()) {
        ImGui::Button("구매 불가", ImVec2(100, 60));
        return ;
    }
    // 상품을 구매 가능한 상황일 경우
    std::string label("구매##" + std::to_string(rack.getID()));

    // 버튼 구별을 위한 식별자 추가
    ImGui::PushID(rack.getID());
    // 구매 버튼 추가
    if (ImGui::Button(label.c_str(), ImVec2(100, 60))) {
        DebugLog::AddLog("%s 구매 버튼 클릭", rack.getLabelCstring());
        try
        {
            // 물품 판매. 재고 없을 시 에러 발생
            machine.sell(rack.getID());
            // 서버로 로그 전송
            std::string msg = concatenate( "SELL", ' ', rack.getID(), '\n');
            client.addToWrite(msg);
        }
        catch (const std::logic_error& e)
        {
            DebugLog::AddLog("%s", e.what());
        }
        DebugLog::AddLog("%s 남은 수량 : %ld", rack.getLabelCstring(), rack.getAmount());
    }
    ImGui::PopID(); // 식별자 해제
}

/*
 * 화폐 버튼을 추가하는 단위 함수
 * 10, 50, 100, 500, 1000원 버튼을 추가
 * 클릭 시 투입 화폐를 증가한다
 */
void Page::__addCoinButton(VendingMachine &machine, CashTray &tray, const ImVec2 &ratio)
{
    std::string label(std::to_string(tray.getLabel())+ "원");

    ImGui::SetCursorPos(getVec2(ratio));
    if (ImGui::Button(label.c_str(), ImVec2(90, 60))) {
        DebugLog::AddLog("%s 동전 버튼 클릭", label.c_str());
        // 금액 상한 검사
        if (tray.getLabel() + machine.getBalance()> 7000) {
            DebugLog::AddLog("금액 상한 도달");
        }
        // 1000원 지폐 투입 개수 검사
        else if (tray.getID() == 4 && machine.getMax1000won() >= 5) {
            DebugLog::AddLog("1000원 지폐는 최대 5장까지 투입 가능");
        }
        else {
            // 클릭한 화폐를 증가
            machine.deposit(tray.getID());
        }
    }
}

void Page::__addBeverageInfo(Shelf &rack, const ImVec2 &ratio)
{
    std::string menuLabel("상품명: " + rack.getLabel());
    std::string priceLabel("가격: " + std::to_string(rack.getPrice()));
    std::string amountLabel("재고: " + std::to_string(rack.getAmount()));

    // 상품명 추가
    __addDisplayPanel(menuLabel.c_str(), ratio);
    // 가격 추가
    __addDisplayPanel(priceLabel.c_str(), ratio + ImVec2(0, 0.04f));
    // 재고 수량 추가
    __addDisplayPanel(amountLabel.c_str(), ratio + ImVec2(0, 0.08f));
}

void Page::__addStockController(Shelf &rack, const ImVec2 &ratio)
{
    static int stock[6] = { 0 };

    int id = rack.getID();
    std::string label(std::to_string(stock[id]) + "개 보충##" + std::to_string(id));
    std::string minusLabel("-##" + std::to_string(id));
    std::string plusLabel("+##" + std::to_string(id));

    ImGui::SetCursorPos(getVec2(ratio));
    if (ImGui::Button(minusLabel.c_str(), ImVec2(30, 70))) {
        DebugLog::AddLog("%s 감소 버튼 클릭", rack.getLabelCstring());
        if (stock[id] > 0)
            stock[id]--;
        else
            DebugLog::AddLog("더 이상 뺄 수 없습니다");
    }
    ImGui::SameLine();
    if (ImGui::Button(label.c_str(), ImVec2(100, 70))) {
        rack.refill(stock[id]);
        DebugLog::AddLog("%s %d개 보충", rack.getLabelCstring(), stock[id]);
        stock[id] = 0;
    }
    ImGui::SameLine();
    if (ImGui::Button(plusLabel.c_str(), ImVec2(30, 70))) {
        DebugLog::AddLog("%s 증가 버튼 클릭", rack.getLabelCstring());
        if (stock[id] + rack.getAmount() < INT32_MAX)
            stock[id]++;
        else
            DebugLog::AddLog("더 이상 추가할 수 없습니다");
    }
}

/**
 *
 * Utility functions
 *
 **/

/*
 * 창 생성시 기본적으로 적용할 옵션 적용
 * 사이즈 조절 불가, 창 이동 불가, 창 줄이기 불가
 * 프로그램 창이 아닌 프로그램 내부의 창을 의미
 */
void Page::__beginDefaultProps(const char *title)
{
    ImGui::Begin(title, nullptr,
    ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
}

/*
 * 창의 사이즈와 위치를 프로그램 창의 비율 기반으로 지정
 * 프로그램 창의 크기가 조절되어도 UI가 비율에 따라 조정됨
 */
void Page::__setWindowProps(const ImVec2 &posRatio, const ImVec2 &sizeRatio)
{
    ImGui::SetWindowPos(getVec2(posRatio));
    ImGui::SetWindowSize(getVec2(sizeRatio));
}
