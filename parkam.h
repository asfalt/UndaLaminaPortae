#ifndef PARKAM_H
#define PARKAM_H

#include <QWidget>
#include <QtGui>
#include <QDoubleValidator>
#include "kamera.h"


class QPushButton;
class QLineEdit;

class Parkam : public QWidget
{
    Q_OBJECT
public:
//    explicit Parkam(QWidget *parent = 0);
//    Parkam(QWidget *parent = 0, float *parVal);
    Parkam(QWidget *parent, QString nameVal);



    QPushButton *Button_ok;
    QLineEdit *EditPar;
    QLabel *LabelPar;

//    QDoubleValidator *valida;

//    double Param;


    
signals:
    void ChangePar(double &ParamVal);



    
public slots:
    void Button_onClick();
    void EditChangeText();


protected:

    
};

#endif // PARKAM_H
