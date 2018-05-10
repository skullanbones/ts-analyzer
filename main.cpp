#include "mainwindow.h"
#include <QApplication>
#include <QtDebug>

#include "TsStandards.h"

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(application);

    QApplication app(argc, argv);
    QCoreApplication::setOrganizationName("Company AB");
    QCoreApplication::setApplicationName("Transportstream Analyzer");
    qDebug() << "Hello World!" << TS_PACKET_SYNC_BYTE;

    MainWindow w;
    w.show();

    return app.exec();
}
