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

void Interval::EvalForward(double K1PrewVal, double K2PrewVal, double vgr1Val, double vgr2Val, int ntimesVal)
{
    //Функция вычисления коэффициентов прямой прогонки

    //Сначала вычислим упрощающие коэффициенты по коэффициентам уравнений соханения
    double W1, W2, W3, W4;

    W2 = 1/(b1-(b3*a1/a3));
    W1 = W2*a1/a3;

    W4 = 1/(a2-(a4*b2/b4));
    W3 = W4*b2/b4;

    Q1 = a4*W1 - a2*W2;
    Q2 = b4*W1 - b2*W2;
    Q3 = L*W1 - K*W2;

    M1 = a3*W3 - a1*W4;
    M2 = b3*W3 - b1*W4;
    M3 = L*W3 - K*W4;

    //Теперь вычисляем коэффициенты прогонки
    //Верхний граничный интервал
    if(Slice1->isBound)
    {
        K1 = (Q2*M2)/(1-Q1*M2);
        K2 = (vgr1Val*M1 + Q3*M2 + M3)/(1-Q1*M2);
        Slice1->vti[ntimesVal] = vgr1Val;
    }
    //Нижний граничный интервал
    else if(Slice2->isBound)
    {
        K1 = (Q2*(K1PrewVal*M1 + M2)) / (1 - K1PrewVal*Q1*M1 - Q1*M2);
        K2 = (Q3*(K1PrewVal*M1 + M2) + K2PrewVal*M1 + M3) / (1 - K1PrewVal*Q1*M1 - Q1*M2);
        Slice2->vti[ntimesVal] = vgr2Val;
        Slice2->hti[ntimesVal] = (vgr2Val - K2) / K1;
    }
    //Внутренние интервалы
    else
    {
        K1 = (Q2*(K1PrewVal*M1 + M2)) / (1 - K1PrewVal*Q1*M1 - Q1*M2);
        K2 = (Q3*(K1PrewVal*M1 + M2) + K2PrewVal*M1 + M3) / (1 - K1PrewVal*Q1*M1 - Q1*M2);
    }
}

void Interval::EvalBackwardInner(double K1prVal, double K2prVal, int ntimesVal)
{
    //Функция обратной прогонки для внутренних и нижнего интервалов
    double h1;

    h1 = Slice2->vti[ntimesVal]*Q1 + Slice2->hti[ntimesVal]*Q2 + Q3;               // h1 = v2*Q1 + h2*Q2 + Q3;
    Slice1->hti[ntimesVal] = h1;
    Slice1->vti[ntimesVal] = K1prVal*h1 + K2prVal;                                 // v1
}

void Interval::EvalBackwardFirst(int ntimesVal)
{
    //Функция обратной прогонки для верхнего интервала
    Slice1->hti[ntimesVal] = Slice2->vti[ntimesVal]*Q1 + Slice2->hti[ntimesVal]*Q2 + Q3;                // h1 = v2*Q1 + h2*Q2 + Q3;
                                                                                                        // VecIntervals[i].Slice1->hti[ntimes] = h1;
}



