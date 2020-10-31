#ifndef BITRATESELECTDIALOG_H
#define BITRATESELECTDIALOG_H

#include <qdialog.h>
#include <qlineedit.h>

namespace Ui {
class BitrateSelectDialog;
}

class BitrateSelectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BitrateSelectDialog(QWidget *parent = nullptr);
    ~BitrateSelectDialog();
    QString getBitrate();
    QString getFormat();

private:
    Ui::BitrateSelectDialog *ui;
    const QStringList audioFormats = {  "mp3", "ogg", "opus", "wav", "webm" };
};

#endif // BITRATESELECTDIALOG_H
