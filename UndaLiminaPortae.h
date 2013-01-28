#ifndef UNDALIMINAPORTAE_H
#define UNDALIMINAPORTAE_H

#include<QtGui>
#include <QWidget>
#include <QVector>
#include<QTimer>

#include "view.h"
#include "Slice.h"
#include "Interval.h"

#include "kamera.h"

    // === Используемые классы ============================================================================
class QWidget;
class QLabel;
class QPushButton;
class View;
class QSlider;
class QDial;


    // === Объявление основного класса ====================================================================

class Unda :public QWidget
{
    Q_OBJECT

public:
    Unda(QWidget *parent = 0);



    // === Константы ======================================================================================
    float g = 9.806;                  //Ускорение свободного падения



    // === Используемые переменные ========================================================================


    int vspomint;
    float vspomfloat;



public slots:
    void Button1_click();
    void Button2_click();
    void Button3_click();
    void MasshSlider_slide();
    void Dial1_slide();
    void OnTimer1();

private:
    QPushButton *Button1;
    QPushButton *Button2;
    QPushButton *Button3;
    View *view;
    QSlider *MasshSlider;
    QLabel *MasshLabel;
    QDial *Dial1;
    QTimer *Timer1;

    QVector<Slice> VecSlices;       //Вектор сечений

    Kamera kam;




};

#endif // UNDALIMINAPORTAE_H
