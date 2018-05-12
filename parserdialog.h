#ifndef PARSERDIALOG_H
#define PARSERDIALOG_H

#include <QDialog>
#include <QtCore>
#include <QTextBrowser>

// Project files
#include "qhexedit.h"
#include "TsDemuxer.h"

namespace Ui {
    class Dialog;
}

class ParserDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ParserDialog(QHexEdit *hexEdit, QWidget *parent = 0);
    ~ParserDialog();
    Ui::Dialog* _ui;

private slots:
    void on_parseButton_clicked();

private:
    void init();
    void parseData();
    void printData(std::string str);
    void static PATCallback(PsiTable* table);
    void parsePat();

    QHexEdit* _hexEdit;
    QTextBrowser* _textBrowser;
    TsDemuxer g_tsDemux;
};



#endif // PARSERDIALOG_H
