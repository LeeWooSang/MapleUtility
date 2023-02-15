#include <QApplication>
#include "Framework/Framework.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Framework framework;
    if(framework.Initialize() == false)
    {
        return -1;
    }

    framework.Run();
    return a.exec();
}
