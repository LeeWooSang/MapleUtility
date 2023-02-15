#include "Framework.h"
#include "../Common/Utility.h"
#include "../WindowManager/WindowManager.h"

Framework::Framework()
{
}

Framework::~Framework()
{   
    GET_INSTANCE(WindowManager)->Release();
}

bool Framework::Initialize()
{    
    if(GET_INSTANCE(WindowManager)->Initialize() == false)
    {
        return false;
    }

    return true;
}

void Framework::Run()
{
    GET_INSTANCE(WindowManager)->GetMainWindow().Render();
}
