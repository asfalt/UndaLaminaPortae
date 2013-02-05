#include "parkam.h"
#include <QtGui>

#include <QDebug>
#include <QMessageBox>



Parkam::Parkam(QWidget *parent, QString nameVal)
{
    //setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    Button_ok = new QPushButton("ok");
    EditPar = new QLineEdit();
    LabelPar = new QLabel(nameVal);

    EditPar->setMaximumWidth(100);
    EditPar->setStyleSheet("color: black; background-color: beige");
    Button_ok->setFixedWidth(40);
    Button_ok->setEnabled(false);

    //Подложка
    QLabel *la = new QLabel(" ", this);
    QPixmap pixmap("image3.png");
    la->setFixedHeight(75);
    la->setFixedWidth(500);
    la->setPixmap(pixmap);



    QGridLayout *magri = new QGridLayout;

    QHBoxLayout *Hbox = new QHBoxLayout;
    Hbox->addSpacing(10);
    Hbox->addWidget(EditPar,2);
    Hbox->addWidget(Button_ok,2);
    Hbox->addStretch();

    QVBoxLayout *Vbox = new QVBoxLayout;
    Vbox->addWidget(LabelPar);
    Vbox->addLayout(Hbox);

    magri->addLayout(Vbox,0,0,1,1);

    setLayout(magri);
    setFixedWidth(250);

    connect(Button_ok, SIGNAL(clicked()), this, SLOT(Button_onClick()) );
    connect(EditPar, SIGNAL(textEdited(QString)), this, SLOT(EditChangeText()) );

/*
    valida = new QDoubleValidator(-5, 2999, 5;
    valida->setNotation(QDoubleValidator::StandardNotation);
    EditPar->setValidator(valida);
*/
}

void Parkam::EditChangeText()
{
    Button_ok->setEnabled(true);
    Button_ok->setStyleSheet("color: rgb(200, 0, 200)");
    EditPar->setStyleSheet("color: brown; background-color: white");
    LabelPar->setStyleSheet("color: red");
}

void Parkam::Button_onClick()
{
    QString sa;
    sa = EditPar->text();


    double val;
    val = sa.toDouble();

//    qDebug() << "sa: " << sa << "   val: " << val;

    Button_ok->setEnabled(false);
    Button_ok->setStyleSheet("color: rgb(160, 160, 160)");
    EditPar->setStyleSheet("color: black; background-color: beige");
    LabelPar->setStyleSheet("color: black");

    emit ChangePar(val);



}



