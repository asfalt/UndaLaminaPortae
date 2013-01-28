#ifndef VIEW_H
#define VIEW_H

#include<QGraphicsView>
#include <QGraphicsScene>
#include <QVector>
#include "Slice.h"
#include "kamera.h"

class View :public QGraphicsView
{
    Q_OBJECT
public:
    View(QObject *parent = 0);
    QGraphicsScene *Scene1;
    float Mavert, Mahor;     //Вертикальный и горизонтальный масштабы

    void ScaleAll(int sc);
    void DrawScene1();
    void DrawScene2(QVector<Slice> *VecSlices);
    void DrawKamera(Kamera *kamera);

    //Цвета
    QPen pen[5];

protected:


};


#endif // VIEW_H
