#include "KeyManager.h"
#include <Windows.h>
//#include <WinUser.h>
#include <iostream>

int gWindowKeys[(int)KEY::LAST_KEY] =
{
        VK_SCROLL
//	VK_LEFT, VK_RIGHT, VK_UP, VK_DOWN,
//	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
//	VK_LCONTROL, VK_RCONTROL, VK_LMENU, VK_RMENU, VK_LSHIFT, VK_RSHIFT, VK_SPACE, VK_RETURN, VK_ESCAPE, VK_TAB, VK_BACK,
//	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
//	VK_NUMPAD0, VK_NUMPAD1, VK_NUMPAD2, VK_NUMPAD3, VK_NUMPAD4, VK_NUMPAD5, VK_NUMPAD6, VK_NUMPAD7, VK_NUMPAD8, VK_NUMPAD9,
//	VK_F1, VK_F2, VK_F3, VK_F4, VK_F5, VK_F6, VK_F7, VK_F8, VK_F9, VK_F10, VK_F11, VK_F12,
//	VK_DELETE, VK_END, VK_NEXT, VK_PRIOR, VK_HOME, VK_INSERT,
//	//VK_PRINT, VK_SCROLL, VK_PAUSE
};

INIT_INSTACNE(KeyManager);
KeyManager::KeyManager()
{
	mKeyInfos.clear();
}

KeyManager::~KeyManager()
{
	mKeyInfos.clear();
}

void KeyManager::Initialize()
{
	mKeyInfos.reserve((int)KEY::LAST_KEY);

	// 키 체크 상태 초기화
	for (int i = 0; i < (int)KEY::LAST_KEY; ++i)
	{
		mKeyInfos.emplace_back(KeyInfo(KEY_STATE::NONE, false));
	}
}

void KeyManager::Update()
{
	// 윈도우가 활성화 되어 있는지 판단
    HWND hwd = GetFocus();

    if(hwd == nullptr)
    {
        return;
    }

	for (int i = 0; i < (int)KEY::LAST_KEY; ++i)
	{
		if (GetAsyncKeyState(gWindowKeys[i]) & 0x8000)
		{
			// 현재 계속 키를 누르고 있을 때
			if (mKeyInfos[i].prevPush == true)
			{
				mKeyInfos[i].state = KEY_STATE::HOLD;
			}
			else
			{
				mKeyInfos[i].state = KEY_STATE::TAP;
//				std::cout << "키 처음으로 누름" << std::endl;
//				std::cout << "눌린 키 : " << (char)gWindowKeys[i] << std::endl;
			}

			mKeyInfos[i].prevPush = true;
		}
		// 현재 키가 안눌림
		else
		{
			if (mKeyInfos[i].prevPush == true)
			{
				mKeyInfos[i].state = KEY_STATE::AWAY;
			}
			else
			{
				mKeyInfos[i].state = KEY_STATE::NONE;
			}
			mKeyInfos[i].prevPush = false;
		}
	}
}
