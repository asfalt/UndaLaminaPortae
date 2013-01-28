#include <QtGui>
#include "UndaLiminaPortae.h"

int main(int argc, char *argv[])
{
    QApplication aap(argc, argv);
    Unda unda;
    unda.show();

    return aap.exec();
}
