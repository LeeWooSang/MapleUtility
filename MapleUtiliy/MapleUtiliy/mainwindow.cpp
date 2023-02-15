#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "./Common/MyWidget.h"
#include <QVBoxLayout>
#include <QHeaderView>
#include "./Common/Utility.h"
#include "./Auction/Auction.h"

#include "./Network/Network.h"

#include "direct.h"
#include <QFileDialog>
#include "./Common/JsonFormat/JsonFormat.h"
#include "./KeyManager/KeyManager.h"

#include "Common/ThreadPool/ThreadPool.h"

#include <QScreen>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mRateCheck = false;
    mFlag = false;
}

MainWindow::~MainWindow()
{
    if(mTimer != nullptr)
    {
        mTimer->stop();
        mTimer.reset();
    }

    GET_INSTANCE(Auction)->Release();

    GET_INSTANCE(Network)->Release();
    GET_INSTANCE(ThreadPool)->Release();

    delete ui;
}

bool MainWindow::Initialize()
{
    setWindowTitle(FixKorQString("MVP�� ��ƿ��Ƽ(ver1.54)"));
    //setFixedWidth(size().width());

    // �޴� ����
    QMenu* menu = menuBar()->addMenu(FixKorQString("����"));

    QAction* action1 = new QAction(FixKorQString("����"));
    action1->setShortcut(tr("Ctrl+S"));
    menu->addAction(action1);

    QAction* action2 = new QAction(FixKorQString("���� Ctrl+O,"));
    //action2->setShortcut(tr("Ctrl+O"));
    QList<QKeySequence> shortcuts;
    shortcuts.append(QKeySequence(Qt::CTRL + Qt::Key_X));
    shortcuts.append(QKeySequence(Qt::CTRL + Qt::Key_O));
    action2->setShortcuts(shortcuts);
    menu->addAction(action2);

    connect(action1, SIGNAL(triggered()), this, SLOT(saveData()));
    connect(action2, SIGNAL(triggered()), this, SLOT(loadData()));

    ui->label->setText(FixKorQString("1��� ����(���� ��)"));
    ui->label->setAlignment(Qt::AlignCenter);

    // ���ڸ� �Է°���
    ui->lineEdit->setValidator(new QIntValidator);

    ui->label_2->setText("");

    ui->pushButton->setText(FixKorQString("�߰�"));
    ui->pushButton_5->setText(FixKorQString("����"));
    ui->pushButton_2->setText(FixKorQString("����"));

    ui->pushButton_3->setText(FixKorQString("����"));

    ui->pushButton_4->setText(FixKorQString("Reset"));
    ui->pushButton_4->setEnabled(false);

    ui->pushButton_6->setText(FixKorQString("Request"));

    ui->label_3->setText(FixKorQString("ĳ�� ���� ������(%)"));
    ui->doubleSpinBox->setValue(0.);
    ui->doubleSpinBox->setMaximum(100);
    ui->doubleSpinBox->setMinimum(0);

    ui->label_4->setText(FixKorQString("������"));
    ui->label_4->setAlignment(Qt::AlignCenter);

    ui->radioButton->setText("5%");
    ui->radioButton_2->setText("3%");
    ui->radioButton->setChecked(true);

    QPixmap image("../Resource/Image/MvpRed.png");
    if(image.isNull() == true)
    {
        ShowMessageBox(this, "\"Resource/Image/MvpRed.png\" ������ �������� �ʽ��ϴ�.");
        return false;
    }
    ui->label_5->setPixmap(image);

    if(GET_INSTANCE(Auction)->Initialize() == false)
    {
        return false;
    }

    if(initializeAuctionTable() == false)
    {
        return false;
    }

    GET_INSTANCE(KeyManager)->Initialize();
    initTimer();

    return true;
}

void MainWindow::initTimer()
{
    mTimer = std::make_unique<QTimer>();
    // 60fps
    mTimer->start(16);
    connect(mTimer.get(), SIGNAL(timeout()), this, SLOT(runTimer()));
}

void MainWindow::runTimer()
{
    Update();

    if(GET_INSTANCE(KeyManager)->GetKeyState(KEY::SCROLL_LOCK) == KEY_STATE::AWAY)
    {
        std::string time = GetCurrentDateTime();
        QPixmap pixmap = QPixmap::grabWindow(this->winId());
        QString filename = "../Screenshot/" + QString(time.c_str()) + ".png";
        pixmap.save(filename, 0, 100);
    }
}

void MainWindow::Update()
{
    GET_INSTANCE(KeyManager)->Update();
}

void MainWindow::Render()
{
    show();
}

