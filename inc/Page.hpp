#ifndef PAGE_HPP
#define PAGE_HPP

#include "imgui.h"
#include "utils.hpp"
#include "DebugLog.hpp"
#include "Program.hpp"

class Page
{
private:
    /* data */
    static ViewMode _drawMenuID;

public:
    Page(/* args */);
    ~Page();

public:
    static void initDrawData();
    static ViewMode getDrawID();
    static void drawSalesWindows(Program &program);
    static void drawAdminWindows(Program &program);
    void drawConnectToServerWindows();

private: /* Sales */
    static void _addDisplayBeverage(Program &program, const ImVec2 &start);
    static void _addPaymentPanel(Program &program, const ImVec2 &start);
    static void _drawSignInWindows(Program &program, const ImVec2 &start);

private: /* Admin */
    static void _addBeveragesInfo(Program &program, const ImVec2 &start);
    static void _addCurrencyInfo(Program &program, const ImVec2 &start);
    static void _addChangePasswd(Program &program, const ImVec2 &start);

private: /* Sales utility */
    static void __addDisplayPanel(const char *label, const ImVec2 &ratio);
    static void __addBuyButton(VendingMachine &machine, Shelf &rack, const ImVec2 &ratio);
    static void __addCoinButton(VendingMachine &machine, CashTray &tray, const ImVec2 &ratio);

private: /* Admin utility */
    static void __addBeverageInfo(Shelf &rack, const ImVec2 &ratio);
    static void __addStockController(Shelf &rack, const ImVec2 &ratio);

private: /* Utility */
    static void __beginDefaultProps(const char *title);
    static void __setWindowProps(const ImVec2 &posRatio, const ImVec2 &sizeRatio);
};


#endif
