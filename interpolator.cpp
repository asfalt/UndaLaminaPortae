#include "interpolator.h"

Interpolator::Interpolator()
{
     MaxTimeq = 0.0;
     MaxVaq = 0.0;
}

void Interpolator::ShowData()
{
    for(int i=0; i<this->timeq.count(); i++)
    {
        qDebug() << "i: " << i << "   timeq= " << timeq[i] << "   valq= " << valq[i];
    }
    qDebug() << "MaxQ " << MaxVaq << "   MaxTime " << MaxTimeq;
}

double Interpolator::InterpolateLinear(double TimeVal)
{
    double x1, x2, y1, y2, answer;
    x1 = 0.0; x2 = 0.0; y1 = 0.0; y2 = 0.0;

    if(TimeVal > timeq[timeq.count()-1])
    {
        answer = valq[valq.count()-1];
    }
    else
    {
       for(int i=0; i<this->timeq.count(); i++)
       {
           //Поскльку время у нас начинаеся с нуля можно не проверять
           if(timeq[i] >= TimeVal)
           {
               x2 = timeq[i];
               x1 = timeq[i-1];
               y2 = valq[i];
               y1 = valq[i-1];

               break;
           }
       }
 //      qDebug() << "x1= " << x1 << "   y1= " << y1 << "   x2= " << x2 << "   y2= " << y2;
       answer = ((y2 - y1)/(x2-x1))*(TimeVal - x1) + y1;
    }

//    qDebug() << "answer= " << answer;

    return answer;
}
