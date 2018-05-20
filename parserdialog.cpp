#include <QMessageBox>
#include <QTreeWidgetItem>

// proj files
#include "parserdialog.h"
#include "ui_parserdialog.h"
#include "TsPacketInfo.h"
#include "TsParser.h"

#include <iostream>
#include <string>

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

/*****************************************************************************/
/* Private Slots */
/*****************************************************************************/
void ParserDialog::on_parseButton_clicked()
{
    qDebug() << "parseButton clicked->start parsing.";
    parseData();
    parseTransportStream();
}

/*****************************************************************************/
/* Private Methods */
/*****************************************************************************/
void ParserDialog::init()
{
    _textBrowser = _ui->textBrowser;
    _treeWidget = _ui->treeWidget;
}


void ParserDialog::parseData()
{
    printData("(C) Copyright 2018 All rights reserverd Quantux AB.");
    // Start parsing
    QByteArray data = _hexEdit->data();
    if (data.size()) {
        qDebug() << "1st byte: " << QString::number(data.at(0), 16);

        if (data.at(0) != 0x47)
        {
            qDebug() << "ERROR: 1'st byte not in sync!!!";
        }

        // Start parsing 1st packet data
        TsPacketInfo info;
        TsParser parser;
        parser.parseTsPacketInfo((const uint8_t*)data.data(), info);
        std::cout << info << std::endl;
        std::stringstream buffer;
        buffer << info << std::endl;
        printData(buffer.str());
    }
}

void ParserDialog::printData(std::string inStr)
{
    QString str = QString::fromUtf8(inStr.c_str());
    _textBrowser->append(str);
}

void ParserDialog::PATCallback(PsiTable* table, void* hdl)
{
    auto pat = dynamic_cast<PatTable*>(table);
    ParserDialog* instance = reinterpret_cast<ParserDialog*>(hdl);

    // Print out data
    std::stringstream buffer;
    buffer << *pat << std::endl;
    std::string inStr = buffer.str();
    QString str = QString::fromUtf8(inStr.c_str());
    qDebug() << str;
    //instance->printData(buffer.str());

    // Book keep all PATs for quality check
    instance->_pmtPids.push_back(pat->programs[0].program_map_PID);
    float average = accumulate( instance->_pmtPids.begin(), instance->_pmtPids.end(), 0.0)/instance->_pmtPids.size();
    instance->printData("average:");
    instance->printData(std::to_string(average));
    // Book keep number of programs, TODO use map std:pair<int, int> for PID, numberPrograms

    // Register PMT callback only if PMT PID changed
    int newPmtPid = pat->programs[0].program_map_PID;
    if (instance->_gPmtPid != newPmtPid)
    {
        instance->_gPmtPid = newPmtPid;
        // std::cout << "Single Program Transport Stream PID: " << g_PMTPID << std::endl;
        instance->_tsDemuxer.addPsiPid(instance->_gPmtPid, std::bind(&ParserDialog::PMTCallback, std::placeholders::_1, std::placeholders::_2), hdl);
    }
}

void ParserDialog::PMTCallback(PsiTable* table, void* hdl)
{
    auto pmt = dynamic_cast<PmtTable*>(table);
    ParserDialog* instance = reinterpret_cast<ParserDialog*>(hdl);

    // Print out data
    std::stringstream buffer;
    buffer << *pmt << std::endl;
    std::string inStr = buffer.str();
    QString str = QString::fromUtf8(inStr.c_str());
    qDebug() << str;
    instance->printData(buffer.str());

    // TODO for now we always use the last PMT in the stream.
    // TODO need use something better...
    instance->_pmt = *pmt; //  copy instance
}

void ParserDialog::parseTransportStream()
{
    QByteArray data = _hexEdit->data();
    uint64_t count = 0;
    int readIndex = 0;
    const uint8_t* packetsData = (const uint8_t*)data.data();
    // Register callback
    _tsDemuxer.addPsiPid(0, std::bind(&ParserDialog::PATCallback, std::placeholders::_1, std::placeholders::_2), (void*) this);

    if ((data.at(0) != 0x47) || (data.size() <= 0))
    {
        qDebug() << "ERROR: 1'st byte not in sync!!!";
        return;
    }

    while (readIndex < data.size())
    {
        _tsDemuxer.demux(packetsData + readIndex);
        readIndex += TS_PACKET_SIZE;
        count++;
    }
    qDebug() << "Found " << count << " ts packets.";

    buildTreeView();
}

void ParserDialog::buildTreeView()
{
    _treeWidget->setColumnCount(3);
    QStringList ColumnNames;
    ColumnNames << "Table" << "PID" << "Description";

    _treeWidget->setHeaderLabels(ColumnNames);

    // TODO dynamic build upp depending on parsing findings...
    // Add root nodes
    QTreeWidgetItem* root = addTreeRoot("PAT", 0, "Program Association Table");
    QTreeWidgetItem* pmtRoot = addTreeChild(root, "PMT", _pmtPids.at(2), "Program Map Table");

    for (StreamTypeHeader stream : _pmt.streams)
    {
        addTreeChild(pmtRoot, "ES", stream.elementary_PID, "Elementary Stream");
    }
}

QTreeWidgetItem* ParserDialog::addTreeRoot(QString name,
                                           int PID,
                                           QString description)
{
    // QTreeWidgetItem(QTreeWidget * parent, int type = Type)
    QTreeWidgetItem *treeItem = new QTreeWidgetItem(_treeWidget);
    treeItem->setText(0, name);
    treeItem->setText(1, QString::number(PID));
    treeItem->setText(2, description);
    return treeItem;
}

QTreeWidgetItem* ParserDialog::addTreeChild(QTreeWidgetItem *parent,
                                QString name,
                                int PID,
                                QString description)
{
    // QTreeWidgetItem(QTreeWidget * parent, int type = Type)
    QTreeWidgetItem *treeItem = new QTreeWidgetItem();

    // QTreeWidgetItem::setText(int column, const QString & text)
    treeItem->setText(0, name);
    treeItem->setText(1, QString::number(PID));
    treeItem->setText(2, description);

    // QTreeWidgetItem::addChild(QTreeWidgetItem * child)
    parent->addChild(treeItem);
    return treeItem;
}
