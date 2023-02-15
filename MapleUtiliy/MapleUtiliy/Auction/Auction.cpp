#include "Auction.h"
#include "Auction.h"
#include "../Common/Utility.h"
#include "AuctionItem.h"
#include "./UI/QRegister.h"
#include "./UI/QAuctionTable.h"
#include "../WindowManager/WindowManager.h"

INIT_INSTACNE(Auction)
int Auction::mAuctionItemListKey = 0;
Auction::Auction()
{
    mRegister = nullptr;

    mRate = 0;
    mRateOfMeso = 0.;
    mRateOfDiscount = 0.;

    mAuctionItemList.clear();
}

Auction::~Auction()
{
    if(mRegister != nullptr)
    {
        delete mRegister;
    }

    mAuctionItemList.clear();
}

bool Auction::Initialize()
{   
    mFee = 5;

    mRegister = new QRegister(&GET_INSTANCE(WindowManager)->GetMainWindow());
    if(mRegister->Initialize() == false)
    {
        return false;
    }

    return true;
}

void Auction::AddItem(const std::string& name, int cash, long long meso, bool isUseMileage)
{ 
    AuctionItem auctionItem(Item(name, cash, meso, isUseMileage), mRate, mFee, mRateOfDiscount);

    mAuctionItemList.emplace(mAuctionItemListKey, auctionItem);
    GET_INSTANCE(WindowManager)->GetMainWindow().AddAuctionTable(mAuctionItemListKey, auctionItem);

    ++mAuctionItemListKey;
}

void Auction::SetRateOfMeso()
{
    mRateOfMeso = 100000000 / mRate * (1 - (mFee * 0.01));
}

void Auction::UpdateAuctionItemRate()
{
    for(auto& auctionItem : mAuctionItemList)
    {
        auctionItem.second.SetRate(mRate);
    }
}

void Auction::UpdateAuctionItemListFee(int fee)
{
    mFee = fee;

    // 바뀐 수수료 반영
    for(auto& auctionItem : mAuctionItemList)
    {
        auctionItem.second.SetFee(fee);
    }
}

void Auction::UpdateRateOfDiscount()
{
    for(auto& auctionItem : mAuctionItemList)
    {
        auctionItem.second.SetDiscount(mRateOfDiscount);
    }
}

void Auction::RenderRegisterUI()
{
    mRegister->Render();
}

AuctionItem& Auction::GetAuctionItem(int key)
{
    auto iter = mAuctionItemList.find(key);
    assert(iter != mAuctionItemList.end());

    return (*iter).second;
}

bool Auction::DeleteAuctionItem(int key)
{
    if(mAuctionItemList.count(key) == 0)
    {
        return false;
    }

    mAuctionItemList.erase(key);
    return true;
}

void Auction::ClearAuction()
{
    mAuctionItemList.clear();
    mAuctionItemListKey = 0;
}
