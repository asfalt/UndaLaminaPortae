#ifndef INTERPOLATOR_H
#define INTERPOLATOR_H

#include <QVector>
#include <QDebug>

class Interpolator
{
public:
    Interpolator();
    double InterpolateLinear(double TimeVal);
    void ShowData();

    double MaxTimeq, MaxVaq;

    QVector<double> timeq;
    QVector<double> valq;

};

#endif // INTERPOLATOR_H
