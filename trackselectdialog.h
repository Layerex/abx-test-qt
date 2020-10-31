#ifndef TRACKSELECTDIALOG_H
#define TRACKSELECTDIALOG_H

#include <qchar.h>
#include <qdialog.h>
#include <qdir.h>
#include <qeventloop.h>
#include <qfiledialog.h>
#include <qfileinfo.h>
#include <qlineedit.h>
#include <qlist.h>
#include <qobject.h>
#include <qmediaplayer.h>
#include <qmessagebox.h>
#include <qprocess.h>
#include <qpushbutton.h>
#include <qstandardpaths.h>
#include <qtemporaryfile.h>
#include <qthread.h>
#include <qtimer.h>
#include "bitrateselectdialog.h"
#include "processingdialog.h"

namespace Ui {
class TrackSelectDialog;
}

class TrackSelectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TrackSelectDialog(QWidget *parent = nullptr);
    ~TrackSelectDialog();
    QString getTrack1();
    QString getTrack2();
    int getTrials();
    qint64 getSegmentStart();
    qint64 getSegmentStop();

private:
    Ui::TrackSelectDialog *ui;
    ProcessingDialog *processingDialog;
    const QString fileMatchingPatterns =
            "Audio Files (*.flac *.mp3 *.ogg *.opus *.wav *.webm);; Any (*)";
    qint64 track1Length;
    qint64 track2Length;
    QProcess *ffmpegProcess;
    QLineEdit *targetLineEdit;
    bool isFileReadable(const QString &filePath);
    qint64 getFileLength(const QString &filePath);
    enum timestampElementsEnumeration { HOURS, MINUTES, SECONDS };
    QList<int> getTimestampElements(const QString &timestamp);
    bool isTimestampValid(const QString &timestamp);
    QString millisecondsToTimestamp(const qint64 &milliseconds);
    qint64 timestampToMilliseconds(const QString &timestamp);

private slots:
    void convertAudio(const QString &filePath);

signals:
    void conversionStarted();
    void conversionFinished(QString outputFilePath);
};

#endif // TRACKSELECTDIALOG_H
