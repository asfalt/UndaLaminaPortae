#include "inputq.h"
#include <QDebug>

Inputq::Inputq(QWidget *parent) :
    QWidget(parent)
{

    //Подложка
    QLabel *la = new QLabel(" ", this);
    QPixmap pixmap("podd2.png");
    la->setFixedHeight(1500);
    la->setFixedWidth(2000);
    la->setPixmap(pixmap);


    label1 = new QLabel("qqqqqqqqqqqqqqq");
    button1 = new QPushButton("Из файла");
    button2 = new QPushButton("Сохранить");
    button3 = new QPushButton("Добавить строку");
    button4 = new QPushButton("Применить");
    button5 = new QPushButton("Button5");
    button6 = new QPushButton("Button6");
    table1 = new QTableWidget(1, 2, this);

    interpolator = new Interpolator;

    view = new QGraphicsView(this);
    view->setFixedWidth(340);
    scene = new QGraphicsScene(this);
    view->setScene(scene);


    QIcon ico;
    ico.addFile("open.png");
    button1->setIcon(ico);

    ico.addFile("save.png");
    button2->setIcon(ico);

    button4->setStyleSheet("background-color: rgb(243, 177, 110); color: black");



    QVBoxLayout *VBoxLayout1 = new QVBoxLayout;
    QHBoxLayout *HBoxLayout = new QHBoxLayout;
    QGridLayout *MainLayout = new QGridLayout;

    HBoxLayout->addWidget(button1);
    HBoxLayout->addWidget(button2);

    VBoxLayout1->addWidget(view);
    VBoxLayout1->addLayout(HBoxLayout);

    VBoxLayout1->addWidget(button3);
    VBoxLayout1->addWidget(button4);
//    VBoxLayout1->addWidget(button5);
//    VBoxLayout1->addWidget(button6);

    MainLayout->addLayout(VBoxLayout1,0,0);
    MainLayout->addWidget(table1,0,1);

    connect(button1, SIGNAL(clicked()), this, SLOT(onButton1Click()) );
    connect(button2, SIGNAL(clicked()), this, SLOT(onButton2Click()) );
    connect(button3, SIGNAL(clicked()), this, SLOT(onButton3Click()) );
    connect(button4, SIGNAL(clicked()), this, SLOT(onButton4Click()) );
    connect(button5, SIGNAL(clicked()), this, SLOT(onButton5Click()) );
    connect(button6, SIGNAL(clicked()), this, SLOT(onButton6Click()) );


    this->setLayout(MainLayout);
    this->setFixedSize(600, 400);
    this->setWindowTitle("Установка значения расхода");

    QTableWidgetItem *item1 = new QTableWidgetItem();
    item1->setText("Time");
    table1->setHorizontalHeaderItem(0,item1);
    QTableWidgetItem *item2 = new QTableWidgetItem();
    item2->setText("Q");
    table1->setHorizontalHeaderItem(1,item2);

    Mahor = 0.0;
    Mavert= 0.0;

}

void Inputq::onButton1Click()
{

    OpenTextFile();

}

void Inputq::onButton2Click()
{

    WriteTextFile();

}

void Inputq::onButton3Click()
{
    //Добавляем строку
    table1->setRowCount(table1->rowCount() + 1);
}

void Inputq::onButton4Click()
{
    //Применяем значения
    SetDataToInterpolator();
    PlotGraphik();
}

void Inputq::onButton5Click()
{
    qDebug() << "Button5Click ";
    interpolator->ShowData();
}

void Inputq::onButton6Click()
{
    qDebug() << "Button6Click ";
    interpolator->InterpolateLinear(81);
    ShowTimeWhichInterpolate(81);

}



void Inputq::OpenTextFile()
{
    QString FileName;
    FileName = QFileDialog::getOpenFileName(this,
                                            "Открыть текстовый файл со значениями расхода", "",
                                            "Текстовые файлы (*.txt);;Все файлы (*)");


    if(FileName.isEmpty())
    {
        return;
    }
    else
    {
        //Очистим таблицу
        table1->setRowCount(0);
        table1->setColumnCount(0);

        table1->setRowCount(1);
        table1->setColumnCount(2);

        //Назначим заголовки
        QTableWidgetItem *item1 = new QTableWidgetItem();
        item1->setText("Time");
        table1->setHorizontalHeaderItem(0,item1);
        QTableWidgetItem *item2 = new QTableWidgetItem();
        item2->setText("Q");
        table1->setHorizontalHeaderItem(1,item2);

         //Прочитаем файл
        QFile filere(FileName);
        filere.open(QIODevice::ReadOnly);
        QTextStream in(&filere);

        QString sa = "one";
        int column = 0;
        int row = 0;

        while(true)
        {
            in >> sa;
            if(sa != "")
            {
                if(column == 2)
                {
                    column = 0;
                    row++;
                    table1->setRowCount(row+1);
                }

         //       qDebug() << "read col  " << column << "  read row  " << row << sa;
                QTableWidgetItem *item = new QTableWidgetItem();
                item->setText(sa);
                table1->setItem(row, column, item);

                column++;
             }
             else
             {
                //Добавим данные в интерполятор
                SetDataToInterpolator();
                PlotGraphik();
                return;
             }
         }

    }

}

