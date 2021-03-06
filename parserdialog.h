#ifndef PARSERDIALOG_H
#define PARSERDIALOG_H

#include <QDialog>
#include <QtCore>
#include <QTextBrowser>
#include <QTreeWidget>

// Project files
#include "qhexedit.h"
#include "mpeg2ts/mpeg2ts.h"
#include "mpeg2ts/TsUtilities.h"

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

private:
    void init();
    void parseData();

    void buildTreeView();
    void buildPatView(QTreeWidgetItem* patRoot);
    void buildPmtView(QTreeWidgetItem* pmtRoot);
    void buildPidView(QTreeWidgetItem* root);
    void buildMediaView(QTreeWidgetItem* root);
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
    tsutil::TsUtilities _tsUtil;
    std::vector<uint16_t> _pmtPids;
    mpeg2ts::PmtTable _pmt;
    std::map<int, mpeg2ts::PmtTable> _pmtTables;
    mpeg2ts::PatTable _pat;
    tsutil::VideoMediaInfo _videoInfo;
};



#endif // PARSERDIALOG_H
