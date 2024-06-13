#ifndef PAGE_HPP
#define PAGE_HPP

#include "imgui.h"
#include "utils.hpp"
#include "DebugLog.hpp"
#include "Program.hpp"

/*
 * 자판기 GUI 구현을 위한 그리기 함수 집합 클래스
 * 판매 화면, 관리 화면
 */
class Page
{
private:
    static ViewMode _drawMenuID;

public:
    Page();
    ~Page();

public: /* 그리기 함수 */
    static void initDrawData(); // 초기 화면 지정
    static ViewMode getDrawID(); // 현재 그리는 화면의 ID
    static void drawSalesWindows(Program &program); // 판매 화면 그리기 (Sales)
    static void drawAdminWindows(Program &program); // 관리 화면 그리기 (Admin)

private: /* Sales */
    static void _addDisplayBeverage(Program &program, const ImVec2 &start);
    static void _addPaymentPanel(Program &program, const ImVec2 &start);
    static void _drawSignInWindows(Program &program, const ImVec2 &start);

private: /* Admin */
    static void _addBeveragesInfo(Program &program, const ImVec2 &start);
    static void _addCurrencyInfo(Program &program, const ImVec2 &start);
    static void _addChangePasswd(Program &program, const ImVec2 &start);
    static void _addChangeProductInfo(Program &program, const ImVec2 &start);
    static void _addconnectServer(Program &program, const ImVec2 &start);

private: /* Sales utility */
    static void __addDisplayPanel(const char *label, const ImVec2 &ratio);
    static void __addBuyButton(Client &client, VendingMachine &machine, Shelf &rack, const ImVec2 &ratio);
    static void __addCoinButton(VendingMachine &machine, CashTray &tray, const ImVec2 &ratio);

private: /* Admin utility */
    static void __addBeverageInfo(Shelf &rack, const ImVec2 &ratio);
    static void __addStockController(Shelf &rack, const ImVec2 &ratio);

private: /* Utility */
    static void __beginDefaultProps(const char *title);
    static void __setWindowProps(const ImVec2 &posRatio, const ImVec2 &sizeRatio);
};


#endif
