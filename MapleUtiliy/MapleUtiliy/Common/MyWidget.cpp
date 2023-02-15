#include "MyWidget.h"
#include <QMouseEvent>
#include <iostream>

MyTableWidget::MyTableWidget(QWidget* parent)
    : QTableWidget(parent)
{
    mRowDataList.clear();

    setMouseTracking(true);
}

MyTableWidget::~MyTableWidget()
{
    mRowDataList.clear();
}

void MyTableWidget::InsertFirstRowAndData(void* data)
{
    int row = 0;
    // 기존 row에 데이터가 있었다면
    if(mRowDataList.count(row) != 0)
    {
        // 바뀌는 row에 대해 키를 바꿈
        row = rowCount() - 1;
        for(int i = row; i >= 0; --i)
        {
            void* data = mRowDataList[i];
            mRowDataList.erase(i);
            mRowDataList.emplace(i + 1, data);
        }

        row = 0;
    }

    insertRow(row);
    mRowDataList.emplace(row, data);
}

void* MyTableWidget::FindRowData(int row)
{
    auto iter = mRowDataList.find(row);
    if(iter == mRowDataList.end())
    {
        return nullptr;
    }

    return (*iter).second;
}

void MyTableWidget::ClearRow()
{
    setRowCount(0);
    mRowDataList.clear();
}

/////////////////////////////////////////////////////////////////////////////////////

MyTableWidgetItem::MyTableWidgetItem(const QString& text, int type)
    : QTableWidgetItem(text, type)
{
}

MyTableWidgetItem::~MyTableWidgetItem()
{
}

/////////////////////////////////////////////////////////////////////////////////////

MyTreeWidget::MyTreeWidget(QWidget* parent)
    : QTreeWidget(parent)
{
    mParentList.clear();
}

MyTreeWidget::~MyTreeWidget()
{
    ClearTree();
}

void MyTreeWidget::AddParent(const std::string& key, QTreeWidgetItem* item)
{
    MyTree* myTree = new MyTree;
    myTree->ParentItem = item;
    mParentList.emplace(key, myTree);
}

QTreeWidgetItem* MyTreeWidget::FindParentItem(const std::string& key)
{
    auto iter = mParentList.find(key);
    if(iter == mParentList.end())
    {
        return nullptr;
    }

    return (*iter).second->ParentItem;
}

MyTree* MyTreeWidget::FindParentMyTree(const std::string& key)
{
    auto iter = mParentList.find(key);
    if(iter == mParentList.end())
    {
        return nullptr;
    }

    return (*iter).second;
}

MyTreeData* MyTreeWidget::FindChildTreeData(const std::string& parentKey, const std::string& childKey)
{
    auto iter = mParentList.find(parentKey);
    if(iter == mParentList.end())
    {
        return nullptr;
    }

    auto iter2 = (*iter).second->ChildList.find(childKey);
    if(iter2 == (*iter).second->ChildList.end())
    {
        return nullptr;
    }

    return (*iter2).second;
}

void MyTreeWidget::ClearTree()
{
    clear();

    for(auto iter = mParentList.begin(); iter != mParentList.end(); )
    {
        if((*iter).second != nullptr)
        {
            for(auto iter2 = (*iter).second->ChildList.begin(); iter2 != (*iter).second->ChildList.end(); )
            {
                if((*iter2).second != nullptr)
                {
                    delete (*iter2).second;
                }
                iter2 = (*iter).second->ChildList.erase(iter2);
            }
            (*iter).second->ChildList.clear();

            delete (*iter).second;
        }
        iter = mParentList.erase(iter);
    }
    mParentList.clear();
}

MyListWidgetItem::MyListWidgetItem(const QString& text, QListWidget* listview, int type)
    : QListWidgetItem(text, listview, type)
{
}

MyListWidgetItem::~MyListWidgetItem()
{

}
