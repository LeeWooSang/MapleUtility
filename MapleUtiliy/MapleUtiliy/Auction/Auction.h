#pragma once
#include "../Common/Macro.h"
#include <unordered_map>
#include "AuctionItem.h"

class Auction
{
    SINGLE_TONE(Auction);

public:
    bool Initialize();

    void AddItem(const std::string& name, int cash, long long meso, bool isUseMileage);

    int GetFee()    const { return mFee; }
    void SetFee(int fee) { mFee = fee; }

    int GetRate()   const { return mRate; }
    void SetRate(int rate) { mRate = rate; }

    double GetRateOfMeso()  const { return mRateOfMeso; }
    void SetRateOfMeso();

    double GetRateOfDiscount() const { return mRateOfDiscount; }
    void SetRateOfDiscount(double discount) { mRateOfDiscount = discount; }

    void UpdateAuctionItemRate();
    void UpdateAuctionItemListFee(int fee);
    void UpdateRateOfDiscount();

    void RenderRegisterUI();

    const std::unordered_map<int, AuctionItem>& GetAuctionItemList() const {return mAuctionItemList;}
    AuctionItem& GetAuctionItem(int key);

    bool DeleteAuctionItem(int key);

    void ClearAuction();

private:

private:
    class QRegister* mRegister;

    int mFee;
    int mRate;
    double mRateOfMeso;
    double mRateOfDiscount;

    std::unordered_map<int, AuctionItem> mAuctionItemList;
    static int mAuctionItemListKey;
};

inline long long GetProfitAndLossOfMeso(long long mesoPrice, int rate, int fee, int discountedCashPrice);
inline int GetProfitAndLossOfCash();
