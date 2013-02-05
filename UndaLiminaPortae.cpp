#include <QtGui>
#include "UndaLiminaPortae.h"
#include <QMessageBox>
#include <QFrame>
#include <QFile>
#include <QDataStream>
#include <QFileDialog>



#include <QDebug>

Unda::Unda(QWidget *parent)
    :QWidget(parent)
{
    // === Инициализация ==================================================================================
    this->setGeometry(40,40,1200,800);
    vspomfloat = 0.0;
    vspomint = 0;
    QIcon *iconWin = new QIcon;
    iconWin->addFile("mainIcon.png");
    setWindowIcon(*iconWin);

    g = 9.806;


    // === Элементы =======================================================================================

    Button1 = new QPushButton("Задать расход");
    Button2 = new QPushButton("Пошаговый расчёт");
    Button3 = new QPushButton("Button 3");
    Button4 = new QPushButton("Расчет");

    //Подложка
    QLabel *la = new QLabel(" ", this);
    QPixmap pixmap("podd2.png");
    la->setFixedHeight(1500);
    la->setFixedWidth(2000);
    la->setPixmap(pixmap);

    QLabel *StatikLabel = new QLabel("<h3>Поверхность воды в камере судоходного шлюза</h>", this);
    StatikLabel->setStyleSheet("color: grey");

    QLabel *LabelLogo = new QLabel("logo", this);
    QPixmap logoPixmap("logo.png");
    LabelLogo->setPixmap(logoPixmap);
    LabelLogo->setFixedSize(50,60);


    view = new View();
    MasshSlider = new QSlider;
    MasshLabel = new QLabel("1.0");
    Dial1 = new QDial;
    Timer1 = new QTimer(this);


    par1 = new Parkam(this, "Длина камеры");
    par2 = new Parkam(this, "Ширина камеры");
    par3 = new Parkam(this, "Максимальная глубина в камере");
    par4 = new Parkam(this, "Глубина в начале наполнения");
    par5 = new Parkam(this, "Количество расчётных сечений");
    par6 = new Parkam(this, "Расчётный промежуток времени");
    par7 = new Parkam(this, "Параметр fi");
    par8 = new Parkam(this, "Время окончания расчёта");

    vertScale = new ScaleSetter(this, "Верт. масштаб");
    horiScale = new ScaleSetter(this, "Гориз. Масштаб");
    veloScale = new ScaleSetter(this, "Масшт. Скорости");




    // === Инициализация элементов ========================================================================
    MasshSlider->setOrientation(Qt::Horizontal);
    MasshSlider->setMinimum(1);
    Dial1->setEnabled(false);

    Button1->setFixedHeight(40);
    Button1->setStyleSheet("background-color: rgb(243, 177, 110); border-radius: 10px; border-width: 2px; border-color: beige; border-style: outset");
    Button2->setFixedSize(150,40);
    Button2->setStyleSheet("background-color: rgb(243, 177, 110); border-radius: 10px; border-width: 2px; border-color: beige; border-style: outset");
    Button3->setMaximumWidth(100);
    Button4->setFixedSize(120,40);
    Button4->setStyleSheet("background-color: rgb(243, 177, 110); border-radius: 10px; border-width: 2px; border-color: beige; border-style: outset");


    // === Меню ===========================================================================================
    QIcon ico;

    //Открыть файл параметров
    ico.addFile("open.png");
    OpenAct = new QAction(ico, tr("&Open"), this);
    OpenAct->setShortcut(tr("Ctrl+O"));
    connect(OpenAct, SIGNAL(triggered()), this, SLOT(ReadParkamAct()) );

    //Сохранить файл параметров
    ico.addFile("save.png");
    SaveAct = new QAction(ico, tr("&Save"), this);
    SaveAct->setShortcut(tr("Ctrl+S"));
    connect(SaveAct, SIGNAL(triggered()), this, SLOT(WriteParkam()) );


    MyMenuBar = new QMenuBar(this);
    FileMenu = MyMenuBar->addMenu("&File");
    FileMenu->addAction(OpenAct);
    FileMenu->addAction(SaveAct);

 /*   //Вставляем контекстное меню для отображения его по щелчку лев кн мыши
    this->addAction(OpenAct);
    this->addAction(SaveAct);
    this->setContextMenuPolicy(Qt::ActionsContextMenu);
*/

    MyMenuBar->setShown(false);  //так покажет только на убунте в юнити

    FileToolBar = new QToolBar(this);
    FileToolBar->addAction(OpenAct);
    FileToolBar->addSeparator();
    FileToolBar->addAction(SaveAct);
    FileToolBar->setStyleSheet("background-color: beige; border-style: outset; border-width: 1px; border-color: grey");
    FileToolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    FileToolBar->setAutoFillBackground(true);
    FileToolBar->setFixedWidth(170);


    MyStatusBar = new QStatusBar(this);
    QLabel *TimeLabel = new QLabel("Время: 0.0");
    MyStatusBar->addWidget(TimeLabel);


    // === Группировка элементов ==========================================================================



    QHBoxLayout *MasshLayout = new QHBoxLayout;
    MasshLayout->addWidget(MasshSlider);
    MasshLayout->addWidget(MasshLabel);


    QVBoxLayout *ParamKameraLayout = new QVBoxLayout;

    ParamKameraLayout->addWidget(Button1);
    ParamKameraLayout->addWidget(par1);
    ParamKameraLayout->addWidget(par2);
    ParamKameraLayout->addWidget(par3);
    ParamKameraLayout->addWidget(par4);
    ParamKameraLayout->addWidget(par5);
    ParamKameraLayout->addWidget(par6);
    ParamKameraLayout->addWidget(par7);
    ParamKameraLayout->addWidget(par8);
    ParamKameraLayout->addWidget(Button2, 0, Qt::AlignRight);
    ParamKameraLayout->addWidget(Button4, 0, Qt::AlignRight);

    ParamKameraLayout->setSpacing(2);
    ParamKameraLayout->addStretch();


    QVBoxLayout *ButtonLayout1 = new QVBoxLayout;
 //   ButtonLayout1->addSpacing(50);
 //   ButtonLayout1->addWidget(Button1);
 //   ButtonLayout1->addWidget(Button2);
 //   ButtonLayout1->addWidget(Button3);               //Единственная пока не задействованная
 //   ButtonLayout1->addWidget(Dial1);
    ButtonLayout1->addLayout(ParamKameraLayout);


    QHBoxLayout *BarsLayout = new QHBoxLayout;
    BarsLayout->addWidget(FileToolBar);

    QHBoxLayout *ScaleLayout = new QHBoxLayout;
    ScaleLayout->addWidget(StatikLabel);
    ScaleLayout->addStretch();
    ScaleLayout->addWidget(veloScale, 0, Qt::AlignLeft);
    ScaleLayout->addSpacing(20);
    ScaleLayout->addWidget(horiScale, 0, Qt::AlignLeft);
    ScaleLayout->addSpacing(20);
    ScaleLayout->addWidget(vertScale, 0, Qt::AlignLeft);


    QGridLayout *MainLayout = new QGridLayout;
    MainLayout->addLayout(BarsLayout,0,0);
    MainLayout->addLayout(ButtonLayout1,1,0,Qt::AlignTop);
    MainLayout->addLayout(ScaleLayout, 0, 1);

    MainLayout->addWidget(view,1,1);
    MainLayout->addWidget(MyStatusBar,3,1);
    MainLayout->addLayout(MasshLayout,2,1);
    MainLayout->addWidget(LabelLogo,2,0,2,1);


    setLayout(MainLayout);

    // === Соединение сигналов ============================================================================

    connect(Button1, SIGNAL(clicked()), this, SLOT(Button1_click()));
    connect(Button2, SIGNAL(clicked()), this, SLOT(Button2_click()));
    connect(Button3, SIGNAL(clicked()), this, SLOT(Button3_click()));
    connect(Button4, SIGNAL(clicked()), this, SLOT(Button4_click()));

    connect(MasshSlider, SIGNAL(valueChanged(int)), this, SLOT(MasshSlider_slide()));
    connect(Dial1, SIGNAL(valueChanged(int)), this, SLOT(Dial1_slide()));

    connect(Timer1, SIGNAL(timeout()), this, SLOT(OnTimer1()) );

    connect(par1, SIGNAL(ChangePar(double&)), this, SLOT(onPar1_ButtonClick(double&)) );
    connect(par2, SIGNAL(ChangePar(double&)), this, SLOT(onPar2_ButtonClick(double&)) );
    connect(par3, SIGNAL(ChangePar(double&)), this, SLOT(onPar3_ButtonClick(double&)) );
    connect(par4, SIGNAL(ChangePar(double&)), this, SLOT(onPar4_ButtonClick(double&)) );
    connect(par5, SIGNAL(ChangePar(double&)), this, SLOT(onPar5_ButtonClick(double&)) );
    connect(par6, SIGNAL(ChangePar(double&)), this, SLOT(onPar6_ButtonClick(double&)) );
    connect(par7, SIGNAL(ChangePar(double&)), this, SLOT(onPar7_ButtonClick(double&)) );
    connect(par8, SIGNAL(ChangePar(double&)), this, SLOT(onPar8_ButtonClick(double&)) );

    connect(vertScale, SIGNAL(ChangeScale(double&)), this, SLOT(onVertScaleCange(double&)) );
    connect(horiScale, SIGNAL(ChangeScale(double&)), this, SLOT(onHoriScaleCange(double&)) );
    connect(veloScale, SIGNAL(ChangeScale(double&)), this, SLOT(onVeloScaleCange(double&)) );

    connect(view, SIGNAL(SignalWheelUp()), this, SLOT(ZoomInView()) );
    connect(view, SIGNAL(SignalWheelDown()), this, SLOT(ZoomOutView()) );


    //отображаем камеру
    ReadParkam(true);



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


/*
    Timer1->start();

    vspomint++;
*/

 //   qDebug() << "lkam: " << kam.lkam << "    bkam: " << kam.bkam << "ho: " << kam.ho << "    nslices: " << kam.nslices << "tau: " << kam.tau << "    fi: " << kam.fi;

  //  WriteParkam();

   // inputq->show();
    kam.inputq->show();





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

    if (kam.Timen >= kam.TimenEnd)
    {
        Timer1->stop();
        MasshSlider->setMaximum(kam.ntimes);
        MasshSlider->setValue(kam.ntimes);
        return;
    }

    kam.InreaseTime();
    QString sa;
    sa.setNum(kam.Timen);



    MyStatusBar->showMessage("Время с начала наполнения:  " + sa + "сек.");

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
/*
    kam.CreateIntervals();
    view->DrawKamera(&kam);
    MasshSlider->setValue(40);
    MasshSlider->setMaximum(400);
*/




}

