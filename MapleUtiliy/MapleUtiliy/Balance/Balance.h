#pragma once

class Balance
{
public:
    void ChargeGameCash(int cashAmount, int discount);

private:
    int mRealCash;
    int mGameCash;
};

