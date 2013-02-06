#include "kamera.h"



#include <QDebug>

Kamera::Kamera()
{

    g = 9.806;
    bkam = 20;       // Ширина камеры
    lkam = 140;      // Длина камеры
    Hk = 10;        // Максимальная глубина в камере
    ho = 1.5;          // Глубина в камере в начале наполнения
    Q = 0;           // Расход
    delta = 1.0;       // Расстояние между сечениями (принимаем постоянным)
    tau = 0.1;         // Промежуток времени
    Timen = 0.0;       // Текущее время с начала наполнения
    fi = 0.7;        // Вспомогательная переменная схемы Прейсмана
    nslices = 20;    // Количество сечений
    TimenEnd = 20;   //Время окончания расчёта

    ntimes = 0;      // Текущий временной слой

    inputq = new Inputq();

    granv = 1.0;

}

//Функция, создающая сечения и интервалы по размерам камеры
void Kamera::CreateIntervals()
{
   /* //Удаляем старые интервалы
    VecSlices.clear();
    VecIntervals.clear();
    ntimes = 0;      // Текущий временной слой
   */
    ClearKam();

    //определяем расстояние между интервалами
    delta = lkam/(nslices-1);


    //Создаём вектор сечений
    for (int i=0; i < nslices; i++)
    {
        Slice *sl = new Slice(i, delta * i, ho);

        //Назначим сечения в начале и в конце граничными
        if (i == 0 || i == (nslices-1))
        {
            sl->isBound = true;
        }


        VecSlices.append(*sl);
//        qDebug() << i;
    }

//    qDebug() << "----------";

    //Создаём вектор интервалов
    for (int i=1; i<nslices; i++)
    {
        Interval *inte = new Interval(i-1, &VecSlices[i-1], &VecSlices[i]);
        VecIntervals.append(*inte);
//        qDebug() << i << " num: "<< VecIntervals[i-1].number <<  "    sl1.num: " << VecSlices[i-1].number <<  "     sl2.num: " << VecSlices[i].number;
    }



}

//Функция увеличения текущего времени
void Kamera::InreaseTime()
{
    ntimes++;
    Timen = Timen + tau;
    //Необходимо в сечениях создать соответствующие элементы для хранения h v
    //Проход по всем сечениям
    QVector<Slice>::iterator it = VecSlices.begin();
    for (; it != VecSlices.end(); ++it)
    {
        Slice *sl;
        sl = it;
        sl->hti.append(-1.0);   //пока присваиваем такую глубину
        sl->vti.append(-1.0);   //и такую скорость

    }
}

//Функция, вычисления коэффициентов уравнений
void Kamera::EvalKoeffsEquations()
{

    float h1Prew, h2Prew, v1Prew, v2Prew;
    float Ha, Va;

    if(ntimes != 0)
    {

        //Проход по всем интервалам
        QVector<Interval>::iterator it = VecIntervals.begin();
        for(; it != VecIntervals.end(); ++it)
        {
            Interval *Interv;
            Interv = it;

            //С предыдущего слоя по времени
            h1Prew = Interv->Slice1->hti[ntimes-1];
            h2Prew = Interv->Slice2->hti[ntimes-1];
            v1Prew = Interv->Slice1->vti[ntimes - 1];
            v2Prew = Interv->Slice2->vti[ntimes - 1];

            Va = (v1Prew + v2Prew)/2;
            Ha = (h1Prew + h2Prew)/2;

            //Уравнение движения
            Interv->a1 = (1/(2*tau))-(fi*Va/delta);
            Interv->a2 = (1/(2*tau))+(fi*Va/delta);
            Interv->b1 = -(g*fi/delta);
            Interv->b2 = g*fi/delta;
            Interv->K = ( ((1-fi)/delta)*(Va*(v2Prew-v1Prew) + g*(h2Prew-h1Prew)) ) - ((v1Prew+v2Prew)/(2*tau));

            //Уравнение неразрывности
            Interv->a3 = -(Ha*fi/delta);
            Interv->a4 = Ha*fi/delta;
            Interv->b3 = (1/(2*tau)) - (Va*fi/delta);
            Interv->b4 = (1/(2*tau)) + (Va*fi/delta);
            Interv->L = ( ((1-fi)/delta)*(Va*(h2Prew-h1Prew) + Ha*(v2Prew-v1Prew)) ) - ((h1Prew+h2Prew)/(2*tau));


        }


    }


}

