#ifndef PROCESSINGDIALOG_H
#define PROCESSINGDIALOG_H

#include <qdialog.h>

namespace Ui {
class ProcessingDialog;
}

class ProcessingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ProcessingDialog(QWidget *parent = nullptr);
    ~ProcessingDialog();

private:
    Ui::ProcessingDialog *ui;
    void reject();
};

#endif // PROCESSINGDIALOG_H
