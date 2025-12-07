#include "utility.h"

QString getJustifiedNumString(int num, int size, NumStringAlign align)
{
     QString s = QString::number(num);
     if (s.size() < size)
     {
         uint64_t diffsize = size - s.size();
         switch (align)
         {
             case NumStringAlign::LEFT:
                 s = s + QString(diffsize, '0');
                 break;
             case NumStringAlign::RIGHT:
                 s = QString(diffsize, '0') + s;
                 break;
             default:
                 break;
         }
     }
     return s;
}

QString getTimeStringFromSecs(uint64_t secs)
{
    int hours = secs / 3600;
    secs %= 3600;
    int mins = secs / 60;
    secs %= 60;
    auto s = QString("%1:%2:%3").
        arg(getJustifiedNumString(hours)).
        arg(getJustifiedNumString(mins)).
        arg(getJustifiedNumString(secs));
    return s;
}

QString dateTime2String(const QDateTime& dt)
{
    return QString("%1-%2-%3 %4:%5:%6").
        arg(getJustifiedNumString(dt.date().year())).
        arg(getJustifiedNumString(dt.date().month())).
        arg(getJustifiedNumString(dt.date().day())).
        arg(getJustifiedNumString(dt.time().hour())).
        arg(getJustifiedNumString(dt.time().minute())).
        arg(getJustifiedNumString(dt.time().second()));
}

QDateTime string2DateTime(const QString& str)
{
    return QDateTime::fromString(str.simplified().replace(' ', 'T'), Qt::ISODate);
}

QString dateTime2StringForFilename(const QDateTime& dt)
{
    return QString("%1-%2-%3 %4%5%6").
        arg(getJustifiedNumString(dt.date().year())).
        arg(getJustifiedNumString(dt.date().month())).
        arg(getJustifiedNumString(dt.date().day())).
        arg(getJustifiedNumString(dt.time().hour())).
        arg(getJustifiedNumString(dt.time().minute())).
        arg(getJustifiedNumString(dt.time().second()));
}
