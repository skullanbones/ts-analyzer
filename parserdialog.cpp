#include <QMessageBox>

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
    parsePat();
}

/*****************************************************************************/
/* Private Methods */
/*****************************************************************************/
void ParserDialog::init()
{
    _textBrowser = _ui->textBrowser;
}

void ParserDialog::parseData()
{
    printData("Hello World!");
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
    _ui->textBrowser->setText(str);
}

void ParserDialog::PATCallback(PsiTable* table)
{
    auto pat = dynamic_cast<PatTable*>(table);

    // Print out data
    std::stringstream buffer;
    buffer << *pat << std::endl;
    std::string inStr = buffer.str();
    QString str = QString::fromUtf8(inStr.c_str());
    qDebug() << str;
}

void ParserDialog::parsePat()
{
    QByteArray data = _hexEdit->data();
    uint64_t count = 0;
    int readIndex = 0;
    const uint8_t* packetsData = (const uint8_t*)data.data();
    // Register callback
    _tsDemux.addPsiPid(0, std::bind(&ParserDialog::PATCallback, std::placeholders::_1));

    if ((data.at(0) != 0x47) || (data.size() <= 0))
    {
        qDebug() << "ERROR: 1'st byte not in sync!!!";
        return;
    }

    while (readIndex < data.size())
    {
        _tsDemux.demux(packetsData + readIndex);
        readIndex += TS_PACKET_SIZE;
        count++;
    }
    qDebug() << "Found " << count << " ts packets.";
}
