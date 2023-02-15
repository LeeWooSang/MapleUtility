#pragma once
#include <vector>
#include "../Common/Macro.h"

#pragma comment(lib, "User32.lib")

enum class KEY
{
//	LEFT, RIGHT, UP, DOWN,
//	A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
//	LCTRL, RCTRL, LALT, RALT, LSHIFT, RSHIFT, SPACE, ENTER, ESC, TAB, BACK,
//	NUM0, NUM1, NUM2, NUM3, NUM4, NUM5, NUM6, NUM7, NUM8, NUM9,
//	NUMPAD0, NUMPAD1, NUMPAD2, NUMPAD3, NUMPAD4, NUMPAD5, NUMPAD6, NUMPAD7, NUMPAD8, NUMPAD9,
//	F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
//	DELETE, END, PAGE_DOWN, PAGE_UP, HOME, INSERT,
//	PRINT_SCREEN, SCROLL_LOCK, PAUSE,
    SCROLL_LOCK,
	LAST_KEY
};

enum class KEY_STATE
{
	// �������� ������ �ʾҰ�, ���絵 ������ ����
	NONE = 0,
	// �� ���� ����
	TAP,
	// ������ �ִ� ����
	HOLD,
	// �� �� ����
	AWAY
};

struct KeyInfo
{
	KeyInfo(KEY_STATE _state, bool _prevPush)
		: state(_state), prevPush(_prevPush) {}

	KEY_STATE state;
	// ���� �����ӿ��� ���ȴ��� ����
	bool prevPush;
};

class KeyManager
{
	SINGLE_TONE(KeyManager);

public:
	void Initialize();
	void Update();

	KEY_STATE GetKeyState(KEY key)	const { return mKeyInfos[(int)key].state; }

private:
	std::vector<KeyInfo> mKeyInfos;
};

