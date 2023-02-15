#include "WindowManager.h"

INIT_INSTACNE(WindowManager)
WindowManager::WindowManager()
{
}

WindowManager::~WindowManager()
{
}

bool WindowManager::Initialize()
{
    if(mMainWindow.Initialize() == false)
    {
        return false;
    }

    return true;
}
