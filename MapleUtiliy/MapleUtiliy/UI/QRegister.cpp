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
        ShowMessageBox(this, "CashItems.json ���� �ε� ����!");
        return false;
    }

    for(rapidjson::SizeType i = 0; i < doc.Size(); ++i)
    {
        {
            std::string errorMsg = "CashItems.json ���� ���� ���� : " + std::to_string(i +1) + "��° ";

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

    setWindowTitle(FixKorQString("������ ���"));
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    // ����, ���� ũ�� ����
    setFixedWidth(mExpandInfo.BeforeSize.first);

    ui->label->setAlignment(Qt::AlignCenter);
    ui->label_2->setAlignment(Qt::AlignCenter);
    ui->label_3->setAlignment(Qt::AlignCenter);

    ui->label->setText(FixKorQString("�̸�"));
    ui->label_2->setText(FixKorQString("ĳ�� ����"));
    ui->label_3->setText(FixKorQString("�޼� ����"));

    // ���ڸ� �Է°���
    ui->lineEdit_2->setValidator(new QIntValidator);
    ui->lineEdit_3->setValidator(new QLongLongValidator);

    ui->pushButton->setText(FixKorQString("���"));

    QFont font;
    font.setPointSize(13);
    font.setBold(true);
    ui->pushButton_2->setFont(font);
    ui->pushButton_2->setText(FixKorQString("�����"));
    mExpandInfo.IsExpand = true;

    QFont font2;
    font.setPointSize(10);
    font.setBold(true);
    ui->listWidget->setFont(font);

    ui->checkBox->setText(FixKorQString("���ϸ��� ����(30%)"));

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
        QString qText = text.c_str() + FixKorQString("��");

        MyListWidgetItem* item = new MyListWidgetItem(qText);
        item->SetItemData(reinterpret_cast<void*>(key));
        ui->listWidget->addItem(item);
    }

    connect(ui->listWidget, SIGNAL(itemSelectionChanged()), this, SLOT(cashItemListPress()));
}

void QRegister::cashItemListChoice(QListWidgetItem* item)
{
    // ���õ� �������� �̸��� ĳ�� ������ ����
    void* itemData = reinterpret_cast<MyListWidgetItem*>(item)->GetItemData();
    int key = reinterpret_cast<int>(itemData);

    auto iter = mCashItemList.find(key);
    assert(iter != mCashItemList.end());

    ui->lineEdit->setText((*iter).second.Name.c_str());
    ui->lineEdit_2->setText(QString::number((*iter).second.Price));
    ui->checkBox->setChecked((*iter).second.mIsUseMileage);
}

// ��� ��ư Ŭ��
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
    // Ȯ�� ���� ��,
    if(mExpandInfo.IsExpand == false)
    {
        mExpandInfo.IsExpand = true;
        ui->pushButton_2->setText(FixKorQString("�����"));
        resize(mExpandInfo.AfterSize.first, mExpandInfo.AfterSize.second);
    }
    else
    {
        mExpandInfo.IsExpand = false;
        ui->pushButton_2->setText(FixKorQString("��õ��"));
        resize(mExpandInfo.BeforeSize.first, mExpandInfo.BeforeSize.second);
    }
}

void QRegister::cashItemListPress()
{
    cashItemListChoice(ui->listWidget->currentItem());
}


