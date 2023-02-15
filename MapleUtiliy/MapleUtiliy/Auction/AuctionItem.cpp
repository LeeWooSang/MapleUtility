#include "AuctionItem.h"

AuctionItem::AuctionItem(const Item& item, int rate, int fee, double discount)
{
    mItem = item;
    mRate = rate;
    mFee = fee;
    mDiscount = discount;
}

int AuctionItem::GetRateOfMeso() const
{
    return 100000000 / mRate * (1 - (mFee * 0.01));
}

long long AuctionItem::GetProfitAndLossOfMeso() const
{
    long long mesoPrice =  mItem.GetMesoPrice() * (1 - (mFee * 0.01));
    long long cashPrice = GetDiscountedCashPrice();
    double itemRate = GetRateOfMeso() * cashPrice;

    long long profitAndLossOfMeso = mesoPrice - static_cast<long long>(itemRate);
    return profitAndLossOfMeso;
}

long long AuctionItem::GetProfitAndLossOfCash() const
{
    return GetProfitAndLossOfMeso() / static_cast<long long>(GetRateOfMeso());
}

int AuctionItem::GetDiscountedCashPrice() const
{
    int cashPrice = mItem.GetCashPrice();
    if(mItem.GetIsUseMileage() == true)
    {
        cashPrice *= 0.7f;
    }

    return  cashPrice * (1 - (mDiscount * 0.01));
}