void Unda::Button4_click()
{
    kam.CreateIntervals();
    Timer1->start();
}

void Unda::ReadParkam(bool ReadDefault)
{


    qDebug() << "Read";
    QString FileName;

    if (ReadDefault)
    {
        FileName = "Default_Parkam.kamre";
    }
    else
    {
        FileName = QFileDialog::getOpenFileName(this,
                                                "Открыть файл параметров камеры", "",
                                                "Параметры камеры (*.kamre);;Все файлы (*)");
    }


    if (FileName.isEmpty())
    {
        return;
    }
    else
    {
    kam.ClearKam();

    QFile filere(FileName);
    filere.open(QIODevice::ReadOnly);
    QDataStream in(&filere);
    in.setVersion(QDataStream::Qt_4_5);
    in >> kam.lkam;
    in >> kam.bkam;
    in >> kam.Hk;
    in >> kam.ho;
    in >> kam.nslices;
    in >> kam.tau;
    in >> kam.fi;
    in >> kam.TimenEnd;

    //Данные в интерполятор о расходе
    int countVal;
    double datum1, datum2;
    double Maxqval, Maxt;
    Maxqval = 0.0; Maxt = 0.0;

    //Очистим интерполятор
    kam.inputq->interpolator->timeq.clear();
    kam.inputq->interpolator->valq.clear();

    //прочитаем количество пар данных
    in >> countVal;

    for(int i =0; i<countVal; i++)
    {
    in >> datum1;
    in >> datum2;

    Maxt = qMax(Maxt, datum1);
    Maxqval = qMax(Maxqval,datum2);

    kam.inputq->interpolator->timeq.append(datum1);
    kam.inputq->interpolator->valq.append(datum2);
    }

    kam.inputq->interpolator->MaxTimeq = Maxt;
    kam.inputq->interpolator->MaxVaq = Maxqval;

    //Добавим данные из интерполятора в таблицу для редактирования на форму inputq
    kam.inputq->ShowDataFromInterpolator();


    qDebug() << "lkam: " << kam.lkam << "Hk: " << kam.Hk << "    bkam: " << kam.bkam << "ho: " << kam.ho << "    nslices: " << kam.nslices << " tau: " << kam.tau << "    fi: " << kam.fi << " TimenEnd: " << kam.TimenEnd ;

    QString sa;
    sa.setNum(kam.lkam);
    par1->EditPar->setText(sa);

    sa.setNum(kam.bkam);
    par2->EditPar->setText(sa);

    sa.setNum(kam.Hk);
    par3->EditPar->setText(sa);

    sa.setNum(kam.ho);
    par4->EditPar->setText(sa);

    sa.setNum(kam.nslices);
    par5->EditPar->setText(sa);

    sa.setNum(kam.tau);
    par6->EditPar->setText(sa);

    sa.setNum(kam.fi);
    par7->EditPar->setText(sa);

    sa.setNum(kam.TimenEnd);
    par8->EditPar->setText(sa);


    //Масштабируем
    float h, w;
    h = view->height();
    w = view->width();

    view->Mahor = qRound( w/(kam.lkam*1.5) );
    horiScale->SetScaleVal(view->Mahor, false);

    view->Mavert = qRound( h/(kam.Hk*1.5) );
    vertScale->SetScaleVal(view->Mavert, false);

    view->MaVelos = 1.0;
    veloScale->SetScaleVal(1.0, false);     //Когда масштаб скорости совпадает с вертикальным то он равен 1


    //Обновим и отрисуем камеру
    kam.CreateIntervals();
    view->DrawKamera(&kam);

    }


}