void MainWindow::AddAuctionTable(int key, const AuctionItem& auctionItem)
{
    int row = 0;
    mAuctionTable->insertRow(row);

    QTableWidgetItem* tableItem = nullptr;

    tableItem = new QTableWidgetItem(QString::number(key));
    tableItem->setTextAlignment(Qt::AlignCenter);
    mAuctionTable->setItem(row, AUCTIONTABLE_KEY, tableItem);

    tableItem = new QTableWidgetItem(QString(auctionItem.GetItemName().c_str()));
    tableItem->setTextAlignment(Qt::AlignCenter);
    //setTableItemEditFasle(tableItem);
    mAuctionTable->setItem(row, AUCTIONTABLE_ITEM_NAME, tableItem);

    // ���� ������ ����
    //tableItem = new QTableWidgetItem(QString::number(auctionItem.GetDiscountedCashPrice()));
    tableItem = new QTableWidgetItem(NumberToCommaNumber(auctionItem.GetDiscountedCashPrice()).c_str());
    tableItem->setTextAlignment(Qt::AlignCenter);
    mAuctionTable->setItem(row, AUCTIONTABLE_CASH_PRICE, tableItem);

    //tableItem = new QTableWidgetItem(QString::number(auctionItem.GetItemMesoPrice()));
    tableItem = new QTableWidgetItem(NumberToCommaNumber(auctionItem.GetItemMesoPrice()).c_str());
    tableItem->setTextAlignment(Qt::AlignCenter);
    mAuctionTable->setItem(row, AUCTIONTABLE_MESO_PRICE, tableItem);

    long long profitAndLossMeso = auctionItem.GetProfitAndLossOfMeso();
    long long profitAndLossCash = auctionItem.GetProfitAndLossOfCash();

    //tableItem = new QTableWidgetItem(QString::number(profitAndLossMeso));
    tableItem = new QTableWidgetItem(NumberToCommaNumber(profitAndLossMeso).c_str());
    tableItem->setTextAlignment(Qt::AlignCenter);
    setTableItemEditFasle(tableItem);
    profitAndLossMeso > 0 ? tableItem->setTextColor(RED_COLOR) : tableItem->setTextColor(BLUE_COLOR);
    mAuctionTable->setItem(row, AUCTIONTABLE_PROFIT_AND_LOSS_MESO, tableItem);

    //tableItem = new QTableWidgetItem(QString::number(profitAndLossCash));
    tableItem = new QTableWidgetItem(NumberToCommaNumber(profitAndLossCash).c_str());
    tableItem->setTextAlignment(Qt::AlignCenter);    
    setTableItemEditFasle(tableItem);
    profitAndLossCash > 0 ? tableItem->setTextColor(RED_COLOR) : tableItem->setTextColor(BLUE_COLOR);
    mAuctionTable->setItem(row, AUCTIONTABLE_PROFIT_AND_LOSS_CASH, tableItem);

    //tableItem = new QTableWidgetItem(QString::number(auctionItem.GetRate()));
    tableItem = new QTableWidgetItem(NumberToCommaNumber(auctionItem.GetRate()).c_str());
    tableItem->setTextAlignment(Qt::AlignCenter);
    setTableItemEditFasle(tableItem);
    mAuctionTable->setItem(row, AUCTIONTABLE_RATE, tableItem);

    tableItem = new QTableWidgetItem(QString::number(auctionItem.GetFee()));
    tableItem->setTextAlignment(Qt::AlignCenter);
    setTableItemEditFasle(tableItem);
    mAuctionTable->setItem(row, AUCTIONTABLE_FEE, tableItem);

    char isUseMileage = auctionItem.GetItemIsUseMileage();
    isUseMileage == true ? isUseMileage = 'O' : isUseMileage = 'X';

    tableItem = new QTableWidgetItem(QString(isUseMileage));
    tableItem->setTextAlignment(Qt::AlignCenter);
    setTableItemEditFasle(tableItem);
    mAuctionTable->setItem(row, AUCTIONTABLE_MILEAGE, tableItem);

    mAuctionTable->resizeColumnsToContents();
}

