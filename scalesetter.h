#ifndef SCALESETTER_H
#define SCALESETTER_H

#include <QWidget>
#include <QtGui>
#include <QLabel>
#include <QPushButton>

#include <QTimer>

class QPushButton;
class QLabel;

class ScaleSetter : public QWidget
{
    Q_OBJECT
public:
    ScaleSetter(QWidget *parent, QString nameVal);
    double scalePar;

    QLabel *LabelName;
    QLabel *LabelScale;
    QPushButton *ButtonZoomIn;
    QPushButton *ButtonZoomOut;

    QTimer *timerRepeat;
    QTimer *timerWait;
    bool zoomIn, zoomOut;
    bool released;

    void SetScaleVal(double ScaleVal, bool isEmit);
    
signals:
    void ChangeScale(double &ScaleVal);
    
public slots:
    void OnBbuttonZoomInClick();
    void OnBbuttonZoomOutClick();
    void OnTimerRepeat();

    void OnButtonZoomInRelease();
    void OnButtonZoomOutRelease();
    void StartTimerRepeat();

    
};

#endif // SCALESETTER_H
