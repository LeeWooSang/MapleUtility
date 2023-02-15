#include "QAuctionTable.h"
#include "ui_QAuctionTable.h"
#include "../Common/Utility.h"
#include "../Common/MyTableWidget.h"
#include <QVBoxLayout>
#include <QHeaderView>
#include <list>

QAuctionTable::QAuctionTable(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QAuctionTable)
{
    ui->setupUi(this);
}

QAuctionTable::~QAuctionTable()
{
    delete ui;
}

bool QAuctionTable::Initialize()
{
    setWindowTitle(FixKorQString("����� ������ ���"));

    ui->pushButton->setText(FixKorQString("����"));

    // centralwidget�� layout�� �߰��ϰ�, layout�� tablewidget�� �־��ָ�,
    // �ڵ����� ������ ũ�� ����� tablewidget ũ�⵵ �ٲ�
    mLayout = new QVBoxLayout(ui->centralwidget);
    mAuctionTable = new MyTableWidget(ui->centralwidget);

    mLayout->addWidget(mAuctionTable);

    mLayout->setContentsMargins(0, 40, 0, 0);

    mAuctionTable->clear();
    // tablewidget���� ������ row�� ����
    mAuctionTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    // tablewidget���� ������ Ŭ���� ���� �Ұ���
    mAuctionTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    // ���ٸ� ���� ����
    mAuctionTable->setSelectionMode(QAbstractItemView::SingleSelection);
    // �ε��� ��ȣ �����
    mAuctionTable->verticalHeader()->setVisible(false);
    // ������ width�� ������ ��� ���� ���߱�
    mAuctionTable->horizontalHeader()->setStretchLastSection(true);

    int index = 0;
    // Į�� �߰�
    QStringList columnList;
    columnList.insert(index++, FixKorQString("������"));
    columnList.insert(index++, FixKorQString("ĳ�� ����"));
    columnList.insert(index++, FixKorQString("�޼� ����"));
    columnList.insert(index++, FixKorQString("����"));
    columnList.insert(index++, FixKorQString("1��� ����"));

    mAuctionTable->setColumnCount(columnList.size());
    mAuctionTable->setHorizontalHeaderLabels(columnList);

    return true;
}

void QAuctionTable::Render()
{
    show();
}

void QAuctionTable::AddAuctionTable(const AuctionItem& auctionItem, int rate)
{
    int row = 0;
    mAuctionTable->insertRow(row);

    QTableWidgetItem* tableItem = nullptr;
    tableItem = new QTableWidgetItem(QString(auctionItem.GetItemName().c_str()));
    tableItem->setTextAlignment(Qt::AlignCenter);
    mAuctionTable->setItem(row, 0, tableItem);

    tableItem = new QTableWidgetItem(QString::number(auctionItem.GetItemCashPrice()));
    tableItem->setTextAlignment(Qt::AlignCenter);
    mAuctionTable->setItem(row, 1, tableItem);

    tableItem = new QTableWidgetItem(QString::number(auctionItem.GetItemMesoPrice()));
    tableItem->setTextAlignment(Qt::AlignCenter);
    mAuctionTable->setItem(row, 2, tableItem);

    tableItem = new QTableWidgetItem(QString::number(auctionItem.GetProfitAndLossOfMeso()));
    tableItem->setTextAlignment(Qt::AlignCenter);
    mAuctionTable->setItem(row, 3, tableItem);

    tableItem = new QTableWidgetItem(std::to_string(rate).c_str());
    tableItem->setTextAlignment(Qt::AlignCenter);
    mAuctionTable->setItem(row, 4, tableItem);
}

// ���� ��ư Ŭ��
void QAuctionTable::on_pushButton_clicked()
{
    std::cout << "���̺� �ʱ�ȭ" << std::endl;
    mAuctionTable->setRowCount(0);

    std::list<AuctionItem> auctionItemList;
    for(const auto& auctionItem : GET_INSTANCE(Auction)->GetAuctionItemList())
    {
        auctionItemList.emplace_back(auctionItem.second);
    }

    auctionItemList.sort([](const AuctionItem& a, const AuctionItem& b) { return a.GetProfitAndLossOfMeso() < b.GetProfitAndLossOfMeso(); });
    std::cout << "���� ������ ���� �Ϸ�" << std::endl;

    for(const auto& item : auctionItemList)
    {
        AddAuctionTable(item, GET_INSTANCE(Auction)->GetRate());
    }

    std::cout << "������ ���� �Ϸ�" << std::endl;
}