bool MainWindow::initializeAuctionTable()
{
    // centralwidget�� layout�� �߰��ϰ�, layout�� tablewidget�� �־��ָ�,
    // �ڵ����� ������ ũ�� ����� tablewidget ũ�⵵ �ٲ�
    mLayout = new QVBoxLayout(ui->centralwidget);
    mAuctionTable = new MyTableWidget(ui->centralwidget);

    mLayout->addWidget(mAuctionTable);

    mLayout->setContentsMargins(0, 130, 0, 0);

    mAuctionTable->clear();
    // tablewidget���� ������ row�� ����
    mAuctionTable->setSelectionBehavior(QAbstractItemView::SelectRows);

    // tablewidget���� ������ Ŭ���� ���� �Ұ���
    //mAuctionTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mAuctionTable->setEditTriggers(QAbstractItemView::DoubleClicked);

    // ���ٸ� ���� ����
    mAuctionTable->setSelectionMode(QAbstractItemView::SingleSelection);

    // �ε��� ��ȣ �����
    mAuctionTable->verticalHeader()->setVisible(false);
    // ������ width�� ������ ��� ���� ���߱�
    //mAuctionTable->horizontalHeader()->setStretchLastSection(true);

    QFont font;
    font.setPointSize(9);
    font.setBold(true);
    mAuctionTable->horizontalHeader()->setFont(font);

    int index = 0;
    // Į�� �߰�
    QStringList columnList;
    columnList.insert(index++, FixKorQString("Key"));
    columnList.insert(index++, FixKorQString("������"));
    columnList.insert(index++, FixKorQString("ĳ�� ����"));
    columnList.insert(index++, FixKorQString("�޼� ����"));
    columnList.insert(index++, FixKorQString("����(�޼�)"));
    columnList.insert(index++, FixKorQString("����(��)"));
    columnList.insert(index++, FixKorQString("1��� ����"));
    columnList.insert(index++, FixKorQString("������(%)"));
    columnList.insert(index++, FixKorQString("���ϸ��� ���� ����"));

    mAuctionTable->setColumnCount(columnList.size());
    mAuctionTable->setHorizontalHeaderLabels(columnList);
    // �Ǿ� �÷� �����
    mAuctionTable->setColumnHidden(AUCTIONTABLE_KEY, true);

    int oldWidth = mAuctionTable->columnWidth(AUCTIONTABLE_CASH_PRICE);
    mAuctionTable->setColumnWidth(AUCTIONTABLE_CASH_PRICE, oldWidth - 30);

    oldWidth = mAuctionTable->columnWidth(AUCTIONTABLE_MESO_PRICE);
    mAuctionTable->setColumnWidth(AUCTIONTABLE_MESO_PRICE, oldWidth - 15);

    oldWidth = mAuctionTable->columnWidth(AUCTIONTABLE_PROFIT_AND_LOSS_MESO);
    mAuctionTable->setColumnWidth(AUCTIONTABLE_PROFIT_AND_LOSS_MESO, oldWidth - 15);

    oldWidth = mAuctionTable->columnWidth(AUCTIONTABLE_PROFIT_AND_LOSS_CASH);
    mAuctionTable->setColumnWidth(AUCTIONTABLE_PROFIT_AND_LOSS_CASH, oldWidth - 30);

    oldWidth = mAuctionTable->columnWidth(AUCTIONTABLE_RATE);
    mAuctionTable->setColumnWidth(AUCTIONTABLE_RATE, oldWidth - 35);

    oldWidth = mAuctionTable->columnWidth(AUCTIONTABLE_FEE);
    mAuctionTable->setColumnWidth(AUCTIONTABLE_FEE, oldWidth - 37);

    oldWidth = mAuctionTable->columnWidth(AUCTIONTABLE_MILEAGE);
    mAuctionTable->setColumnWidth(AUCTIONTABLE_MILEAGE, oldWidth + 15);

    connect(mAuctionTable, SIGNAL(itemDoubleClicked(QTableWidgetItem*)), this, SLOT(auctionTableItemDoubleClick(QTableWidgetItem*)));
    connect(mAuctionTable, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(auctionTableItemChange(QTableWidgetItem*)));

    return true;
}

void MainWindow::setFirstRowColor()
{
    for(int i = 1; i < mAuctionTable->columnCount(); ++i)
    {
        mAuctionTable->item(0, i)->setBackgroundColor(YELLOW_COLOR);
    }
//    mAuctionTable->item(0, AUCTIONTABLE_ITEM_NAME)->setBackgroundColor(YELLOW_COLOR);
//    mAuctionTable->item(0, AUCTIONTABLE_CASH_PRICE)->setBackgroundColor(YELLOW_COLOR);
//    mAuctionTable->item(0, AUCTIONTABLE_MESO_PRICE)->setBackgroundColor(YELLOW_COLOR);

//    mAuctionTable->item(0, AUCTIONTABLE_PROFIT_AND_LOSS_MESO)->setBackgroundColor(YELLOW_COLOR);
//    mAuctionTable->item(0, AUCTIONTABLE_PROFIT_AND_LOSS_CASH)->setBackgroundColor(YELLOW_COLOR);

//    mAuctionTable->item(0, AUCTIONTABLE_RATE)->setBackgroundColor(YELLOW_COLOR);
//    mAuctionTable->item(0, AUCTIONTABLE_FEE)->setBackgroundColor(YELLOW_COLOR);
}

