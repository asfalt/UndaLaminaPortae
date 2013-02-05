#include <QtGui>
#include <QTextCodec>
#include "UndaLiminaPortae.h"

int main(int argc, char *argv[])
{
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForCStrings(codec);


    QApplication aap(argc, argv);
    Unda unda;
    unda.show();

    return aap.exec();
}
