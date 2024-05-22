#ifndef GUIWRAPPER_HPP
#define GUIWRAPPER_HPP

#include "imgui.h"
#include "utils.hpp"
#include "DebugLog.hpp"
#include "VendingMachine.hpp"

class GuiWrapper
{
private:
    /* data */
    static ViewMode _drawMenuID;
    static bool _drawSignIn;

public:
    GuiWrapper(/* args */);
    ~GuiWrapper();

public:
    static void initDrawData();
    static ViewMode getDrawID();
    static void drawSalesWindows(VendingMachine &machine);
    static void drawAdminWindows();
    void drawConnectToServerWindows();

private: /* Sales */
    static void _addDisplayBeverage(VendingMachine &machine, const ImVec2 &start);
    static void _addPaymentPanel(const ImVec2 &start);
    static void _drawSignInWindows(const ImVec2 &start);

private: /* Admin */
    void _addBeveragesInfo();
    void _addCurrencyInfo();

private: /* Sales utility */
    static void __addBuyButton(Slot &slot, const ImVec2 &ratio);
    static void __addCoinButton(const char *label, const ImVec2 &ratio);
    static void __addDisplayPanel(const char *label, const ImVec2 &ratio);
    void __addImage();

private: /* Admin utility */
    void __addBeverageInfo();
    void __addStockController();
    void __addCollectRevenue();

private: /* Utility */
    static void __beginDefaultProps(const char *title);
    static void __setWindowProps(const ImVec2 &posRatio, const ImVec2 &sizeRatio);
};


#endif