void MainWindow::setDarkMode()
{
}

void MainWindow::setTableItemEditFasle(QTableWidgetItem* item)
{
    item->setFlags(item->flags()
                        .setFlag(Qt::ItemIsEditable, false)
                   .setFlag(Qt::ItemIsSelectable, true));
}

// ������ ��� ��ư
void MainWindow::on_pushButton_clicked()
{
    if(mRateCheck == false)
    {
        ShowMessageBox(this, "1��� ������ ���� �������ּ���.");
        return;
    }

    GET_INSTANCE(Auction)->RenderRegisterUI();
}

// ���� ��ư
void MainWindow::on_pushButton_2_clicked()
{
    if(mAuctionTable->rowCount() <= 0)
    {
        ShowMessageBox(this, "����忡 ��ϵ� �������� �����ϴ�.");
        return;
    }

    // ���̺� �ʱ�ȭ
    mAuctionTable->setRowCount(0);

    std::list<std::pair<int, AuctionItem>> auctionItemList;
    for(const auto& auctionItem : GET_INSTANCE(Auction)->GetAuctionItemList())
    {
        auctionItemList.emplace_back(auctionItem.first, auctionItem.second);
    }

    // ����(�޼�) �������� ����
    using tempType = std::pair<int, AuctionItem>;
    auctionItemList.sort([](const tempType& a, const tempType& b)   { return a.second.GetProfitAndLossOfMeso() < b.second.GetProfitAndLossOfMeso(); });

    // ���̺� ����
    for(const auto& item : auctionItemList)
    {
        AddAuctionTable(item.first, item.second);
    }

    setFirstRowColor();
}

// ���� ��ư Ŭ��
void MainWindow::on_pushButton_3_clicked()
{
    try
    {
        int rate = ui->lineEdit->text().toInt();
        if(rate > 0 && mRateCheck == false)
        {
            // ���� �Ұ����ϵ���
            ui->lineEdit->setEnabled(false);

            mRateCheck = true;
            ui->pushButton_3->setEnabled(false);
            ui->pushButton_4->setEnabled(true);

            GET_INSTANCE(Auction)->SetRate(rate);
            GET_INSTANCE(Auction)->SetRateOfMeso();

            std::string labelText = "���� : " + NumberToCommaNumber(GET_INSTANCE(Auction)->GetRate()) + "�� -----�� 1���� "
                    + NumberToCommaNumber(static_cast<int>(GET_INSTANCE(Auction)->GetRateOfMeso())) + " �޼�";
            ui->label_2->setText(FixKorQString(labelText));

            // �̹� ��ϵ� �������� �ִٸ�
            if(GET_INSTANCE(Auction)->GetAuctionItemList().size() > 0)
            {
                // �ٲ� ������ ������Ʈ
                GET_INSTANCE(Auction)->UpdateAuctionItemRate();

                for(int i = 0; i < mAuctionTable->rowCount(); ++i)
                {
                    int key = mAuctionTable->item(i, AUCTIONTABLE_KEY)->text().toInt();

                    //mAuctionTable->item(i, AUCTIONTABLE_RATE)->setText(QString::number(rate));
                    mAuctionTable->item(i, AUCTIONTABLE_RATE)->setText(NumberToCommaNumber(rate).c_str());

                    // ����(�޼�) �ݿ�
                   long long profitAndLossOfMeso = GET_INSTANCE(Auction)->GetAuctionItem(key).GetProfitAndLossOfMeso();
                   //mAuctionTable->item(i, AUCTIONTABLE_PROFIT_AND_LOSS_MESO)->setText(QString::number(profitAndLossOfMeso));
                   mAuctionTable->item(i, AUCTIONTABLE_PROFIT_AND_LOSS_MESO)->setText(NumberToCommaNumber(profitAndLossOfMeso).c_str());

                    // ����(��) �ݿ�
                   long long profitAndLossOfCash = GET_INSTANCE(Auction)->GetAuctionItem(key).GetProfitAndLossOfCash();
                   //mAuctionTable->item(i, AUCTIONTABLE_PROFIT_AND_LOSS_CASH)->setText(QString::number(profitAndLossOfCash));
                   mAuctionTable->item(i, AUCTIONTABLE_PROFIT_AND_LOSS_CASH)->setText(NumberToCommaNumber(profitAndLossOfCash).c_str());
                }
            }
        }

        else if(rate <= 0)
        {
            ShowMessageBox(this, "1��� ������ ���� �Է����ּ���.");
        }
    }
    catch (std::exception& e)
    {
        //std::cout << e.what() << std::endl;
        ShowMessageBox(this, e.what());
    }
}

