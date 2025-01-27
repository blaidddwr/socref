#ifndef SOCREF_TESTS_UTILITY_H
#define SOCREF_TESTS_UTILITY_H
#include <QtGui>

bool areIconsEqual(const QIcon& icon0, const QIcon& icon1)
{
    QByteArray data0;
    QByteArray data1;
    QDataStream in0(&data0,QIODevice::WriteOnly);
    QDataStream in1(&data1,QIODevice::WriteOnly);
    in0 << icon0;
    in1 << icon1;
    return data0 == data1;
}

#endif
