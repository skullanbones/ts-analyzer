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
    diag->_pat = *pat;

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
    ParserDialog* diag = reinterpret_cast<ParserDialog*>(hdl);

    // Print out data
    std::stringstream buffer;
    buffer << *pmt << std::endl;
    std::string inStr = buffer.str();
    QString str = QString::fromUtf8(inStr.c_str());
    //qDebug() << str;
    diag->printData(buffer.str());

    // TODO for now we always use the last PMT in the stream.
    // TODO need use something better...
    diag->_pmt = *pmt; //  copy instance
    diag->_pmtPid = pid;

    diag->printData("PMTCAllback pid: " + std::to_string(pid));
}

void ParserDialog::parseTransportStream()
{
    QByteArray data = _hexEdit->data();
    _addedPmts = false;
    _pmtPids.clear();

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
    _treeWidget->setColumnCount(2);
    _treeWidget->setColumnWidth(0, 300);
    QStringList ColumnNames;
    ColumnNames << "Description" << "Data";

    _treeWidget->setHeaderLabels(ColumnNames);

    // Add root nodes
    QTreeWidgetItem* root = addTreeRoot(_fileName, "");

    // Add child nodes
    QTreeWidgetItem* psiRoot = addTreeChild(root, "PSI (Program Specific Information)", "");
    QTreeWidgetItem* patRoot = addTreeChild(psiRoot, "PAT (Program Association Table) PID:", QString::number(TS_PACKET_PID_PAT));
    addTreeChild(psiRoot, "PMT", "Program Map Table" + _pmtPid);
    buildPatView(patRoot);

    // PES
    for (StreamTypeHeader stream : _pmt.streams)
    {
        QString qstr = QString::fromStdString(StreamTypeToString[stream.stream_type]) + " (" + QString::number(stream.elementary_PID) + ")";
        addTreeChild(root, "PES", qstr);
    }

    // Add PRC PID
    addTreeChild(root, "PCR (Program Clock Reference)", QString::number(_pmt.PCR_PID));
}

void ParserDialog::buildPatView(QTreeWidgetItem* patRoot)
{
    addTreeChild(patRoot, "table_id", QString::number(_pat.table_id));
    addTreeChild(patRoot, "section_syntax_indicator", QString::number(_pat.section_syntax_indicator));
    addTreeChild(patRoot, "section_length", QString::number(_pat.section_length));
    addTreeChild(patRoot, "transport_stream_id", QString::number(_pat.transport_stream_id));
    addTreeChild(patRoot, "version_number", QString::number(_pat.version_number));
    addTreeChild(patRoot, "current_next_indicator", QString::number(_pat.current_next_indicator));
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
