#pragma once

#include <string>
class Item
{
public:
    Item();
    Item(const std::string& name, int cash, long long meso, bool isUseMileage = false);

    const std::string& GetName()    const { return mName; }
    void SetName(const std::string& name) { mName = name; }

    int GetCashPrice() const { return mCashPrice; }
    void SetCashPrice(int cash) { mCashPrice = cash; }

    long long GetMesoPrice() const { return mMesoPrice; }
    void SetMesoPrice(long long meso) { mMesoPrice = meso; }

    bool GetIsUseMileage()  const { return mIsUseMileage; }
private:
    std::string mName;
    int mCashPrice;
    long long mMesoPrice;
    bool mIsUseMileage;
};

