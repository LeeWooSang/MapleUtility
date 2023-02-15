#pragma once
#include <unordered_map>
#include "../Common/Macro.h"
#include "../mainwindow.h"

class WindowManager
{
    SINGLE_TONE(WindowManager)

public:
    bool Initialize();
    MainWindow& GetMainWindow() { return mMainWindow; }

private:

private:
    class MainWindow mMainWindow;
};
