#include "Balance.h"

// �����Ϸ��� �ݾ�, ������
// ���� ĳ�� ����
void Balance::ChargeGameCash(int cashAmount, int discount)
{
    // ĳ�ø� �����ϴµ� �ʿ��� ����
    mRealCash = cashAmount * (1 - (discount * 0.01));
    mGameCash = cashAmount;
}


