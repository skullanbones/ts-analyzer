#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
class QPlainTextEdit;
class QSessionManager;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    /*!
     * \brief loadFile Loads a file from file system.
     * \param fileName File to load.
     */
    void loadFile(const QString &fileName);

private slots:
    /*!
     * \brief Present an about widget window.
     */
    void about();

private:
    /*!
     * \brief createActions Creates the menus and toolbar.
     */
    void createActions();

    Ui::MainWindow *ui;

    QPlainTextEdit *textEdit;
};

#endif // MAINWINDOW_H