// ���� ��ư Ŭ��
void MainWindow::on_pushButton_4_clicked()
{
    if(ui->pushButton_4->isEnabled() == true)
    {
        // ���� �Ұ����ϵ���
        ui->lineEdit->setEnabled(true);

        mRateCheck = false;
        ui->pushButton_3->setEnabled(true);
        ui->pushButton_4->setEnabled(false);
        ui->lineEdit->setText("");
        ui->label_2->setText("");
    }
}

// ������ ����
void MainWindow::on_pushButton_5_clicked()
{
    if(mAuctionTable->rowCount() <= 0)
    {
        ShowMessageBox(this, "����忡 ��ϵ� �������� �����ϴ�.");
        return;
    }

    int selectedRow = mAuctionTable->currentRow();
    if(selectedRow == -1)
    {
        ShowMessageBox(this, "���õ� �������� �����ϴ�.");
        return;
    }

    int key = mAuctionTable->item(selectedRow, AUCTIONTABLE_KEY)->text().toInt();
    if(GET_INSTANCE(Auction)->DeleteAuctionItem(key) == true)
    {
        mAuctionTable->removeRow(selectedRow);
        //std::cout << "���õ� �� : " << mSelectedAuctionTableItem << " ����" << std::endl;
    }
}

// ����忡 ��ϵ� ������ ����
void MainWindow::auctionTableItemDoubleClick(QTableWidgetItem* item)
{
    int selectedColumn = item->column();
    if(selectedColumn == AUCTIONTABLE_CASH_PRICE || selectedColumn == AUCTIONTABLE_MESO_PRICE || selectedColumn == AUCTIONTABLE_ITEM_NAME)
    {
        mFlag = true;
    }
}

void MainWindow::auctionTableItemChange(QTableWidgetItem* item)
{
    if(mFlag == false)
    {
        return;
    }
    mFlag = false;

    int selectedRow =item->row();
    int key = mAuctionTable->item(selectedRow, AUCTIONTABLE_KEY)->text().toInt();
    std::string value = item->text().toStdString();
    switch (item->column())
    {
        case AUCTIONTABLE_ITEM_NAME:
        {
            try
            {
                // �ٲ� �̸� ������Ʈ
                GET_INSTANCE(Auction)->GetAuctionItem(key).SetItemName(value);
                mAuctionTable->item(selectedRow, AUCTIONTABLE_ITEM_NAME)->setText(item->text());
            }
            catch (std::exception& e)
            {
                ShowMessageBox(this, e.what());
            }

            break;
        }

       case AUCTIONTABLE_CASH_PRICE:
        {
            // �ٲ� ĳ�� ���� �ݿ�
            int newCashPrice = std::stoi(Split(value));
            GET_INSTANCE(Auction)->GetAuctionItem(key).SetItemCashPrice(newCashPrice);
            mAuctionTable->item(selectedRow, AUCTIONTABLE_CASH_PRICE)->setText(NumberToCommaNumber(GET_INSTANCE(Auction)->GetAuctionItem(key).GetDiscountedCashPrice()).c_str());

            // ����(�޼�) �ݿ�
           long long profitAndLossOfMeso = GET_INSTANCE(Auction)->GetAuctionItem(key).GetProfitAndLossOfMeso();
           //mAuctionTable->item(selectedRow, AUCTIONTABLE_PROFIT_AND_LOSS_MESO)->setText(QString::number(profitAndLossOfMeso));
           mAuctionTable->item(selectedRow, AUCTIONTABLE_PROFIT_AND_LOSS_MESO)->setText(NumberToCommaNumber(profitAndLossOfMeso).c_str());

           // ����(��) �ݿ�
           long long profitAndLossOfCash = GET_INSTANCE(Auction)->GetAuctionItem(key).GetProfitAndLossOfCash();
           //mAuctionTable->item(selectedRow, AUCTIONTABLE_PROFIT_AND_LOSS_CASH)->setText(QString::number(profitAndLossOfCash));
           mAuctionTable->item(selectedRow, AUCTIONTABLE_PROFIT_AND_LOSS_CASH)->setText(NumberToCommaNumber(profitAndLossOfCash).c_str());
           break;
        }

        case AUCTIONTABLE_MESO_PRICE:
        {
            // �ٲ� �޼� ���� �ݿ�
            long long newMesoPrice = std::stoll(Split(value));
            GET_INSTANCE(Auction)->GetAuctionItem(key).SetItemMesoPrice(newMesoPrice);
            mAuctionTable->item(selectedRow, AUCTIONTABLE_MESO_PRICE)->setText(NumberToCommaNumber(newMesoPrice).c_str());

             // ����(�޼�) �ݿ�
            long long profitAndLossOfMeso = GET_INSTANCE(Auction)->GetAuctionItem(key).GetProfitAndLossOfMeso();
            //mAuctionTable->item(selectedRow, AUCTIONTABLE_PROFIT_AND_LOSS_MESO)->setText(QString::number(profitAndLossOfMeso));
            mAuctionTable->item(selectedRow, AUCTIONTABLE_PROFIT_AND_LOSS_MESO)->setText(NumberToCommaNumber(profitAndLossOfMeso).c_str());

            // ����(��) �ݿ�
            long long profitAndLossOfCash = GET_INSTANCE(Auction)->GetAuctionItem(key).GetProfitAndLossOfCash();
            //mAuctionTable->item(selectedRow, AUCTIONTABLE_PROFIT_AND_LOSS_CASH)->setText(QString::number(profitAndLossOfCash));
            mAuctionTable->item(selectedRow, AUCTIONTABLE_PROFIT_AND_LOSS_CASH)->setText(NumberToCommaNumber(profitAndLossOfCash).c_str());
            break;
        }

        default:
            break;
    }
}

