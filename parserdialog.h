#ifndef PARSERDIALOG_H
#define PARSERDIALOG_H

#include <QDialog>
#include <QtCore>
#include <QTextBrowser>
#include <QTreeWidget>

// Project files
#include "qhexedit.h"
#include "TsDemuxer.h"

// Forward declarations
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
    void setFileName(QString fileName);

private slots:
    void on_parseButton_clicked();
    void on_threeItem_clicked(QTreeWidgetItem* item, int ind);

private:
    void init();
    void parseData();
    void static PATCallback(PsiTable* table, uint16_t pid, void* hdl);
    void static PMTCallback(PsiTable* table, uint16_t pid, void* hdl);
    void parseTransportStream();
    void buildTreeView();
    void buildPatView(QTreeWidgetItem* patRoot);
    void buildPmtView(QTreeWidgetItem* pmtRoot);
    void buildPidView(QTreeWidgetItem* root);
    QTreeWidgetItem* addTreeRoot(QString name,
                                 QString description);

    QTreeWidgetItem* addTreeChild(QTreeWidgetItem *parent,
                                  QString name,
                                  QString description);

    QHexEdit* _hexEdit;
    QTextBrowser* _textBrowser;
    QTreeWidget* _treeWidget;
    QString _fileName;

    // Parsing stuff
    TsDemuxer _tsDemuxer;
    std::vector<uint16_t> _pmtPids;
    PmtTable _pmt;
    bool _addedPmts;
    uint16_t _pmtPid; // Returned PID from PMTCallback

    PatTable _pat;
};



#endif // PARSERDIALOG_H
