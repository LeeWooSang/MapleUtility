#pragma once
#include <QDialog>
#include <unordered_map>

namespace Ui {class QRegister;}

struct ExpandWindowInfo
{
    ExpandWindowInfo()
    {
        IsExpand = false;
        BeforeSize.first = 0;
        BeforeSize.second = 0;
        AfterSize.first = 0;
        AfterSize.second = 0;
    }

    bool IsExpand;
    // width, height
    std::pair<int, int> BeforeSize;
    std::pair<int, int> AfterSize;
};

struct CashItems
{
    CashItems(const std::string& name, int price, bool isUseMileage)
        : Name(name), Price(price), mIsUseMileage(isUseMileage) {}

    std::string Name;
    int Price;
    bool mIsUseMileage;
};

class QRegister : public QDialog
{
    Q_OBJECT

public:
    explicit QRegister(QWidget *parent = nullptr);
    ~QRegister();

    bool Initialize();
    void Render();

private:
    void addCashItemList();
    void cashItemListChoice(class QListWidgetItem* item);

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void cashItemListPress();
private:
    Ui::QRegister *ui;
    ExpandWindowInfo mExpandInfo;
    std::unordered_map<int, CashItems> mCashItemList;
};

