#pragma once
#include <iostream>
#include <string>
#include <QString>
#include <QMessageBox>
#include <QDateTime>

#define BLACK_COLOR  (QColor(0, 0, 0))
#define RED_COLOR      (QColor(255, 0, 0))
#define BLUE_COLOR    (QColor(0, 0, 255))
#define YELLOW_COLOR    (QColor(255, 255, 0))

#define BACKGROUND_BLACK_COLOR (qRgb(0, 0, 0))
#define BACKGROUND_GRAY_COLOR (qRgb(125, 125, 125))

#define LABEL_TEXT_RED_COLOR "QLabel{color : red;}"
#define LABEL_TEXT_GREEN_COLOR "QLabel{color : green;}"

inline QString FixKorQString(const std::string& text)
{
    return QString::fromLocal8Bit(text.c_str());
}

inline QString FixKorQString(const char* text)
{
    return QString::fromLocal8Bit(text);
}

inline void ShowMessageBox(QWidget* widget, const std::string& msg)
{
    QMessageBox::information(widget, "error", FixKorQString(msg.c_str()));
}

inline QMessageBox::StandardButton ShowQuestionMessageBox(QWidget* widget, const std::string& msg)
{
    return QMessageBox::question(widget, "notify", FixKorQString(msg.c_str()), QMessageBox::Yes|QMessageBox::No);
}

inline const std::string NumberToCommaNumber(long long number)
{
    bool flag = false;
    if(number < 0)
    {
        number = std::abs(number);
        flag = true;
    }

    std::string strNumber = std::to_string(number);
    std::string commaNumber;

     // 음수일때 부호 계산해야됨
    int mod = strNumber.length() % 3;

    for(int i = 0; i < strNumber.length(); ++i)
    {
        if(i != 0 && i % 3 == mod)
        {
            commaNumber.push_back(',');
        }
        commaNumber.push_back(strNumber[i]);
    }

    if(flag == true)
    {
        commaNumber = '-' + commaNumber;
    }

    return commaNumber;
}

inline const std::string Split(const std::string& str)
{
    int start = 0;
    std::string splitString;

    while(true)
    {
        int pos = str.find(",", start);
        if(pos == std::string::npos)
        {
            splitString += str.substr(start, str.length() - start);
            break;
        }
        splitString += str.substr(start, pos - start);
        start = pos + 1;
    }

    return splitString;
}

inline std::string GetCurrentDate()
{
    QString format = "yyyyMMdd";
    QDateTime date = QDateTime::currentDateTime();
    return date.toString(format).toStdString();
}

inline std::string GetCurrentDateTime()
{
    QString format = "yyyyMMddhhmmss";
    QDateTime date = QDateTime::currentDateTime();
    return date.toString(format).toStdString();
}
