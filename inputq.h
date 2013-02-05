#ifndef INPUTQ_H
#define INPUTQ_H

#include <QWidget>
#include <QtGui>
#include <QTableWidget>
#include <QTableWidgetItem>

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsLineItem>

#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QMessageBox>

#include "interpolator.h"

class QLabel;
class QPushButton;
class QTableWidget;
class QGraphicsView;
class QGraphicsScene;
class QGraphicsLineItem;

class Interpolator;

class Inputq : public QWidget
{
    Q_OBJECT
public:
    explicit Inputq(QWidget *parent = 0);

    QPushButton *button1;
    QPushButton *button2;
    QPushButton *button3;
    QPushButton *button4;
    QPushButton *button5;
    QPushButton *button6;

    QTableWidget *table1;
    QLabel *label1;

    QGraphicsView *view;
    QGraphicsScene *scene;
    QGraphicsLineItem *tekLine;


    Interpolator *interpolator;

    float Mahor, Mavert;
    
signals:
    
public slots:
    void onButton1Click();
    void onButton2Click();
    void onButton3Click();
    void onButton4Click();
    void onButton5Click();
    void onButton6Click();
    void OpenTextFile();
    void WriteTextFile();
    void SetDataToInterpolator();
    void PlotGraphik();
    void ShowTimeWhichInterpolate(double TimeVal);
    void ShowDataFromInterpolator();

    
};

#endif // INPUTQ_H
