#include "Balance.h"

// 충전하려는 금액, 할인율
// 게임 캐시 충전
void Balance::ChargeGameCash(int cashAmount, int discount)
{
    // 캐시를 충전하는데 필요한 현금
    mRealCash = cashAmount * (1 - (discount * 0.01));
    mGameCash = cashAmount;
}