void MainWindow::on_doubleSpinBox_valueChanged(double discount)
{
    GET_INSTANCE(Auction)->SetRateOfDiscount(discount);
    GET_INSTANCE(Auction)->UpdateRateOfDiscount();

    int size = mAuctionTable->rowCount();
    for(int i = 0; i < size; ++i)
    {
        int key = mAuctionTable->item(i, AUCTIONTABLE_KEY)->text().toInt();

        int discountedCashPrice = GET_INSTANCE(Auction)->GetAuctionItem(key).GetDiscountedCashPrice();
        // ĳ�� ���� �缳��
        mAuctionTable->item(i, AUCTIONTABLE_CASH_PRICE)->setText(QString::number(discountedCashPrice));

        // ����(�޼�) �缳��
        long long profitAndLossOfMeso = GET_INSTANCE(Auction)->GetAuctionItem(key).GetProfitAndLossOfMeso();
        //mAuctionTable->item(i, AUCTIONTABLE_PROFIT_AND_LOSS_MESO)->setText(QString::number(profitAndLossOfMeso));
        mAuctionTable->item(i, AUCTIONTABLE_PROFIT_AND_LOSS_MESO)->setText(NumberToCommaNumber(profitAndLossOfMeso).c_str());
        profitAndLossOfMeso > 0 ? mAuctionTable->item(i, AUCTIONTABLE_PROFIT_AND_LOSS_MESO)->setTextColor(RED_COLOR) : mAuctionTable->item(i, AUCTIONTABLE_PROFIT_AND_LOSS_MESO)->setTextColor(BLUE_COLOR);

        // ����(��) �缳��
        long long profitAndLossOfCash = GET_INSTANCE(Auction)->GetAuctionItem(key).GetProfitAndLossOfCash();
        //mAuctionTable->item(i, AUCTIONTABLE_PROFIT_AND_LOSS_CASH)->setText(QString::number(profitAndLossOfCash));
        mAuctionTable->item(i, AUCTIONTABLE_PROFIT_AND_LOSS_CASH)->setText(NumberToCommaNumber(profitAndLossOfCash).c_str());
        profitAndLossOfCash > 0 ? mAuctionTable->item(i, AUCTIONTABLE_PROFIT_AND_LOSS_CASH)->setTextColor(RED_COLOR) : mAuctionTable->item(i, AUCTIONTABLE_PROFIT_AND_LOSS_CASH)->setTextColor(BLUE_COLOR);
    }
}

