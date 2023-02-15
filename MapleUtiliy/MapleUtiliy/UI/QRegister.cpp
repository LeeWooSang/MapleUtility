#include "QRegister.h"
#include "ui_QRegister.h"
#include "../Common/Utility.h"
#include "../Common/LongLongValidator.h"
#include "../Auction/Auction.h"
#include "../Common/JsonFormat/JsonFormat.h"
#include "../Common/MyWidget.h"

QRegister::QRegister(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QRegister)
{
    ui->setupUi(this);

    mExpandInfo.BeforeSize.first = 400;
    mExpandInfo.BeforeSize.second = 230;
    mExpandInfo.AfterSize.first = 400;
    mExpandInfo.AfterSize.second = mExpandInfo.BeforeSize.second + 360;
    resize(mExpandInfo.AfterSize.first, mExpandInfo.AfterSize.second);
}

QRegister::~QRegister()
{
    delete ui;
}

bool QRegister::Initialize()
{
    rapidjson::Document doc;
    if(GET_INSTANCE(JsonFormat)->LoadCashItems(doc) == false)
    {
        ShowMessageBox(this, "CashItems.json 파일 로드 실패!");
        return false;
    }

    for(rapidjson::SizeType i = 0; i < doc.Size(); ++i)
    {
        {
            std::string errorMsg = "CashItems.json 파일 수정 실패 : " + std::to_string(i +1) + "번째 ";

            if(doc[i].HasMember("name") == false)
            {
                errorMsg += "\'name\' is not found";
                ShowMessageBox(this, errorMsg);
                return false;
            }
            else if(doc[i].HasMember("price") == false)
            {
                errorMsg += "\'price\' is not found";
                ShowMessageBox(this, errorMsg);
                return false;
            }
            else if(doc[i].HasMember("isUseMileage") == false)
            {
                errorMsg += "\'isUseMileage\' is not found";
                ShowMessageBox(this, errorMsg);
                return false;
            }
        }


        std::string name = doc[i]["name"].GetString();
        int price = doc[i]["price"].GetInt();
        bool isUseMileage = doc[i]["isUseMileage"].GetBool();

        mCashItemList.emplace(i, CashItems(name, price, isUseMileage));
    }
    addCashItemList();

    setWindowTitle(FixKorQString("아이템 등록"));
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    // 가로, 세로 크기 고정
    setFixedWidth(mExpandInfo.BeforeSize.first);

    ui->label->setAlignment(Qt::AlignCenter);
    ui->label_2->setAlignment(Qt::AlignCenter);
    ui->label_3->setAlignment(Qt::AlignCenter);

    ui->label->setText(FixKorQString("이름"));
    ui->label_2->setText(FixKorQString("캐시 가격"));
    ui->label_3->setText(FixKorQString("메소 가격"));

    // 숫자만 입력가능
    ui->lineEdit_2->setValidator(new QIntValidator);
    ui->lineEdit_3->setValidator(new QLongLongValidator);

    ui->pushButton->setText(FixKorQString("등록"));

    QFont font;
    font.setPointSize(13);
    font.setBold(true);
    ui->pushButton_2->setFont(font);
    ui->pushButton_2->setText(FixKorQString("접기▲"));
    mExpandInfo.IsExpand = true;

    QFont font2;
    font.setPointSize(10);
    font.setBold(true);
    ui->listWidget->setFont(font);

    ui->checkBox->setText(FixKorQString("마일리지 할인(30%)"));

    return true;
}

void QRegister::Render()
{
    raise();
    show();
}

void QRegister::addCashItemList()
{
    for(const auto& cashItem : mCashItemList)
    {
        int key = cashItem.first;
        std::string text = std::to_string(cashItem.first + 1) + ". " + cashItem.second.Name + " : " + NumberToCommaNumber(cashItem.second.Price);
        QString qText = text.c_str() + FixKorQString("원");

        MyListWidgetItem* item = new MyListWidgetItem(qText);
        item->SetItemData(reinterpret_cast<void*>(key));
        ui->listWidget->addItem(item);
    }

    connect(ui->listWidget, SIGNAL(itemSelectionChanged()), this, SLOT(cashItemListPress()));
}

void QRegister::cashItemListChoice(QListWidgetItem* item)
{
    // 선택된 아이템의 이름과 캐시 가격을 셋팅
    void* itemData = reinterpret_cast<MyListWidgetItem*>(item)->GetItemData();
    int key = reinterpret_cast<int>(itemData);

    auto iter = mCashItemList.find(key);
    assert(iter != mCashItemList.end());

    ui->lineEdit->setText((*iter).second.Name.c_str());
    ui->lineEdit_2->setText(QString::number((*iter).second.Price));
    ui->checkBox->setChecked((*iter).second.mIsUseMileage);
}

// 등록 버튼 클릭
void QRegister::on_pushButton_clicked()
{
    std::string name = ui->lineEdit->text().toStdString();
    long long cashPrice = ui->lineEdit_2->text().toInt();
    long long mesoPrice = ui->lineEdit_3->text().toLongLong();
    bool isUseMileage = ui->checkBox->isChecked();

    if(name.length() > 0 && cashPrice > 0 && mesoPrice > 0)
    {
        GET_INSTANCE(Auction)->AddItem(name, cashPrice, mesoPrice, isUseMileage);
    }

    close();
}

void QRegister::on_pushButton_2_clicked()
{
    // 확장 전일 때,
    if(mExpandInfo.IsExpand == false)
    {
        mExpandInfo.IsExpand = true;
        ui->pushButton_2->setText(FixKorQString("접기▲"));
        resize(mExpandInfo.AfterSize.first, mExpandInfo.AfterSize.second);
    }
    else
    {
        mExpandInfo.IsExpand = false;
        ui->pushButton_2->setText(FixKorQString("추천▼"));
        resize(mExpandInfo.BeforeSize.first, mExpandInfo.BeforeSize.second);
    }
}

void QRegister::cashItemListPress()
{
    cashItemListChoice(ui->listWidget->currentItem());
}


