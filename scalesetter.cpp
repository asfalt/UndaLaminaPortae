#include "scalesetter.h"


ScaleSetter::ScaleSetter(QWidget *parent, QString nameVal) :
    QWidget(parent)
{
    scalePar = 1.0;

    QIcon ico;
    QSize size;

    size.setHeight(22);
    size.setWidth(22);

    LabelName = new QLabel(nameVal);
    LabelScale = new QLabel("1.0");
    ButtonZoomIn = new QPushButton();
    ButtonZoomOut = new QPushButton();
    timerRepeat = new QTimer(this);
    timerWait = new QTimer(this);

    ButtonZoomIn->setFixedSize(32,32);
    ButtonZoomOut->setFixedSize(32,32);

    ico.addFile("zoom_in.png");

    ButtonZoomIn->setIcon(ico);
    ButtonZoomIn->setIconSize(size);

    ico.addFile("zoom_out.png");
    ButtonZoomOut->setIcon(ico);
    ButtonZoomOut->setIconSize(size);

    ButtonZoomIn->setStyleSheet("border-style: outset;  border-width: 2px; border-color: grey; border-radius: 10px;");
    ButtonZoomOut->setStyleSheet("border-style: outset;  border-width: 2px; border-color: grey; border-radius: 10px;");

    LabelScale->setMinimumWidth(20);



    //Подложка
    QLabel *la = new QLabel(" ", this);
    QPixmap pixmap("image3.png");
    la->setFixedHeight(54);
    la->setFixedWidth(120);
    la->setPixmap(pixmap);
    la->setFrameStyle(QFrame::Raised);
    la->setStyleSheet("border-style: outset");



    QGridLayout *MainLayout = new QGridLayout;
    QHBoxLayout *SecondLayout = new QHBoxLayout;


    SecondLayout->addWidget(LabelScale, 0, Qt::AlignHCenter);
    SecondLayout->addWidget(ButtonZoomIn);
    SecondLayout->addWidget(ButtonZoomOut);



    MainLayout->addWidget(LabelName, 0, 0, Qt::AlignHCenter);
    MainLayout->addLayout(SecondLayout, 1, 0);
    MainLayout->setSpacing(0);
    MainLayout->setMargin(2);


    this->setLayout(MainLayout);
    this->setFixedHeight(54);
    this->setFixedWidth(120);

    connect(ButtonZoomIn, SIGNAL(pressed()), this, SLOT(OnBbuttonZoomInClick()) );
    connect(ButtonZoomOut, SIGNAL(pressed()), this, SLOT(OnBbuttonZoomOutClick()) );
    connect(timerRepeat, SIGNAL(timeout()), this, SLOT(OnTimerRepeat()) );
    connect(ButtonZoomIn, SIGNAL(released()), this, SLOT(OnButtonZoomInRelease()) );
    connect(ButtonZoomOut, SIGNAL(released()), this, SLOT(OnButtonZoomOutRelease()) );


    zoomIn = false;
    zoomOut = false;
    released = true;

}

void ScaleSetter::OnBbuttonZoomInClick()
{
    zoomIn = true;
    zoomOut = false;

    if(scalePar < 1.0)
    {
        scalePar = scalePar*2.0;
    }
    else
    {
        scalePar = qRound(scalePar);
        scalePar = scalePar + 1.0;
    }
    QString sa;
    sa.setNum(scalePar);
    LabelScale->setText(sa);

    emit ChangeScale(scalePar);

    if(released)
    {
    released = false;
    timerWait->singleShot(300, this, SLOT(StartTimerRepeat()));
    }
}

void ScaleSetter::OnBbuttonZoomOutClick()
{
    zoomIn = false;
    zoomOut = true;

    if(scalePar > 1.0)
    {
        scalePar = scalePar - 1.0;

    }
    else
    {
        scalePar = scalePar*0.5;
    }
    QString sa;
    sa.setNum(scalePar, 'g', 2);
    LabelScale->setText(sa);

    emit ChangeScale(scalePar);

    if(released)
    {
    released = false;
    timerWait->singleShot(300, this, SLOT(StartTimerRepeat()));
    }
}

void ScaleSetter::SetScaleVal(double ScaleVal, bool isEmit)
{
    scalePar = ScaleVal;
    QString sa;
    sa.setNum(scalePar);
    LabelScale->setText(sa);
    if(isEmit)
    {
    emit ChangeScale(scalePar);
    }
}

void ScaleSetter::OnTimerRepeat()
{
    if(!released)
    {
       if(zoomIn)
       {
           OnBbuttonZoomInClick();
       }
       else
       {
           OnBbuttonZoomOutClick();
       }
    }
    else
    {
       timerRepeat->stop();
    }

}

void ScaleSetter::StartTimerRepeat()
{
    timerRepeat->start(50);
}

void ScaleSetter::OnButtonZoomInRelease()
{
    timerRepeat->stop();
    released = true;
}

void ScaleSetter::OnButtonZoomOutRelease()
{
    timerRepeat->stop();
    released = true;
}