void MainWindow::on_radioButton_clicked()
{
    // �̹� �����ᰡ 5%�� ���õȰ��
    if(GET_INSTANCE(Auction)->GetFee() == 5 || mRateCheck == false)
    {
        return;
    }

    GET_INSTANCE(Auction)->UpdateAuctionItemListFee(5);
    GET_INSTANCE(Auction)->SetRateOfMeso();

    std::string labelText = "���� : " + NumberToCommaNumber(GET_INSTANCE(Auction)->GetRate()) + "�� -----�� 1���� "
            + NumberToCommaNumber(static_cast<int>(GET_INSTANCE(Auction)->GetRateOfMeso())) + " �޼�";
    ui->label_2->setText(FixKorQString(labelText));

    int size = mAuctionTable->rowCount();
    for(int i = 0; i < size; ++i)
    {
        int key = mAuctionTable->item(i, AUCTIONTABLE_KEY)->text().toInt();

        // ����(�޼�) �缳��
        long long profitAndLossOfMeso = GET_INSTANCE(Auction)->GetAuctionItem(key).GetProfitAndLossOfMeso();
        //mAuctionTable->item(i, AUCTIONTABLE_PROFIT_AND_LOSS_MESO)->setText(QString::number(profitAndLossOfMeso));
        mAuctionTable->item(i, AUCTIONTABLE_PROFIT_AND_LOSS_MESO)->setText(NumberToCommaNumber(profitAndLossOfMeso).c_str());
        profitAndLossOfMeso > 0 ? mAuctionTable->item(i, AUCTIONTABLE_PROFIT_AND_LOSS_MESO)->setTextColor(RED_COLOR) : mAuctionTable->item(i, AUCTIONTABLE_PROFIT_AND_LOSS_MESO)->setTextColor(BLUE_COLOR);

        // ����(��) �缳��
        long long profitAndLossOfCash = GET_INSTANCE(Auction)->GetAuctionItem(key).GetProfitAndLossOfCash();
        //mAuctionTable->item(i, AUCTIONTABLE_PROFIT_AND_LOSS_CASH)->setText(QString::number(profitAndLossOfCash));
        mAuctionTable->item(i, AUCTIONTABLE_PROFIT_AND_LOSS_CASH)->setText(NumberToCommaNumber(profitAndLossOfCash).c_str());
        profitAndLossOfCash > 0 ? mAuctionTable->item(i, AUCTIONTABLE_PROFIT_AND_LOSS_CASH)->setTextColor(RED_COLOR) : mAuctionTable->item(i, AUCTIONTABLE_PROFIT_AND_LOSS_CASH)->setTextColor(BLUE_COLOR);

        mAuctionTable->item(i, AUCTIONTABLE_FEE)->setText(QString::number(5));
    }
}

void MainWindow::on_radioButton_2_clicked()
{
    if(GET_INSTANCE(Auction)->GetFee() == 3 || mRateCheck == false)
    {
        return;
    }

    GET_INSTANCE(Auction)->UpdateAuctionItemListFee(3);
    GET_INSTANCE(Auction)->SetRateOfMeso();

    std::string labelText = "���� : " + NumberToCommaNumber(GET_INSTANCE(Auction)->GetRate()) + "�� -----�� 1���� "
            + NumberToCommaNumber(static_cast<int>(GET_INSTANCE(Auction)->GetRateOfMeso())) + " �޼�";
    ui->label_2->setText(FixKorQString(labelText));

    int size = mAuctionTable->rowCount();
    for(int i = 0; i < size; ++i)
    {
        int key = mAuctionTable->item(i, AUCTIONTABLE_KEY)->text().toInt();

        // ����(�޼�) �缳��
        long long profitAndLossOfMeso = GET_INSTANCE(Auction)->GetAuctionItem(key).GetProfitAndLossOfMeso();
        //mAuctionTable->item(i, AUCTIONTABLE_PROFIT_AND_LOSS_MESO)->setText(QString::number(profitAndLossOfMeso));
        mAuctionTable->item(i, AUCTIONTABLE_PROFIT_AND_LOSS_MESO)->setText(NumberToCommaNumber(profitAndLossOfMeso).c_str());
        profitAndLossOfMeso > 0 ? mAuctionTable->item(i, AUCTIONTABLE_PROFIT_AND_LOSS_MESO)->setTextColor(RED_COLOR) : mAuctionTable->item(i, AUCTIONTABLE_PROFIT_AND_LOSS_MESO)->setTextColor(BLUE_COLOR);

        // ����(��) �缳��
        long long profitAndLossOfCash = GET_INSTANCE(Auction)->GetAuctionItem(key).GetProfitAndLossOfCash();
        //mAuctionTable->item(i, AUCTIONTABLE_PROFIT_AND_LOSS_CASH)->setText(QString::number(profitAndLossOfCash));
        mAuctionTable->item(i, AUCTIONTABLE_PROFIT_AND_LOSS_CASH)->setText(NumberToCommaNumber(profitAndLossOfCash).c_str());
        profitAndLossOfCash > 0 ? mAuctionTable->item(i, AUCTIONTABLE_PROFIT_AND_LOSS_CASH)->setTextColor(RED_COLOR) : mAuctionTable->item(i, AUCTIONTABLE_PROFIT_AND_LOSS_CASH)->setTextColor(BLUE_COLOR);

        mAuctionTable->item(i, AUCTIONTABLE_FEE)->setText(QString::number(3));
    }
}

