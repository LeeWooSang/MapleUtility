#pragma once

#include <QMainWindow>
#include "../Auction/Auction.h"

namespace Ui {
class QAuctionTable;
}

class QAuctionTable : public QMainWindow
{
    Q_OBJECT

public:
    explicit QAuctionTable(QWidget *parent = nullptr);
    ~QAuctionTable();
    bool Initialize();
    void Render();

    void AddAuctionTable(const AuctionItem& item, int rate);

private slots:
    void on_pushButton_clicked();

private:
    Ui::QAuctionTable *ui;

    class QVBoxLayout* mLayout;
     class MyTableWidget* mAuctionTable;
};