void Unda::ReadParkamAct()
{
    ReadParkam(false);
}

void Unda::WriteParkam()
{

    qDebug() << "write";

    //Запустим диалог сохранения
    QString FileName = QFileDialog::getSaveFileName(this,
                                                    "Сохранение параметров камеры шлюза", "",
                                                    "Параметры камеры (*.kamre);;Все файлы (*)");

    if (FileName.isEmpty())
    {
        return;
    }


    QFile filewr(FileName);
    filewr.open(QIODevice::WriteOnly);
    QDataStream out(&filewr);
    out.setVersion(QDataStream::Qt_4_5);

    out << kam.lkam << kam.bkam << kam.Hk;
    out << kam.ho << kam.nslices << kam.tau;
    out << kam.fi << kam.TimenEnd;

    //Данные из интерполятора о расходе

    int countVal = kam.inputq->interpolator->timeq.count();  //количество пар значений в интерполяторе

    //Сначала запишем количество пар
    out << countVal;

    for(int i=0; i<countVal; i++)
    {
        out << kam.inputq->interpolator->timeq[i];
        out << kam.inputq->interpolator->valq[i];
    }

}

void Unda::MasshSlider_slide()   // === Функция обработки прокрутки масштаба ==============================
{
   // QMessageBox::about(this, "Slider", "Slider \nIs slide");
/*    float Pos;
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

*/
    float Pos;
    QString sa;
    Pos = MasshSlider->value();
    sa.setNum(Pos);
    MasshLabel->setText(sa);

    kam.ntimes = MasshSlider->value();
    view->DrawKamera(&kam);

    float timen = Pos*kam.tau;
    sa.setNum(timen);
    MyStatusBar->showMessage("Время с начала наполнения:  " + sa + "сек.");

    kam.inputq->ShowTimeWhichInterpolate(timen);


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

void Unda::onPar1_ButtonClick(double &ParamVal)
{
    kam.lkam = ParamVal;
    kam.CreateIntervals();
    view->DrawKamera(&kam);
    qDebug() << "lkam: " << kam.lkam << "Hk: " << kam.Hk << "    bkam: " << kam.bkam << "ho: " << kam.ho << "    nslices: " << kam.nslices << " tau: " << kam.tau << "    fi: " << kam.fi << " TimenEnd: " << kam.TimenEnd ;
}

void Unda::onPar2_ButtonClick(double &ParamVal)
{
    kam.bkam = ParamVal;
    kam.CreateIntervals();
    view->DrawKamera(&kam);
    qDebug() << "lkam: " << kam.lkam << "Hk: " << kam.Hk << "    bkam: " << kam.bkam << "ho: " << kam.ho << "    nslices: " << kam.nslices << " tau: " << kam.tau << "    fi: " << kam.fi << " TimenEnd: " << kam.TimenEnd ;
}

void Unda::onPar3_ButtonClick(double &ParamVal)
{
    kam.Hk = ParamVal;
    kam.CreateIntervals();
    view->DrawKamera(&kam);
    qDebug() << "lkam: " << kam.lkam << "Hk: " << kam.Hk << "    bkam: " << kam.bkam << "ho: " << kam.ho << "    nslices: " << kam.nslices << " tau: " << kam.tau << "    fi: " << kam.fi << " TimenEnd: " << kam.TimenEnd ;
}

void Unda::onPar4_ButtonClick(double &ParamVal)
{
    kam.ho = ParamVal;
    kam.CreateIntervals();
    view->DrawKamera(&kam);
    qDebug() << "lkam: " << kam.lkam << "Hk: " << kam.Hk << "    bkam: " << kam.bkam << "ho: " << kam.ho << "    nslices: " << kam.nslices << " tau: " << kam.tau << "    fi: " << kam.fi << " TimenEnd: " << kam.TimenEnd ;
}

void Unda::onPar5_ButtonClick(double &ParamVal)
{
    kam.nslices = ParamVal;
    kam.CreateIntervals();
    view->DrawKamera(&kam);
   qDebug() << "lkam: " << kam.lkam << "Hk: " << kam.Hk << "    bkam: " << kam.bkam << "ho: " << kam.ho << "    nslices: " << kam.nslices << " tau: " << kam.tau << "    fi: " << kam.fi << " TimenEnd: " << kam.TimenEnd ;
}

void Unda::onPar6_ButtonClick(double &ParamVal)
{
    kam.tau = ParamVal;
    kam.CreateIntervals();
    view->DrawKamera(&kam);
    qDebug() << "lkam: " << kam.lkam << "Hk: " << kam.Hk << "    bkam: " << kam.bkam << "ho: " << kam.ho << "    nslices: " << kam.nslices << " tau: " << kam.tau << "    fi: " << kam.fi << " TimenEnd: " << kam.TimenEnd ;
}

void Unda::onPar7_ButtonClick(double &ParamVal)
{
    kam.fi = ParamVal;
    kam.CreateIntervals();
    view->DrawKamera(&kam);
    qDebug() << "lkam: " << kam.lkam << "Hk: " << kam.Hk << "    bkam: " << kam.bkam << "ho: " << kam.ho << "    nslices: " << kam.nslices << " tau: " << kam.tau << "    fi: " << kam.fi << " TimenEnd: " << kam.TimenEnd ;
}

void Unda::onPar8_ButtonClick(double &ParamVal)
{
    kam.TimenEnd = ParamVal;
    kam.CreateIntervals();
    view->DrawKamera(&kam);
    qDebug() << "lkam: " << kam.lkam << "Hk: " << kam.Hk << "    bkam: " << kam.bkam << "ho: " << kam.ho << "    nslices: " << kam.nslices << " tau: " << kam.tau << "    fi: " << kam.fi << " TimenEnd: " << kam.TimenEnd ;
}

void Unda::onVertScaleCange(double &ScaleVal)
{
    view->Mavert = ScaleVal;
    view->DrawKamera(&kam);
}

void Unda::onHoriScaleCange(double &ScaleVal)
{
    view->Mahor = ScaleVal;
    view->DrawKamera(&kam);
}

void Unda::onVeloScaleCange(double &ScaleVal)
{
    view->MaVelos = ScaleVal;
    view->DrawKamera(&kam);
}

void Unda::ZoomInView()
{
     float k = view->Mavert / view->Mahor;
  //   float k2 = view->MaVelos / view->Mavert;

     view->Mavert = view->Mavert + k;
     view->Mahor = view->Mahor+1;
  //   view->MaVelos = view->MaVelos + k2;

     vertScale->SetScaleVal(view->Mavert,false);
     horiScale->SetScaleVal(view->Mahor, false);
  //   veloScale->SetScaleVal(view->MaVelos, false);

     view->DrawKamera(&kam);

}

void Unda::ZoomOutView()
{
     if(view->Mahor > 1 && view->Mavert > 1)
     {
     float k = view->Mavert / view->Mahor;
   //  float k2 = view->MaVelos / view->Mavert;

     view->Mavert = view->Mavert - k;
     view->Mahor = view->Mahor-1;
   //  view->MaVelos = view->MaVelos - k2;

     vertScale->SetScaleVal(view->Mavert,false);
     horiScale->SetScaleVal(view->Mahor, false);
   //  veloScale->SetScaleVal(view->MaVelos, false);

     view->DrawKamera(&kam);

     }

}
