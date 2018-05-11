#ifndef PARSERDIALOG_H
#define PARSERDIALOG_H

#include <QDialog>
#include <QtCore>
#include "qhexedit2/qhexedit.h"

namespace Ui {
    class Dialog;
}

class ParserDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ParserDialog(QHexEdit *hexEdit, QWidget *parent = 0);
    ~ParserDialog();
    Ui::Dialog *_ui;

private slots:
    void on_parseButton_clicked();

private:
    QHexEdit *_hexEdit;
};



#endif // PARSERDIALOG_H
