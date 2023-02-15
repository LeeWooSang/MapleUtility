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
    setWindowTitle(FixKorQString("경매장 아이템 목록"));

    ui->pushButton->setText(FixKorQString("정렬"));

    // centralwidget에 layout을 추가하고, layout에 tablewidget을 넣어주면,
    // 자동으로 윈도우 크기 변경시 tablewidget 크기도 바뀜
    mLayout = new QVBoxLayout(ui->centralwidget);
    mAuctionTable = new MyTableWidget(ui->centralwidget);

    mLayout->addWidget(mAuctionTable);

    mLayout->setContentsMargins(0, 40, 0, 0);

    mAuctionTable->clear();
    // tablewidget에서 선택을 row로 설정
    mAuctionTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    // tablewidget에서 아이템 클릭시 수정 불가능
    mAuctionTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    // 한줄만 선택 가능
    mAuctionTable->setSelectionMode(QAbstractItemView::SingleSelection);
    // 인덱스 번호 숨기기
    mAuctionTable->verticalHeader()->setVisible(false);
    // 윈도우 width에 마지막 헤더 길이 맞추기
    mAuctionTable->horizontalHeader()->setStretchLastSection(true);

    int index = 0;
    // 칼럼 추가
    QStringList columnList;
    columnList.insert(index++, FixKorQString("아이템"));
    columnList.insert(index++, FixKorQString("캐시 가격"));
    columnList.insert(index++, FixKorQString("메소 가격"));
    columnList.insert(index++, FixKorQString("손익"));
    columnList.insert(index++, FixKorQString("1억당 비율"));

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

// 정렬 버튼 클릭
void QAuctionTable::on_pushButton_clicked()
{
    std::cout << "테이블 초기화" << std::endl;
    mAuctionTable->setRowCount(0);

    std::list<AuctionItem> auctionItemList;
    for(const auto& auctionItem : GET_INSTANCE(Auction)->GetAuctionItemList())
    {
        auctionItemList.emplace_back(auctionItem.second);
    }

    auctionItemList.sort([](const AuctionItem& a, const AuctionItem& b) { return a.GetProfitAndLossOfMeso() < b.GetProfitAndLossOfMeso(); });
    std::cout << "손익 순으로 정렬 완료" << std::endl;

    for(const auto& item : auctionItemList)
    {
        AddAuctionTable(item, GET_INSTANCE(Auction)->GetRate());
    }

    std::cout << "데이터 삽입 완료" << std::endl;
}
