#include "Item.h"
#include <iostream>

Item::Item()
{
    mName = "";
    mCashPrice = 0;
    mMesoPrice = 0;
}

Item::Item(const std::string& name, int cash, long long meso, bool isUseMileage)
{
    mName = name;
    mCashPrice = cash;
    mMesoPrice = meso;
    mIsUseMileage = isUseMileage;
}
