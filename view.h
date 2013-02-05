#ifndef VIEW_H
#define VIEW_H

#include<QGraphicsView>
#include <QGraphicsScene>
#include <QVector>
#include "Slice.h"
#include "kamera.h"

#include <QMouseEvent>
#include <math.h>

class View :public QGraphicsView
{
    Q_OBJECT
public:
    View(QObject *parent = 0);
    QGraphicsScene *Scene1;
    float Mavert, Mahor;     //Вертикальный и горизонтальный масштабы
    float MaVelos;           //Масштаб для графика скорости
//    bool ShowMarkers, ShowSlices;



    void ScaleAll(int sc);
    void DrawKamera(Kamera *kamera);

    //Цвета
    QPen pen[5];

//    void keyPressEvent(QKeyEvent *event);
    void wheelEvent(QWheelEvent *event);



public slots:

signals:
    void SignalWheelUp();
    void SignalWheelDown();


protected:


};


#endif // VIEW_H
