#include "mainwindow.h"
#include <QApplication>
#include <QtDebug>

#include "TsStandards.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qDebug() << "Hello World!" << TS_PACKET_SYNC_BYTE;

    MainWindow w;
    w.show();

    return a.exec();
}
