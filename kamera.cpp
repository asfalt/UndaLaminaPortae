#include "kamera.h"



#include <QDebug>

Kamera::Kamera()
{

    bkam = 20;       // Ширина камеры
    lkam = 140;      // Длина камеры
    Hk = 10;        // Начальный напор на камеру
    ho = 1.5;          // Глубина в камере в начале наполнения
    Q = 0;           // Расход
    delta = 1.0;       // Расстояние между сечениями (принимаем постоянным)
    tau = 0.1;         // Промежуток времени
    Timen = 0.0;       // Текущее время с начала наполнения
    fi = 0.7;        // Вспомогательная переменная схемы Прейсмана
    nslices = 20;    // Количество сечений

    ntimes = 0;      // Текущий временной слой


    Qti.append(0.2);
    Qti.append(0.3);
    Qti.append(0.4);
    Qti.append(0.5);
    Qti.append(0.4);
    Qti.append(0.2);
    Qti.append(0.1);

    granv = 1.0;

}

//Функция, создающая сечения и интервалы по размерам камеры
void Kamera::CreateIntervals()
{
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
    Timen = Timen + ntimes * tau;
    //Необходимо в сечениях создать соответствующие элементы для хранения h v
    //Проход по всем сечениям
    QVector<Slice>::iterator it = VecSlices.begin();
    for (; it != VecSlices.end(); ++it)
    {
        Slice *sl;
        sl = it;
        sl->hti.append(-1.0);   //пока присваиваем такую глубину
        sl->vti.append(-1.0);   //и такую скорость

        sl->vti_dop.append(-1.0);
//        Qti.append(0.0);
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
            Interv->b1 = -g*fi/delta;
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

void Kamera::ShowKoeffs2()
{
   ;
}


//Функция прямой прогонки
void Kamera::ForwardShuttle()
{
    double a1, a2, a3, a4, b1, b2, b3, b4, L, K;
    double vgr1, vgr2;
    double K1Prew, K2Prew;
    double W1, W2, W3, W4;

    double Q1, Q2, Q3, M1, M2, M3;

    K1Prew = 0.0; K2Prew = 0.0;

//    Qti.append(0.0);
//    v1 = Qti[ntimes];

    //Назначаем граничные условия
    vgr1 = granv;
    vgr2 = 0.0;


    //Проход по всем интервалам
    QVector<Interval>::iterator it = VecIntervals.begin();
    for(; it != VecIntervals.end(); ++it)
    {
        Interval *Interv;
        Interv = it;
        a1 = Interv->a1;
        a2 = Interv->a2;
        a3 = Interv->a3;
        a4 = Interv->a4;
        b1 = Interv->b1;
        b2 = Interv->b2;
        b3 = Interv->b3;
        b4 = Interv->b4;
        L = Interv->L;
        K = Interv->K;


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

        Interv->M1 = M1;
        Interv->M2 = M2;
        Interv->M3 = M3;
        Interv->Q1 = Q1;
        Interv->Q2 = Q2;
        Interv->Q3 = Q3;

        //Обработка верхнего граничного интервала
        if (Interv->Slice1->isBound)
        {
            Interv->K1 = (Q2*M2)/(1-Q1*M2);
            Interv->K2 = (vgr1*M1 + Q3*M2 + M3)/(1-Q1*M2);

            Interv->Slice1->vti[ntimes] = vgr1;

//            qDebug() << "verhniy  " << "vgr1: " << vgr1;


        }
        //Обработка нижнего граничного интервала
        else if (Interv->Slice2->isBound)
        {

            Interv->K1 = (Q2*(K1Prew*M1 + M2)) / (1 - K1Prew*Q1*M1 - Q1*M2);
            Interv->K2 = (Q3*(K1Prew*M1 + M2) + K2Prew*M1 + M3) / (1 - K1Prew*Q1*M1 - Q1*M2);

            Interv->Slice2->vti[ntimes] = vgr2;
            Interv->Slice2->hti[ntimes] = (vgr2 - Interv->K2) / Interv->K1;

//            qDebug() << "|||||||||||||  nijniy  " << "h2: " << Interv->Slice2->hti[ntimes] << "   k1: " << Interv->K1 << "    k2:" << Interv->K2;

        }
        //Обработка внутренних интервалов
        else
        {
            Interv->K1 = (Q2*(K1Prew*M1 + M2)) / (1 - K1Prew*Q1*M1 - Q1*M2);
            Interv->K2 = (Q3*(K1Prew*M1 + M2) + K2Prew*M1 + M3) / (1 - K1Prew*Q1*M1 - Q1*M2);

        }

        K1Prew = Interv->K1;
        K2Prew = Interv->K2;

    }


}


//Функция обратной прогонки
void Kamera::BackwardShuttle()
{

    double Q1, Q2, Q3, M1, M2, M3;
    double v2, h2;

    double k1pr, k2pr;

    double h1;

    int col = VecIntervals.count();
//    qDebug() << "OBRATNAIA PROGONKA _______________________________________________________ " ;
//    qDebug() << "count: " << col;

    VecIntervals[col-1].number;
//    qDebug() << "nu: " << VecIntervals[col-1].number;

    for (int i = col-1; i>0.0; i--)
    {


        Q1 = VecIntervals[i].Q1;
        Q2 = VecIntervals[i].Q2;
        Q3 = VecIntervals[i].Q3;

        M1 = VecIntervals[i].M1;
        M2 = VecIntervals[i].M2;
        M3 = VecIntervals[i].M3;

        v2 = VecIntervals[i].Slice2->vti[ntimes];
        h2 = VecIntervals[i].Slice2->hti[ntimes];

        k1pr = VecIntervals[i-1].K1;
        k2pr = VecIntervals[i-1].K2;

        h1 = v2*Q1 + h2*Q2 + Q3;
        VecIntervals[i].Slice1->hti[ntimes] = h1;
        VecIntervals[i].Slice1->vti[ntimes] = (v2 - M3 - M2*h1) / M1;
//        VecIntervals[i].Slice1->vti[ntimes] = k1pr*h1 + k2pr;


//        qDebug() << "i: " << i << "    h1= " << h1         << "    h2= " << h2 << "    v2= " << v2       << "  || Q1: " << Q1 << "    Q2= " << Q2         << "    Q3= " << Q3 << "    v2= " << v2;
//         qDebug() << "__ __"         << "  || M1: " << M1 << "    M2= " << M2         << "    M3= " << M3 << "    v1= " << (v2 - M3 - M2*h1) / M1;

    }



    //Верхний интервал

        int i = 0;
        Q1 = VecIntervals[i].Q1;
        Q2 = VecIntervals[i].Q2;
        Q2 = VecIntervals[i].Q2;

        M1 = VecIntervals[i].M1;
        M2 = VecIntervals[i].M2;
        M3 = VecIntervals[i].M3;

        v2 = VecIntervals[i].Slice2->vti[ntimes];
        h2 = VecIntervals[i].Slice2->hti[ntimes];

        h1 = v2*Q1 + h2*Q2 + Q3;
        VecIntervals[i].Slice1->hti[ntimes] = h1;

        double v1obr;
        v1obr = (v2 - M3 - M2*h1) / M1;

        double diver = v1obr - granv;


 //       granv = 0.2;

 //       VecIntervals[i].Slice1->vti[ntimes] =  granv;

        qDebug() << "nijnii______________      ** ";
        qDebug() << "v1 obr: " << v1obr;
        qDebug() << "v1 gr:  "  << VecIntervals[i].Slice1->vti[ntimes];
        qDebug() << "granv:  "  << granv;
        qDebug() << "diver:  "  << diver;


}



















