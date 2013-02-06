#ifndef UNDALIMINAPORTAE_H
#define UNDALIMINAPORTAE_H

#include <QtGui>
#include <QWidget>
#include <QVector>
#include <QTimer>
#include <QAction>
#include <QMenu>
#include <QToolBar>
#include <QStatusBar>

#include "view.h"
#include "Slice.h"
#include "Interval.h"

#include "kamera.h"
#include "parkam.h"

#include "scalesetter.h"


    // === Используемые классы ============================================================================
class QWidget;
class QLabel;
class QPushButton;
class View;
class QSlider;
class QDial;
class QTextEdit;

class QAction;

class Parkam;
class ScaleSetter;


    // === Объявление основного класса ====================================================================

class Unda :public QWidget
{
    Q_OBJECT

public:
    Unda(QWidget *parent = 0);



    // === Константы ======================================================================================
    float g;                  //Ускорение свободного падения



    // === Используемые переменные ========================================================================


    int vspomint;
    float vspomfloat;



public slots:
    void Button1_click();
    void Button2_click();
    void Button3_click();
    void Button4_click();
    void MasshSlider_slide();
    void Dial1_slide();
    void OnTimer1();

    void onPar1_ButtonClick(double &ParamVal);
    void onPar2_ButtonClick(double &ParamVal);
    void onPar3_ButtonClick(double &ParamVal);
    void onPar4_ButtonClick(double &ParamVal);
    void onPar5_ButtonClick(double &ParamVal);
    void onPar6_ButtonClick(double &ParamVal);
    void onPar7_ButtonClick(double &ParamVal);
    void onPar8_ButtonClick(double &ParamVal);

    void onVertScaleCange(double &ScaleVal);
    void onHoriScaleCange(double &ScaleVal);
    void onVeloScaleCange(double &ScaleVal);


    void ReadParkamAct();
    void WriteParkam();   //Функция записи параметров камеры в файл

    void ZoomInView();
    void ZoomOutView();

private:
    void ReadParkam(bool ReadDefault);    //Функция чтения параметров камеры из файла
    void closeEvent(QCloseEvent *);




    QPushButton *Button1;
    QPushButton *Button2;
    QPushButton *Button3;
    QPushButton *Button4;
    View *view;
    QSlider *MasshSlider;
    QLabel *MasshLabel;
    QDial *Dial1;
    QTimer *Timer1;

    //Параметры камеры

    Parkam *par1;
    Parkam *par2;
    Parkam *par3;
    Parkam *par4;
    Parkam *par5;
    Parkam *par6;
    Parkam *par7;
    Parkam *par8;

    ScaleSetter *vertScale;
    ScaleSetter *horiScale;
    ScaleSetter *veloScale;






    QVector<Slice> VecSlices;       //Вектор сечений

    Kamera kam;

    QMenuBar *MyMenuBar;
    QMenu *FileMenu;
    QAction *OpenAct;
    QAction *SaveAct;
    QToolBar *FileToolBar;
    QStatusBar *MyStatusBar;


};

#endif // UNDALIMINAPORTAE_H