void MainWindow::saveData()
{
    std::string basicPath = "../Resource/CustomDatas";
    mkdir(basicPath.c_str());

    std::string fileName = QFileDialog::getSaveFileName(this, FixKorQString("���� ����"), basicPath.c_str(), "Files (*.json)").toLocal8Bit().data();
    if(fileName.length() <= 0)
    {
        return;
    }

    rapidjson::Document doc;
    doc.SetObject();

    int rate = GET_INSTANCE(Auction)->GetRate();
    int fee = GET_INSTANCE(Auction)->GetFee();
    double rateOfDiscount = GET_INSTANCE(Auction)->GetRateOfDiscount();

    doc.AddMember("rate", Value().SetInt(rate), doc.GetAllocator());
    doc.AddMember("fee", Value().SetInt(fee), doc.GetAllocator());
    doc.AddMember("rateOfDiscount", Value().SetDouble(rateOfDiscount), doc.GetAllocator());

    rapidjson::Value arrayData(rapidjson::kArrayType);
    for(const auto& auctionItem : GET_INSTANCE(Auction)->GetAuctionItemList())
    {
        rapidjson::Value data;
        data.SetObject();

        std::string name = auctionItem.second.GetItemName();
        int cashPrice = auctionItem.second.GetItemCashPrice();
        long long mesoPrice = auctionItem.second.GetItemMesoPrice();
        bool isUseMileage = auctionItem.second.GetItemIsUseMileage();

        data.AddMember("name", Value().SetString(name.c_str(), doc.GetAllocator()), doc.GetAllocator());
        data.AddMember("cashPrice", Value().SetInt(cashPrice), doc.GetAllocator());
        data.AddMember("mesoPrice", Value().SetInt64(mesoPrice), doc.GetAllocator());
        data.AddMember("isUseMileage", Value().SetBool(isUseMileage), doc.GetAllocator());
        arrayData.PushBack(data, doc.GetAllocator());
    }
    doc.AddMember("auctionItems", arrayData, doc.GetAllocator());

    GET_INSTANCE(JsonFormat)->SaveJsonFile(doc, fileName);
}

void MainWindow::loadData()
{
    std::string basicPath = "../Resource/CustomDatas";
    mkdir(basicPath.c_str());

    std::string fileName = QFileDialog::getOpenFileName(this, FixKorQString("���� ����"), basicPath.c_str(), "Files (*.json)").toLocal8Bit().data();
    if(fileName.length() <= 0)
    {
        return;
    }

    rapidjson::Document doc;
    if(GET_INSTANCE(JsonFormat)->LoadJsonFile(doc, fileName) == false)
    {
        ShowMessageBox(this, "���� �ε� ����!");
        return;
    }

    // ���� ������ ��� ����
    if(mAuctionTable->rowCount() > 0)
    {
        if(ShowQuestionMessageBox(this, "�̹� ����忡 ��ϵ� �������� �ֽ��ϴ�.\n���� �������� ��� �����˴ϴ�.\n������ �ε� �Ͻðڽ��ϱ�?") == QMessageBox::Yes)
        {
            // ���̺� �ʱ�ȭ
            GET_INSTANCE(Auction)->ClearAuction();
            mAuctionTable->setRowCount(0);
        }
        else
        {
            return;
        }
    }

    int rate = doc["rate"].GetInt();
    if(rate <= 0)
    {
        rate = 0;
    }

    int fee = doc["fee"].GetInt();
    double rateOfDiscount = doc["rateOfDiscount"].GetDouble();
    if(rateOfDiscount <= 0)
    {
        rateOfDiscount = 0;
    }

    // ���� ����
    ui->lineEdit->setText(QString::number(rate));
    on_pushButton_3_clicked();

    // ������ ����
    if(fee == 3)
    {
        ui->radioButton_2->click();
    }
    else
    {
        ui->radioButton->click();
    }

    // ĳ�� ���� ������ ����
    ui->doubleSpinBox->setValue(rateOfDiscount);

    // ������ ����
    for(rapidjson::SizeType i = 0; i < doc["auctionItems"].Size(); ++i)
    {
        std::string name = doc["auctionItems"][i]["name"].GetString();
        long long cashPrice = doc["auctionItems"][i]["cashPrice"].GetInt64();
        long long mesoPrice = doc["auctionItems"][i]["mesoPrice"].GetInt64();
        bool isUseMileage = doc["auctionItems"][i]["isUseMileage"].GetBool();

        if(name.length() > 0 && cashPrice > 0 && mesoPrice > 0)
        {
            GET_INSTANCE(Auction)->AddItem(name, cashPrice, mesoPrice, isUseMileage);
        }
    }
}

void MainWindow::on_pushButton_6_clicked()
{
    GET_INSTANCE(ThreadPool)->AddJob([](){ GET_INSTANCE(Network)->HttpRequest(""); });
    //GET_INSTANCE(Network)->HttpRequest("");
}
