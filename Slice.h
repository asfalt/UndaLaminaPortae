#ifndef SLICE_H
#define SLICE_H

#include <QVector>

struct Slice
{
    int number;            //Номер сечения
    double Xpos;            //Координата сечения по оси ОХ
    QVector<double> hti;    //Текущая глубина
    QVector<double> vti;    //Текущая скорость
    bool isBound;          //Флаг, показывающий является ли интервал граничным

public:
    Slice();
    Slice(int numberVal, double XposVal, double hVal);

};

#endif // SLICE_H
