#include "trackselectdialog.h"
#include "./ui_trackselectdialog.h"

TrackSelectDialog::TrackSelectDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::TrackSelectDialog)
{
    ui->setupUi(this);

    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    ui->trackLengthsAreNotEqualLabel->hide();

    connect(ui->chooseTrack1PushButton, &QPushButton::clicked, this, [this] {
        ui->track1LineEdit->setText(QFileDialog::getOpenFileName(
                this, "Choose the first track",
                QStandardPaths::standardLocations(QStandardPaths::MusicLocation)[0],
                fileMatchingPatterns));
    });
    connect(ui->chooseTrack2PushButton, &QPushButton::clicked, this, [this] {
        ui->track2LineEdit->setText(QFileDialog::getOpenFileName(
                this, "Choose the second track",
                QStandardPaths::standardLocations(QStandardPaths::MusicLocation)[0],
                fileMatchingPatterns));
    });

    connect(ui->track1LineEdit, &QLineEdit::textChanged, this, [this](const QString &text) {
        if (isFileReadable(text)) {
            ui->convertToTrack2PushButton->setEnabled(true);
            track1Length = getFileLength(text);
            if (ui->convertToTrack1PushButton->isEnabled()) {
                ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
                if (abs(track1Length - track2Length) > 100)
                    ui->trackLengthsAreNotEqualLabel->show();
                else
                    ui->trackLengthsAreNotEqualLabel->hide();
                ui->segmentStartTimestampLineEdit->setText("00:00:00");
                ui->segmentStopTimestampLineEdit->setText(millisecondsToTimestamp(track1Length));
            }
        } else {
            ui->convertToTrack2PushButton->setEnabled(false);
            ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
        }
    });
    connect(ui->track2LineEdit, &QLineEdit::textChanged, this, [this](const QString &text) {
        if (isFileReadable(text)) {
            ui->convertToTrack1PushButton->setEnabled(true);
            track2Length = getFileLength(text);
            if (ui->convertToTrack2PushButton->isEnabled()) {
                ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
                if (abs(track1Length - track2Length) > 100)
                    ui->trackLengthsAreNotEqualLabel->show();
                else
                    ui->trackLengthsAreNotEqualLabel->hide();
                ui->segmentStartTimestampLineEdit->setText("00:00:00");
                ui->segmentStopTimestampLineEdit->setText(millisecondsToTimestamp(track1Length));
            }
        } else {
            ui->convertToTrack1PushButton->setEnabled(false);
            ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
        }
    });

    connect(ui->segmentStartTimestampLineEdit, &QLineEdit::textChanged, this,
            [this](const QString &text) {
                if (timestampToMilliseconds(text) > track1Length || !isTimestampValid(text))
                    ui->segmentStartTimestampLineEdit->setText("00:00:00");
            });
    connect(ui->segmentStopTimestampLineEdit, &QLineEdit::textChanged, this,
            [this](const QString &text) {
                if (timestampToMilliseconds(text) > track1Length || !isTimestampValid(text))
                    ui->segmentStopTimestampLineEdit->setText(
                            millisecondsToTimestamp(track1Length));
            });

    connect(ui->convertToTrack1PushButton, &QPushButton::clicked, this, [this] {
        convertAudio(ui->track2LineEdit->text());
        targetLineEdit = ui->track1LineEdit;
    });
    connect(ui->convertToTrack2PushButton, &QPushButton::clicked, this, [this] {
        convertAudio(ui->track1LineEdit->text());
        targetLineEdit = ui->track2LineEdit;
    });

    connect(this, &TrackSelectDialog::conversionStarted, this, [this] {
        processingDialog = new ProcessingDialog();
        processingDialog->exec();
    });
    connect(this, &TrackSelectDialog::conversionFinished, this, [this](const QString &filePath) {
        processingDialog->accept();
        delete processingDialog;
        if (isFileReadable(filePath))
            targetLineEdit->setText(filePath);
    });
}

TrackSelectDialog::~TrackSelectDialog()
{
    delete ui;
}

QString TrackSelectDialog::getTrack1()
{
    return ui->track1LineEdit->text();
}