void Kamera::ShowKoeffs()
{
    //Проход по всем интервалам
    QVector<Interval>::iterator it = VecIntervals.begin();
    for(; it != VecIntervals.end(); ++it)
    {
        Interval *Interv;
        Interv = it;

        qDebug() << "interval " << Interv->number << "  a1: " << Interv->a1 << "  a2: " << Interv->a2 << "  a3: " << Interv->a3 << "  a4: " << Interv->a4;
        qDebug() << "            "  << "  b1: " << Interv->b1 << "  b2: " << Interv->b2 << "  b3: " << Interv->b3 << "  b4: " << Interv->b4;
        qDebug() << "            "  << "  K:  " << Interv->K  << "  L:  " << Interv->L;

        qDebug() << "  K1: " << Interv->K1 << "  K2: " << Interv->K2;
        qDebug() << "  Q1: " << Interv->Q1 << "  Q2: " << Interv->Q2 << "  Q3: " << Interv->Q3;
        qDebug() << "  M1: " << Interv->M1 << "  M2: " << Interv->M2 << "  M3: " << Interv->M3;
        qDebug() << "  h1: " << Interv->Slice1->hti[ntimes] << "  h2: " << Interv->Slice2->hti[ntimes] ;
        qDebug() << "  v1: " << Interv->Slice1->vti[ntimes] << "  v2: " << Interv->Slice2->vti[ntimes] ;
//        qDebug() << "  v1 _dop: " << Interv->Slice1->vti_dop[ntimes] << "  v2_dop: " << Interv->Slice2->vti_dop[ntimes] ;
        qDebug() << "____________________________________________________________________________________________________________________________________";

    }
}



//Функция прямой прогонки
void Kamera::ForwardShuttle()
{
    double vgr1, vgr2;
    double K1Prew, K2Prew;

    K1Prew = 0.0; K2Prew = 0.0;

    //Назначаем граничные условия

    //Сначала определим расход
    double Qtek, hgran;
    Qtek = inputq->interpolator->InterpolateLinear(Timen);
    //Покажем линию текущего положения времени на графике расхода
    inputq->ShowTimeWhichInterpolate(Timen);
    //Глубина в камере по предыдущему слою по времени
    hgran = VecSlices[0].hti[ntimes-1];

    //vgr1 = Qtek/(hgran*bkam);
    granv = Qtek/(hgran*bkam);

    vgr1 = granv;
    vgr2 = 0.0;

    //Проход по всем интервалам
    QVector<Interval>::iterator it = VecIntervals.begin();
    for(; it != VecIntervals.end(); ++it)
    {
        Interval *Interv;
        Interv = it;
        Interv->EvalForward(K1Prew, K2Prew, vgr1, vgr2, ntimes);
        K1Prew = Interv->K1;
        K2Prew = Interv->K2;
    }
}


//Функция обратной прогонки
void Kamera::BackwardShuttle()
{
    double k1pr, k2pr;

    int col = VecIntervals.count();
//    qDebug() << "OBRATNAIA PROGONKA _______________________________________________________ " ;
//    qDebug() << "count: " << col;
//    qDebug() << "nu: " << VecIntervals[col-1].number;

    for (int i = col-1; i>0; i--)
    {
        k1pr = VecIntervals[i-1].K1;
        k2pr = VecIntervals[i-1].K2;
        VecIntervals[i].EvalBackwardInner(k1pr, k2pr, ntimes);
    }

    //Верхний интервал
    VecIntervals[0].EvalBackwardFirst(ntimes);
}


void Kamera::ClearKam()
{
    //Удаляем старые интервалы
    VecSlices.clear();
    VecIntervals.clear();
    ntimes = 0;      // Текущий временной слой
    Timen = 0.0;
}



















