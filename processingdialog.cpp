#include "processingdialog.h"
#include "./ui_processingdialog.h"

ProcessingDialog::ProcessingDialog(QWidget *parent) : QDialog(parent), ui(new Ui::ProcessingDialog)
{
    ui->setupUi(this);
}

ProcessingDialog::~ProcessingDialog()
{
    delete ui;
}

void ProcessingDialog::reject() { }
