QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
CONFIG += console

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Auction/Auction.cpp \
    Auction/AuctionItem.cpp \
    Balance/Balance.cpp \
    Common/JsonFormat/JsonFormat.cpp \
    Common/LongLongValidator.cpp \
    Common/MyWidget.cpp \
    Common/ThreadPool/ThreadPool.cpp \
    Framework/Framework.cpp \
    Item/Item.cpp \
    KeyManager/KeyManager.cpp \
    Network/Network.cpp \
    UI/QRegister.cpp \
    WindowManager/WindowManager.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    Auction/Auction.h \
    Auction/AuctionItem.h \
    Balance/Balance.h \
    Common/JsonFormat/JsonFormat.h \
    Common/LongLongValidator.h \
    Common/Macro.h \
    Common/MyWidget.h \
    Common/ThreadPool/ThreadPool.h \
    Common/Utility.h \
    Framework/Framework.h \
    Item/Item.h \
    KeyManager/KeyManager.h \
    Network/Network.h \
    UI/QRegister.h \
    WindowManager/WindowManager.h \
    mainwindow.h

FORMS += \
    UI/QRegister.ui \
    mainwindow.ui

#æ∆¿Ãƒ‹
RC_FILE += ../Resource/Image/MapleIcon.rc

INCLUDEPATH += ./Include ./Boost/boost_1_80_0

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