QString TrackSelectDialog::getTrack2()
{
    return ui->track2LineEdit->text();
}

int TrackSelectDialog::getTrials()
{
    return ui->trialsLineEdit->text().toInt();
}

qint64 TrackSelectDialog::getSegmentStart()
{
    return timestampToMilliseconds(ui->segmentStartTimestampLineEdit->text());
}

qint64 TrackSelectDialog::getSegmentStop()
{
    return timestampToMilliseconds(ui->segmentStopTimestampLineEdit->text());
}
bool TrackSelectDialog::isFileReadable(const QString &filePath)
{
    QFileInfo fileInfo(filePath);
    if (fileInfo.isReadable())
        return true;
    return false;
}

qint64 TrackSelectDialog::getFileLength(const QString &filePath)
{
    if (isFileReadable(filePath)) {
        QMediaPlayer dummyPlayer;
        dummyPlayer.setMedia(QUrl::fromLocalFile(filePath));
        // A workaround to get an actual duration of a file (just setting up a player does not work
        // right with all files).
        dummyPlayer.setMuted(true);
        dummyPlayer.play();
        qint64 duration;
        QTimer timer;
        timer.setSingleShot(true);
        QEventLoop loop;
        connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);
        connect(&dummyPlayer, &QMediaPlayer::durationChanged, this, [&](qint64 d) {
            duration = d;
            loop.quit();
        });
        timer.start(1000);
        loop.exec();
        dummyPlayer.stop();
        return duration;
    }
    return -1;
}

QList<int> TrackSelectDialog::getTimestampElements(const QString &timestamp)
{
    QStringList timestampElements = timestamp.split(":");
    QList<int> intTimestampElements;
    for (int i = 0; i < timestampElements.length(); ++i) {
        if (timestampElements[i].startsWith('0'))
            timestampElements[i] = timestampElements[i][1];
        intTimestampElements.append(timestampElements[i].toInt());
    }
    return intTimestampElements;
}

bool TrackSelectDialog::isTimestampValid(const QString &timestamp)
{
    QList<int> timestampElements = getTimestampElements(timestamp);
    if (timestampElements[MINUTES] >= 60 || timestampElements[SECONDS] >= 60)
        return false;
    return true;
}

QString TrackSelectDialog::millisecondsToTimestamp(const qint64 &milliseconds)
{
    int seconds = milliseconds / 1000 % 60;
    int minutes = milliseconds / 1000 / 60 % 60;
    int hours = milliseconds / 1000 / 60 / 60 % 99;
    return QString("%1:%2:%3")
            .arg(hours, 2, 10, QChar('0'))
            .arg(minutes, 2, 10, QChar('0'))
            .arg(seconds, 2, 10, QChar('0'));
}

qint64 TrackSelectDialog::timestampToMilliseconds(const QString &timestamp)
{
    QList<int> timestampElements = getTimestampElements(timestamp);
    return timestampElements[HOURS] * 60 * 60 * 1000 + timestampElements[MINUTES] * 60 * 1000
            + timestampElements[SECONDS] * 1000;
}

void TrackSelectDialog::convertAudio(const QString &filePath)
{
    BitrateSelectDialog bitrateSelectDialog(this);
    if (bitrateSelectDialog.exec() == QDialog::Rejected)
        return;

    QTemporaryFile outputFile;
    outputFile.setAutoRemove(false);
    outputFile.open();
    QString outputFilePath = outputFile.fileName() + "." + bitrateSelectDialog.getFormat();

    QString program = "ffmpeg";
    QStringList arguments;
    arguments << "-i" << QDir::toNativeSeparators(filePath) << "-preset"
              << "faster"
              << "-b:a" << bitrateSelectDialog.getBitrate()
              << QDir::toNativeSeparators(outputFilePath);
    QProcess *ffmpegProcess = new QProcess();
    connect(ffmpegProcess, &QProcess::started, this, &TrackSelectDialog::conversionStarted);
    connect(ffmpegProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this,
            [=] {
                emit conversionFinished(outputFilePath);
                delete ffmpegProcess;
            });
    ffmpegProcess->start(program, arguments);
}