void Inputq::WriteTextFile()
{
    QString FileName = QFileDialog::getSaveFileName(this,
                                                    "Сохранить текстовый файл со значениями расхода", "",
                                                    "Текстовые файлы (*.txt);;Все файлы (*)");

    if (FileName.isEmpty())
    {
        return;
    }

    QFile filewr(FileName);
    filewr.open(QIODevice::WriteOnly);
    QTextStream out(&filewr);

    QString sa, sa2;
    int row = table1->rowCount();

    for(int i = 0; i<=row; i++)
    {
        if(table1->item(i,0) && table1->item(i,1))
        {
        sa = table1->item(i,0)->text();
        sa2 = table1->item(i,1)->text();

        out << sa << " " << sa2;
        }
        out << "\n";
    }

}

void Inputq::SetDataToInterpolator()
{
    //Сначала очистим интерполятор
    interpolator->timeq.clear();
    interpolator->valq.clear();

    QString sa1, sa2;
    double Maxqval, Maxt;
    Maxqval = 0.0; Maxt = 0.0;

    int row = table1->rowCount();

    for(int i = 0; i<=row; i++)
    {
        if(table1->item(i,0) && table1->item(i,1))
        {
        sa1 = table1->item(i,0)->text();
        sa2 = table1->item(i,1)->text();

        bool ok1, ok2;
        double d1, d2;



        d1 = sa1.toDouble(&ok1);
        d2 = sa2.toDouble(&ok2);

           if(ok1 && ok2)
           {
              interpolator->timeq.append(d1);
              interpolator->valq.append(d2);

              Maxt = qMax(Maxt, d1);
              Maxqval = qMax(Maxqval,d2);

           }
           else
           {
               QString sa;
               sa.setNum(i+1);
               QMessageBox::about(this, "Нечисловое значение.", "Проверьте правильность введённых данных\nв строке "+sa + " :)\nДанные этой строки не добавлены.");
           }
        }
    }
    interpolator->MaxTimeq = Maxt;
    interpolator->MaxVaq = Maxqval;
}

void Inputq::PlotGraphik()
{
    scene->clear();

    //Масштабируем
    float h, w;
    float Ti, Q, TiPrew, QPrew;
    TiPrew = 0.0;
    QPrew = 0.0;

    Ti = interpolator->MaxTimeq;
    Q = interpolator->MaxVaq;


 //   h = view->height();              //поскольку размеры окна постоянны так лучше
 //   w = view->width();               //не рисует правильно график когда это окно еще не показано
    h = 246;
    w = 340;

    Mahor =  w/(Ti*1.5);
    Mavert =  h/(Q*1.2);

    scene->addLine(0,0,Ti*Mahor*1.1,0);
    scene->addLine(0,0,0,-Q*Mavert*1.1);

    QPen pen1;
    pen1.setColor(Qt::red);
    tekLine =  scene->addLine(0, 0, 0, -Q*Mavert*1.1, pen1);

    for(int i=0; i<interpolator->timeq.count(); i++)
    {
        Ti = interpolator->timeq[i] * Mahor;
        Q = -(interpolator->valq[i] * Mavert);

        scene->addEllipse(Ti-2, Q-2, 4, 4);
        scene->addLine(TiPrew, QPrew, Ti, Q);

        TiPrew = Ti;
        QPrew = Q;
    }

}

void Inputq::ShowTimeWhichInterpolate(double TimeVal)
{
    tekLine->setPos(TimeVal*Mahor,2);
}

void Inputq::ShowDataFromInterpolator()
{
    QString sa1, sa2;

    int countVal = interpolator->timeq.count();  //количество пар значений в интерполяторе

    //Очистим таблицу
    table1->setRowCount(0);
    table1->setColumnCount(0);

    table1->setRowCount(countVal);
    table1->setColumnCount(2);

    //Назначим заголовки
    QTableWidgetItem *item1 = new QTableWidgetItem();
    item1->setText("Time");
    table1->setHorizontalHeaderItem(0,item1);
    QTableWidgetItem *item2 = new QTableWidgetItem();
    item2->setText("Q");
    table1->setHorizontalHeaderItem(1,item2);

    for(int i=0; i<countVal; i++)
    {
        QTableWidgetItem *item1 = new QTableWidgetItem();  //добавим ячейку для таблицы
        QTableWidgetItem *item2 = new QTableWidgetItem();  //добавим ячейку для таблицы


        sa1.setNum(interpolator->timeq[i]);
        sa2.setNum(interpolator->valq[i]);

        item1->setText(sa1);
        item2->setText(sa2);

        table1->setItem(i,0,item1);
        table1->setItem(i,1,item2);
    }

    PlotGraphik();
}
