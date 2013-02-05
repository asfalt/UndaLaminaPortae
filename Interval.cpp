#include "Interval.h"

Interval::Interval()
{
    number = -1;
    Slice1 = new Slice;
    Slice2 = new Slice;


    a1 = 0.0; a2 = 0.0; b1 = 0.0; b2 = 0.0; K = 0.0;
    a3 = 0.0; a4 = 0.0; b3 = 0.0; b4 = 0.0; L = 0.0;

    //Коэффициенты для прогонки
    Q1 = 0.0; Q2 = 0.0; Q3 = 0.0;
    M1 = 0.0; M2 = 0.0; M3 = 0.0;
    K1 = 0.0; K2 = 0.0;


}

Interval::Interval(int numberVal, Slice *slice1Item, Slice *slice2Item)
{
    number = numberVal;
    Slice1 = slice1Item;
    Slice2 = slice2Item;

    a1 = 0.0; a2 = 0.0; b1 = 0.0; b2 = 0.0; K = 0.0;
    a3 = 0.0; a4 = 0.0; b3 = 0.0; b4 = 0.0; L = 0.0;

    //Коэффициенты для прогонки
    Q1 = 0.0; Q2 = 0.0; Q3 = 0.0;
    M1 = 0.0; M2 = 0.0; M3 = 0.0;
    K1 = 0.0; K2 = 0.0;

}




