#pragma once
#include <QMainWindow>
#include <QTimer>
#include "Auction/Auction.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

constexpr int AUCTIONTABLE_KEY = 0;
constexpr int AUCTIONTABLE_ITEM_NAME = 1;
constexpr int AUCTIONTABLE_CASH_PRICE = 2;
constexpr int AUCTIONTABLE_MESO_PRICE = 3;
constexpr int AUCTIONTABLE_PROFIT_AND_LOSS_MESO = 4;
constexpr int AUCTIONTABLE_PROFIT_AND_LOSS_CASH = 5;
constexpr int AUCTIONTABLE_RATE = 6;
constexpr int AUCTIONTABLE_FEE = 7;
constexpr int AUCTIONTABLE_MILEAGE = 8;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    bool Initialize();
    void Update();
    void Render();

    void AddAuctionTable(int key, const AuctionItem& auctionItem);

private:
    bool initializeAuctionTable();
    void setFirstRowColor();
    void setDarkMode();
    void setTableItemEditFasle(class QTableWidgetItem* item);
    void initTimer();

private slots:
    void runTimer();

    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked();

    void auctionTableItemDoubleClick(class QTableWidgetItem* item);
    void auctionTableItemChange(class QTableWidgetItem* item);

    void on_doubleSpinBox_valueChanged(double arg1);

    void on_radioButton_clicked();
    void on_radioButton_2_clicked();

    void loadData();
    void saveData();

    void on_pushButton_6_clicked();

private:
    Ui::MainWindow* ui;
    bool mRateCheck;

    class QVBoxLayout* mLayout;
    class MyTableWidget* mAuctionTable;

    bool mFlag;

   std::unique_ptr<QTimer>mTimer;
};
