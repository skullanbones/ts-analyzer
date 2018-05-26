#include <QApplication>
#include <QIcon>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(application);
    QApplication app(argc, argv);
    app.setApplicationName("Transportstream Analyzer");
    app.setOrganizationName("Quantux AB");
    app.setWindowIcon(QIcon(":images/ts-analyzer.ico"));

    // Identify locale and load translation if available
//    QString locale = QLocale::system().name();
//    QTranslator translator;
//    translator.load(QString("qhexedit_") + locale);
//    app.installTranslator(&translator);

    MainWindow *mainWin = new MainWindow;
    mainWin->show();

    return app.exec();
}
