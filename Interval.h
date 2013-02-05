#ifndef INTERVAL_H
#define INTERVAL_H
#include "Slice.h"

struct Interval
{
    Slice *Slice1;
    Slice *Slice2;

    int number;

    //Переменные проходов
//    float v1, v2, h1, h2;
    double a1, a2, b1, b2, K;   //уравнение движения
    double a3, a4, b3, b4, L;   //уравнение неразрывности

    //Коэффициенты для прогонки
    double Q1, Q2, Q3;
    double M1, M2, M3;
    double K1, K2;


public:
    Interval();
    Interval(int numberVal, Slice *slice1Item, Slice *slice2Item);




};

#endif // INTERVAL_H
