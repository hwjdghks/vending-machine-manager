#ifndef GUIWAPPER_HPP
#define GUIWAPPER_HPP

class GuiWapper
{
private:
    /* data */
public:
    GuiWapper(/* args */);
    ~GuiWapper();

public:
    void drawSalesWindows();
    void drawAdminWindows();
    void drawSignInWindows();
    void drawConnectToServerWindows();

private: /* Sales */
    void _addDisplayBeverage();
    void _addPaymentPanel();
    void _addConsolePanel();

private: /* Admin */
    void _addBeveragesInfo();
    void _addCurrencyInfo();

private: /* Sales utility */
    void __addBuyButton();
    void __addCoinButton();
    void __addDisplayPanel();
    void __addImage();

private: /* Admin utility */
    void __addBeverageInfo();
    void __addStockController();
    void __addCollectRevenue();

};


#endif
