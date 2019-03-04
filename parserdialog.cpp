#include <QMessageBox>
#include <QTreeWidgetItem>

// proj files
#include "parserdialog.h"
#include "ui_parserdialog.h"

#include <iostream>
#include <string>
#include <sstream>


ParserDialog::ParserDialog(QHexEdit *hexEdit, QWidget *parent) :
    QDialog(parent),
    _ui(new Ui::Dialog)
{
  _ui->setupUi(this);
  init();
  _hexEdit = hexEdit;
}

ParserDialog::~ParserDialog()
{
  delete _ui;
}

void ParserDialog::setFileName(QString fileName)
{
    _fileName = fileName;
}

/*****************************************************************************/
/* Private Slots */
/*****************************************************************************/
void ParserDialog::on_parseButton_clicked()
{
    qDebug() << "parseButton clicked->start parsing.";
    parseData();
    buildTreeView();
}

/*****************************************************************************/
/* Private Methods */
/*****************************************************************************/
void ParserDialog::init()
{
    _treeWidget = _ui->treeWidget;
    _pmtPids.clear();
    _pmtTables.clear();
}


void ParserDialog::parseData()
{
    qDebug() << "(C) Copyright 2018 All rights reserverd Quantux AB.";
    // Start parsing
    QByteArray data = _hexEdit->data();
    if (data.size()) {
        qDebug() << "1st byte: " << QString::number(data.at(0), 16);

        if (data.at(0) != 0x47)
        {
            qDebug() << "ERROR: 1'st byte not in sync!!!";
        }

        // Start parsing 1st packet data
        _tsUtil.parseTransportStreamData((const uint8_t*)data.data(), data.size());

        _pat =  _tsUtil.getPatTable();
        _pmtPids = _tsUtil.getPmtPids();
        _pmtTables = _tsUtil.getPmtTables();
        _pmt = _pmtTables[_pmtPids.at(0)]; // TODO assumes SPTS
    }
}


void ParserDialog::buildTreeView()
{
    _treeWidget->clear();
    _treeWidget->setColumnCount(2);
    _treeWidget->setColumnWidth(0, 300);
    QStringList ColumnNames;
    ColumnNames << "Description" << "Data";

    _treeWidget->setHeaderLabels(ColumnNames);

    // Add root nodes
    QTreeWidgetItem* root = addTreeRoot(_fileName, "");

    // Add child nodes
    QTreeWidgetItem* psiRoot = addTreeChild(root, "PSI (Program Specific Information)", "");
    QTreeWidgetItem* patRoot = addTreeChild(psiRoot, "PAT (Program Association Table)", "PID: " + QString::number(mpeg2ts::TS_PACKET_PID_PAT));

    // TODO assumes SPTS
    QTreeWidgetItem* pmtRoot = addTreeChild(psiRoot, "PMT (Program Map Table)", "PID: " + QString::number(_pmtPids.at(0)));
    buildPidView(root);
    buildPatView(patRoot);
    buildPmtView(pmtRoot);
}

void ParserDialog::buildPatView(QTreeWidgetItem* patRoot)
{
    addTreeChild(patRoot, "table_id", QString::number(_pat.table_id));
    addTreeChild(patRoot, "section_syntax_indicator", QString::number(_pat.section_syntax_indicator));
    addTreeChild(patRoot, "section_length", QString::number(_pat.section_length));
    addTreeChild(patRoot, "transport_stream_id", QString::number(_pat.transport_stream_id));
    addTreeChild(patRoot, "version_number", QString::number(_pat.version_number));
    addTreeChild(patRoot, "current_next_indicator", QString::number(_pat.current_next_indicator));
    addTreeChild(patRoot, "section_number", QString::number(_pat.section_number));
    addTreeChild(patRoot, "last_section_number", QString::number(_pat.last_section_number));
}

void ParserDialog::buildPmtView(QTreeWidgetItem* pmtRoot)
{
    addTreeChild(pmtRoot, "table_id", QString::number(_pmt.table_id));
    addTreeChild(pmtRoot, "section_syntax_indicator", QString::number(_pmt.section_syntax_indicator));
    addTreeChild(pmtRoot, "section_length", QString::number(_pmt.section_length));
    addTreeChild(pmtRoot, "transport_stream_id", QString::number(_pmt.transport_stream_id));
    addTreeChild(pmtRoot, "version_number", QString::number(_pmt.version_number));
    addTreeChild(pmtRoot, "current_next_indicator", QString::number(_pmt.current_next_indicator));
    addTreeChild(pmtRoot, "section_number", QString::number(_pmt.section_number));
    addTreeChild(pmtRoot, "last_section_number", QString::number(_pmt.last_section_number));
    addTreeChild(pmtRoot, "PCR_PID", QString::number(_pmt.PCR_PID));
    addTreeChild(pmtRoot, "program_info_length", QString::number(_pmt.program_info_length));

    // Streams
    QTreeWidgetItem* streamRoot = addTreeChild(pmtRoot, "streams", "");

    for (mpeg2ts::StreamTypeHeader stream : _pmt.streams)
    {
        addTreeChild(streamRoot, "stream_type", QString::fromStdString(mpeg2ts::StreamTypeToString[stream.stream_type]) + ", (" + QString::number(stream.stream_type) + ")");
        addTreeChild(streamRoot, "elementary_PID", QString::number(stream.elementary_PID));
        addTreeChild(streamRoot, "ES_info_length", QString::number(stream.ES_info_length));
    }
}

void ParserDialog::buildPidView(QTreeWidgetItem* root)
{
    QTreeWidgetItem* pidRoot = addTreeChild(root, "PIDs (Packet IDentifiers)", "");
    addTreeChild(pidRoot, "PAT (Program Association Table)", QString::number(mpeg2ts::TS_PACKET_PID_PAT));
    uint16_t pmtPid = _pmtPids.at(0);
    addTreeChild(pidRoot, "PMT (Program Map Table)", QString::number(pmtPid));

    // Add PRC PID
    addTreeChild(pidRoot, "PCR (Program Clock Reference)", QString::number(_pmt.PCR_PID));

    // Add streams
    for (mpeg2ts::StreamTypeHeader stream : _pmt.streams)
    {
        addTreeChild(pidRoot, QString::fromStdString(mpeg2ts::StreamTypeToString[stream.stream_type]) + ", (" + QString::number(stream.stream_type) + ")",
                QString::number(stream.elementary_PID));
    }
}


QTreeWidgetItem* ParserDialog::addTreeRoot(QString name,
                                           QString description)
{
    // QTreeWidgetItem(QTreeWidget * parent, int type = Type)
    QTreeWidgetItem *treeItem = new QTreeWidgetItem(_treeWidget);
    treeItem->setText(0, name);
    treeItem->setText(1, description);
    return treeItem;
}


QTreeWidgetItem* ParserDialog::addTreeChild(QTreeWidgetItem *parent,
                                QString name,
                                QString description)
{
    // QTreeWidgetItem(QTreeWidget * parent, int type = Type)
    QTreeWidgetItem *treeItem = new QTreeWidgetItem();

    // QTreeWidgetItem::setText(int column, const QString & text)
    treeItem->setText(0, name);
    treeItem->setText(1, description);

    // QTreeWidgetItem::addChild(QTreeWidgetItem * child)
    parent->addChild(treeItem);
    return treeItem;
}
