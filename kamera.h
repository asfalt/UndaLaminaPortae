#ifndef KAMERA_H
#define KAMERA_H

#include "Slice.h"
#include "Interval.h"
#include <QVector>

class Kamera
{
public:
    Kamera();
    void CreateIntervals();
    void InreaseTime();
    void EvalKoeffsEquations();
    void ShowKoeffs();
    void ShowKoeffs2();
    void ForwardShuttle();       //Прямая прогонка
    void BackwardShuttle();      //Обратная прогонка


    // === Константы ======================================================================================
    double g = 9.806;                  //Ускорение свободного падения



    // === Используемые переменные ========================================================================
    double bkam;      // Ширина камеры
    double lkam;      // Длина камеры
    double Hk;        // Начальный напор на камеру
    double ho;        // Глубина в камере в начале наполнения
    double Q;         // Расход
    double delta;     // Расстояние между сечениями (принимаем постоянным)
    double tau;       // Промежуток времени
    double Timen;     // Текущее время с начала наполнения
    double fi;        // Вспомогательная переменная схемы Прейсмана
    double nslices;   // Количество сечений

    int ntimes;      // Текущий временной слой




    QVector<Slice> VecSlices;             //Вектор сечений
    QVector<Interval> VecIntervals;       //Вектор интервалов

    QVector<float> Qti;                 //Расход

    double granv;



};

#endif // KAMERA_H
