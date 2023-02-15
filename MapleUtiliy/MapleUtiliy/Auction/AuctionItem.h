#pragma once
#include "../Item/Item.h"

class AuctionItem
{
public:
    AuctionItem(const Item& item, int rate, int fee, double discount);

    const std::string& GetItemName()    const { return mItem.GetName(); }
    void SetItemName(const std::string& name) { mItem.SetName(name); }

    long long GetItemMesoPrice()  const { return mItem.GetMesoPrice(); }
    void SetItemMesoPrice(long long mesoPrice) { mItem.SetMesoPrice(mesoPrice); }

    int GetItemCashPrice() const { return mItem.GetCashPrice(); }
    void SetItemCashPrice(int cash) { mItem.SetCashPrice(cash); }

    bool GetItemIsUseMileage()  const {return mItem.GetIsUseMileage();}

    int GetRate()   const { return mRate; }
    void SetRate(int rate) { mRate = rate; }

    int GetFee()    const { return mFee; }
    void SetFee(int fee) { mFee = fee; }

    double GetDiscount()    const { return mDiscount; }
    void SetDiscount(double discount) { mDiscount = discount; }

    int GetRateOfMeso() const;

    long long GetProfitAndLossOfMeso()   const;
    long long GetProfitAndLossOfCash() const;

    int GetDiscountedCashPrice()    const;

private:
    Item mItem;
    int mRate;
    int mFee;
    double mDiscount;
};

