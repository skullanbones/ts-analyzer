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
    _ui(new Ui::Dialog),
    _addedPmts(false)
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

void ParserDialog::PATCallback(PsiTable* table, uint16_t pid, void* hdl)
{
    auto pat = dynamic_cast<PatTable*>(table);
    ParserDialog* diag = reinterpret_cast<ParserDialog*>(hdl);

    // Print out data
    std::stringstream buffer;
    buffer << *pat << std::endl;
    std::string inStr = buffer.str();
    QString str = QString::fromUtf8(inStr.c_str());
    //qDebug() << str;
    //diag->printData(buffer.str());

    // Book keep all PATs for quality check
    diag->_pmtPids.push_back(pat->programs[0].program_map_PID);
    float average = accumulate( diag->_pmtPids.begin(), diag->_pmtPids.end(), 0.0)/diag->_pmtPids.size();
    diag->printData("average:");
    diag->printData(std::to_string(average));
    // Book keep number of programs, TODO use map std:pair<int, int> for PID, numberPrograms

    if (!diag->_addedPmts)
    {
        int numPrograms = pat->programs.size();
        if (numPrograms == 0)
        {
            qDebug() << "No programs found in PAT, exiting...";
            exit(EXIT_SUCCESS);
        }
        else if (numPrograms == 1) // SPTS
        {
            qDebug() << "Found Single Program Transport Stream (SPTS).";
            diag->_pmtPids.push_back(pat->programs[0].program_map_PID);
        }
        else if (numPrograms >= 1) // MPTS
        {
            qDebug() << "Found Multiple Program Transport Stream (MPTS).";
            for (auto program : pat->programs)
            {
                if (program.type == ProgramType::PMT)
                {
                    diag->_pmtPids.push_back(program.program_map_PID);
                }
            }
        }
    }

    if (!diag->_addedPmts && (diag->_pmtPids.size() != 0u))
    {
        for (auto pid : diag->_pmtPids)
        {
            qDebug() << "Adding PSI PID for parsing: " << pid;
            diag->_tsDemuxer.addPsiPid(pid, std::bind(&PMTCallback, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3), hdl);
        }
        diag->_addedPmts = true;
    }
}

void ParserDialog::PMTCallback(PsiTable* table, uint16_t pid, void* hdl)
{
    auto pmt = dynamic_cast<PmtTable*>(table);
    ParserDialog* instance = reinterpret_cast<ParserDialog*>(hdl);

    // Print out data
    std::stringstream buffer;
    buffer << *pmt << std::endl;
    std::string inStr = buffer.str();
    QString str = QString::fromUtf8(inStr.c_str());
    //qDebug() << str;
    instance->printData(buffer.str());

    // TODO for now we always use the last PMT in the stream.
    // TODO need use something better...
    instance->_pmt = *pmt; //  copy instance
}

void ParserDialog::parseTransportStream()
{
    QByteArray data = _hexEdit->data();

    // If empty data, just return
    if (data.size() <= 0) {
        printData("No data to parse... data().size:");
        printData(std::to_string(data.size()));
        return;
    }

    uint64_t count = 0;
    int readIndex = 0;
    const uint8_t* packetsData = (const uint8_t*)data.data();
    // Register PAT callback
    _tsDemuxer.addPsiPid(TS_PACKET_PID_PAT, std::bind(&ParserDialog::PATCallback, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3), (void*) this);

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
    _treeWidget->clear();
    _treeWidget->setColumnCount(3);
    QStringList ColumnNames;
    ColumnNames << "Table" << "PID" << "Description";

    _treeWidget->setHeaderLabels(ColumnNames);

    // Add root nodes
    QTreeWidgetItem* root = addTreeRoot("PAT", 0, "Program Association Table");
    QTreeWidgetItem* pmtRoot = addTreeChild(root, "PMT", _pmtPids.at(2), "Program Map Table");

    for (StreamTypeHeader stream : _pmt.streams)
    {
        std::string str = StreamTypeToString[stream.stream_type];
        QString qstr = QString::fromStdString(str);
        addTreeChild(pmtRoot, "PES", stream.elementary_PID, qstr);
    }

    // Add PRC PID
    addTreeChild(pmtRoot, "PCR", _pmt.PCR_PID, "Program Clock Reference");
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
