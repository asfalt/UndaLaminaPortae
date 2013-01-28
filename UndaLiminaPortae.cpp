#include <QtGui>
#include "UndaLiminaPortae.h"
#include <QMessageBox>


#include <QDebug>

Unda::Unda(QWidget *parent)
    :QWidget(parent)
{
    // === Инициализация ==================================================================================
    this->setGeometry(10,10,1100,800);
    vspomfloat = 0.0;
    vspomint = 0;


    // === Элементы =======================================================================================

    Button1 = new QPushButton("Button 1");
    Button2 = new QPushButton("Button 2");
    Button3 = new QPushButton("Button 3");
    view = new View();
    MasshSlider = new QSlider;
    MasshLabel = new QLabel("1.0");
    Dial1 = new QDial;
    Timer1 = new QTimer(this);


    // === Инициализация элементов ========================================================================
    MasshSlider->setOrientation(Qt::Horizontal);
    MasshSlider->setMinimum(1);
    Dial1->setEnabled(false);


    // === Группировка элементов ==========================================================================

    QVBoxLayout *ButtonLayout1 = new QVBoxLayout;
    ButtonLayout1->addWidget(Button1);
    ButtonLayout1->addWidget(Button2);
    ButtonLayout1->addWidget(Button3);
    ButtonLayout1->addWidget(Dial1);

    QHBoxLayout *MasshLayout = new QHBoxLayout;
    MasshLayout->addWidget(MasshSlider);
    MasshLayout->addWidget(MasshLabel);


    QGridLayout *MainLayout = new QGridLayout;
    MainLayout->addLayout(ButtonLayout1,0,0,Qt::AlignTop);
    MainLayout->addWidget(view,0,1);
    MainLayout->addLayout(MasshLayout,1,1);

    setLayout(MainLayout);

    // === Соединение сигналов ============================================================================

    connect(Button1, SIGNAL(clicked()), this, SLOT(Button1_click()));
    connect(Button2, SIGNAL(clicked()), this, SLOT(Button2_click()));
    connect(Button3, SIGNAL(clicked()), this, SLOT(Button3_click()));
    connect(MasshSlider, SIGNAL(valueChanged(int)), this, SLOT(MasshSlider_slide()));
    connect(Dial1, SIGNAL(valueChanged(int)), this, SLOT(Dial1_slide()));

    connect(Timer1, SIGNAL(timeout()), this, SLOT(OnTimer1()) );

}

// _______________________________________* ОСНОВНЫЕ ФУНКЦИИ *_____________________________________________
//________________________________________________*****____________________________________________________


void Unda::Button1_click()    // === Функция нажатия кнопки 1 =============================================
{
   // QMessageBox::about(this, "Button 1", "Button1 \nIs pressed");
   // this->view->Scene1->addEllipse(0,0,20,20);

   // view->DrawScene1();
   // view->DrawScene2(&VecSlices);

   // kam.ntimes--;

  //  kam.ShowKoeffs();

  //  kam.BackwardShuttle();

  //  view->DrawKamera(&kam);


 //   kam.ShowKoeffs();





    /*
    kam.EvalKoeffsEquations();

    kam.ForwardShuttle();

    kam.BackwardShuttle();

    view->DrawKamera(&kam);
*/

     Timer1->start();


    vspomint++;





}


void Unda::Button2_click()    // === Функция нажатия кнопки 2 =============================================
{
   // QMessageBox::about(this, "Button 2", "Button2 \nIs pressed");
/*
    Slice sl(1, 3.056);
    Slice sl2;
    sl2.number = 2222;
    sl2.Xpos = 22.220056;


    Slice sl3;

    Slice *sl4 = new Slice();
    sl4->number = 44;
    sl4->Xpos = 44.045;

    Slice *sl5 = new Slice(55, 55.023);

    VecSlices.append(sl);
    VecSlices.append(sl2);
    VecSlices.append(sl3);
    VecSlices.append(*sl4);
    VecSlices.push_front(*sl5);
*/


    kam.InreaseTime();

    kam.EvalKoeffsEquations();

    kam.ForwardShuttle();

    kam.BackwardShuttle();

    view->DrawKamera(&kam);



    vspomint++;




}


void Unda::Button3_click()    // === Функция нажатия кнопки 3 =============================================
{
   /*
    QVector<Slice>::iterator it = VecSlices.begin();
    for(; it != VecSlices.end(); ++it)
    {
        Slice *u;
        u = it;

        qDebug() << it <<"  number= " << u->number <<"  xpos= " <<u->Xpos <<"   h=" << u->h;
    }


    view->DrawScene2(&VecSlices);
    */


    /*
    Slice s1, s2;
    VecSlices.append(s1);
    VecSlices.append(s2);

    qDebug() <<"do____________";
    qDebug() << "pos1: " << s1.Xpos <<"  pos2: " <<s2.Xpos;
    qDebug() << "Vec  pos1: " << VecSlices[0].Xpos <<"  pos2: " <<VecSlices[1].Xpos;

    Interval *in = new Interval(1, &VecSlices[0], &VecSlices[1]);

    in->increasePos();

    qDebug() <<"posle____________";
    qDebug() << "pos1: " << s1.Xpos <<"  pos2: " <<s2.Xpos;
    qDebug() << "Vec  pos1: " << VecSlices[0].Xpos <<"  pos2: " <<VecSlices[1].Xpos;
    */

   // Kamera *kam2 = new Kamera;

    //Kamera kaam;
    //qDebug() << "bkam = " << kaam.bkam << "  lkam= " << kaam.lkam << "  ho= " << kaam.ho;

    kam.CreateIntervals();

    view->DrawKamera(&kam);

    MasshSlider->setValue(40);
    MasshSlider->setMaximum(400);




}

void Unda::MasshSlider_slide()   // === Функция обработки прокрутки масштаба ==============================
{
   // QMessageBox::about(this, "Slider", "Slider \nIs slide");
    float Pos;
    QString sa, sa2, sa3;
    Pos = MasshSlider->value();
    sa2.setNum(Pos);
    sa = sa2 +" | "+ sa3.setNum(Pos/10);
    MasshLabel->setText(sa);

    view->Mavert = Pos;
    view->Mahor = 1+Pos/10;

    //this->view->s
    view->DrawKamera(&kam);
    Dial1->setEnabled(true);

}

void Unda::Dial1_slide()   // === Функция обработки прокрутки Диала ==============================
{
    float Pos;
    QString sa, sa2, sa3;
    Pos = Dial1->value();
    sa.setNum(Pos/20);

    int ntimes = kam.ntimes;
    kam.VecSlices[5].hti[ntimes] = Pos/20;
    view->DrawKamera(&kam);

    view->Scene1->addSimpleText(sa);
}

void Unda::OnTimer1()
{
    Button2_click();

}

