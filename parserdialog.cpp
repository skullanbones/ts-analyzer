#include <QMessageBox>

// proj files
#include "parserdialog.h"
#include "ui_parserdialog.h"

ParserDialog::ParserDialog(QHexEdit *hexEdit, QWidget *parent) :
    QDialog(parent),
    _ui(new Ui::Dialog)
{
  _ui->setupUi(this);
  _hexEdit = hexEdit;
}

ParserDialog::~ParserDialog()
{
  delete _ui;
}
