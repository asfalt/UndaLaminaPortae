#include "view.h"
#include <QGraphicsSceneMouseEvent>
#include <QMessageBox>
#include <QColor>
#include <QtGui>


#include <QDebug>

View::View(QObject *parent) :QGraphicsView()
{
    Scene1 = new QGraphicsScene();
    this->setScene(Scene1);
    Mavert = 40;
    Mahor = 4.0;



    pen[0].setColor(Qt::black);                 //Для рисования осей
    pen[0].setWidth(2);
    pen[1].setColor(Qt::blue);                  //Для рисования стенок камеры
    pen[1].setWidth(2);
    pen[2].setColor(QColor(100, 200, 100, 150));   //Для рисования сечений
    pen[3].setColor(QColor(61, 137, 255));         //Для рисования воды
    pen[4].setColor(Qt::red);                      //Для рисования границ



    //this->scale(Mahor,Mavert);
}

void View::ScaleAll(int sc)
{
    this->scale(sc, sc);
}

void View::DrawScene1()
{
    this->Scene1->clear();
    this->Scene1->addLine(0, 0, 600, 0);
    this->Scene1->addLine(0, 0, 0, -4);
    this->Scene1->addLine(5, 0, 5, -4.2);
    this->Scene1->addLine(10, 0, 10, -4.25);
}


void View::DrawScene2(QVector<Slice> *VecSlices)
{
    float X, H;

    this->Scene1->clear();



    QVector<Slice>::iterator it = VecSlices->begin();
    for(; it != VecSlices->end(); ++it)
    {
        Slice *S;
        S = it;

        H = 1.4;
        X = S->Xpos;
        Scene1->addLine(X, 0, X, -H*Mavert);
        Scene1->addText("k");
    }





   // this->Scene1->addLine(0, 0, pe, 0);
}

void View::DrawKamera(Kamera *kamera)
{

    //Масштабируем
    float lkam = kamera->lkam;
    float Hk = kamera->Hk;
    float ho = kamera->ho;

    ho = ho*Mavert;
    Hk = Hk*Mavert;
    lkam = lkam*Mahor;


    QRect rec(-20,-Hk-20,lkam+40,Hk+40);    //left, top, width, height

    this->Scene1->clear();
    //this->scroll(50,50);
    this->setSceneRect(rec);   //Чтобы изображение всегда было посередине



    //Изображаем камеру

    Scene1->addLine(0, 0, lkam, 0, pen[0]);
    Scene1->addLine(0, 0, 0, -Hk, pen[0]);
    Scene1->addLine(lkam, 0, lkam, -Hk, pen[0]);
    //края камеры
    Scene1->addLine(-5, -Hk, 0, -Hk, pen[0]);
    Scene1->addLine(lkam, -Hk, lkam+5, -Hk, pen[0]);



    //Изображаем горизонтальную масштабную линию

    Scene1->addLine(-10, 50, lkam+50, 50);
    Scene1->addLine(lkam+42, 52, lkam+50, 50, pen[0]);
    Scene1->addLine(lkam+42, 48, lkam+50, 50, pen[0]);

    float i = 0;
    int co =0;
    while(i < lkam+40)
    {

        Scene1->addLine(i, 50, i, 55);

        QString ss;
        ss.setNum(co);
        QGraphicsTextItem *texcoo = Scene1->addText(ss);
        texcoo->setPos(i-7,52);
        co = co + 20;
        i = i+20*Mahor;
    }




    //Изображаем вертикальную масштабную линию

    Scene1->addLine(lkam+50, 15, lkam+50, -Hk-50);
    Scene1->addLine(lkam+48, -Hk-42, lkam+50, -Hk-50, pen[0]);
    Scene1->addLine(lkam+52, -Hk-42, lkam+50, -Hk-50, pen[0]);

    i = 0;
    co = 0;
    while(i < Hk+40)
    {

        Scene1->addLine(lkam+50, -i, lkam+55, -i);

        QString ss;
        ss.setNum(co);
        QGraphicsTextItem *texcoo = Scene1->addText(ss);
        texcoo->setPos(lkam+58,-i-12);
        co = co + 2;
        i = i+2*Mavert;
    }


    //Надпись камера

    QGraphicsTextItem *tec = Scene1->addText("kamera");
    tec->setPos(lkam/2-28, -Hk-30);


    //Изображаем сечения

    float Xposm = 0;
    float XposmPrew = 0;
    float hm = 0;
    float hmPrew = 0;

    float vm = 0;
    float vmPrew = 0;

    int ii = 0;
    int ntimes = kamera->ntimes;

    QVector<Slice>::iterator it = kamera->VecSlices.begin();
    for (; it != kamera->VecSlices.end(); ++it)
    {
        Slice *slu;
        slu = it;

        Xposm = slu->Xpos * Mahor;
        hm = slu->hti[ntimes] * Mavert;
        vm = slu->vti[ntimes] *Mavert*5;

        if (slu->isBound)
        {
            Scene1->addLine(Xposm, 8, Xposm, -Hk, pen[4]);
        }
        else
        {
            Scene1->addLine(Xposm, 8, Xposm, -Hk, pen[2]);
        }

        //Вода
        if ( ii > 0)
        {
            Scene1->addLine(XposmPrew, -hmPrew, Xposm, -hm, pen[3]);
            Scene1->addLine(XposmPrew, -vmPrew, Xposm, -vm, pen[4]);

        }

        Scene1->addEllipse(Xposm-4, -hm-4, 8, 8, pen[1]);

        //Скорость
        Scene1->addEllipse(Xposm-3, -vm-3, 6, 6, pen[4]);



        XposmPrew = Xposm;
        hmPrew = hm;
        vmPrew = vm;


        ii++;
    }

    QString sss;
    sss.setNum( kamera->VecSlices[0].hti[ntimes] );
    QGraphicsTextItem *tec2 = Scene1->addText(sss);
    tec2->setPos(0.0 , 20);

   //Scene1->addText("kamera");



}
