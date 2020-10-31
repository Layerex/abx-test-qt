#include "bitrateselectdialog.h"
#include "./ui_bitrateselectdialog.h"

BitrateSelectDialog::BitrateSelectDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::BitrateSelectDialog)
{
    ui->setupUi(this);
    ui->formatComboBox->addItems(audioFormats);

    connect(ui->bitrateLineEdit, &QLineEdit::editingFinished, this, [this] {
        if (getBitrate() < 1)
            ui->bitrateLineEdit->setText("1");
        else if (getBitrate() > 320)
            ui->bitrateLineEdit->setText("320");
    });
}

BitrateSelectDialog::~BitrateSelectDialog()
{
    delete ui;
}

QString BitrateSelectDialog::getBitrate()
{
    return ui->bitrateLineEdit->text() + "k";
}

QString BitrateSelectDialog::getFormat()
{
    return ui->formatComboBox->currentText();
}
