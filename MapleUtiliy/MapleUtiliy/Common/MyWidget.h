#pragma once
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QListWidget>
#include <QListWidgetItem>
#include <map>

class MyTableWidget : public QTableWidget
{
public:
    explicit MyTableWidget(QWidget* parent = nullptr);
    virtual ~MyTableWidget();
    void InsertFirstRowAndData(void* data);
    void* FindRowData(int row);
    void ClearRow();

    const std::map<int, void*>& GetRowDataList()    const { return mRowDataList; }

private:
    std::map<int, void*> mRowDataList;
};

class MyTableWidgetItem : public QTableWidgetItem
{
public:
    explicit MyTableWidgetItem(const QString &text, int type = Type);
    virtual ~MyTableWidgetItem();

    void* GetItemData()   { return mItemData;}
    void SetItemData(void* itemData) { mItemData = itemData; }

private:
    void* mItemData;
};

struct MyTreeData
{
    MyTreeData()
    {
        ChildItem = nullptr;
        ChildData = nullptr;
    }
    QTreeWidgetItem* ChildItem;
    void* ChildData;
};

struct MyTree
{
    MyTree()
    {
        ParentItem = nullptr;
        ChildList.clear();
    }

    QTreeWidgetItem* ParentItem;
    std::map<std::string, MyTreeData*> ChildList;
};

class MyTreeWidget : public QTreeWidget
{
public:
    explicit MyTreeWidget(QWidget *parent = nullptr);
    virtual ~MyTreeWidget();

    void AddParent(const std::string& key, QTreeWidgetItem* item);

    QTreeWidgetItem* FindParentItem(const std::string& key);
    MyTree* FindParentMyTree(const std::string& key);

    MyTreeData* FindChildTreeData(const std::string& parentKey, const std::string& childKey);

    void ClearTree();    

    const std::map<std::string, MyTree*>& GetParentList()   const {return mParentList;}
private:
    //std::map<std::string, QTreeWidgetItem*> mParentItemList;
    std::map<std::string, MyTree*> mParentList;
};

class MyListWidgetItem : public QListWidgetItem
{
public:
    explicit MyListWidgetItem(const QString& text, QListWidget* listview = nullptr, int type = Type);
    virtual ~MyListWidgetItem();

    void* GetItemData() { return mItemData; }
    void SetItemData(void* data) { mItemData = data; }

private:
    void* mItemData;
};
