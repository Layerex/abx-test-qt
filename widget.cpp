#include "widget.h"
#include "./ui_widget.h"

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget)
{
    ui->setupUi(this);

    TrackSelectDialog trackSelector(this);
    if (trackSelector.exec() == QDialog::Rejected)
        exit(0);

    tester = new ABXTester(trackSelector.getTrack1(), trackSelector.getTrack2(),
                           trackSelector.getTrials(), trackSelector.getSegmentStart(),
                           trackSelector.getSegmentStop(), this);

    updateTrialCount();
    show();

    connect(tester, &ABXTester::currentTrialFinished, tester, &ABXTester::startNextTrial);
    connect(ui->startPushButton, &QPushButton::clicked, tester, &ABXTester::startTest);
    connect(tester, &ABXTester::testStarted, this, [this] {
        ui->startPushButton->setEnabled(false);
        ui->APushButton->setEnabled(true);
        ui->BPushButton->setEnabled(true);
        ui->seekBackPushButton->setEnabled(true);
        ui->seekForwardPushButton->setEnabled(true);
        ui->rewindPushButton->setEnabled(true);
        ui->chooseAPushButton->setEnabled(true);
        ui->chooseBPushButton->setEnabled(true);
    });
    connect(tester, &ABXTester::testFinished, this, [this](const QList<bool> &results) {
        QMessageBox resultsMessageBox(this);
        QString resultsMessageBoxText;
        QListIterator<bool> resultsIterator(results);
        for (int i = 1; resultsIterator.hasNext(); ++i) {
            resultsMessageBoxText.append(QString("Trial %1 of %2: ").arg(i).arg(results.length()));
            if (resultsIterator.next())
                resultsMessageBoxText.append("Pass");
            else
                resultsMessageBoxText.append("Fail");
            resultsMessageBoxText.append("\n");
        }
        resultsMessageBox.setStandardButtons(QMessageBox::Close);
        resultsMessageBox.setText(resultsMessageBoxText);
        resultsMessageBox.exec();
        close();
    });

    connect(tester, &ABXTester::AIsListenedTo, this, [this] {
        ui->APushButton->setEnabled(false);
        ui->BPushButton->setEnabled(true);
        ui->XPushButton->setEnabled(true);
    });
    connect(tester, &ABXTester::BIsListenedTo, this, [this] {
        ui->APushButton->setEnabled(true);
        ui->BPushButton->setEnabled(false);
        ui->XPushButton->setEnabled(true);
    });
    connect(tester, &ABXTester::XIsListenedTo, this, [this] {
        ui->APushButton->setEnabled(true);
        ui->BPushButton->setEnabled(true);
        ui->XPushButton->setEnabled(false);
    });
    connect(tester, &ABXTester::nextTrialStarted, this, [this] {
        ui->APushButton->setEnabled(true);
        ui->BPushButton->setEnabled(true);
        ui->XPushButton->setEnabled(false);
    });
    connect(ui->APushButton, &QPushButton::clicked, tester, &ABXTester::listenToA);
    connect(ui->BPushButton, &QPushButton::clicked, tester, &ABXTester::listenToB);
    connect(ui->XPushButton, &QPushButton::clicked, tester, &ABXTester::listenToX);

    connect(ui->chooseAPushButton, &QPushButton::clicked, tester, &ABXTester::chooseA);
    connect(ui->chooseBPushButton, &QPushButton::clicked, tester, &ABXTester::chooseB);

    connect(ui->rewindPushButton, &QPushButton::clicked, this,
            [this] { tester->changePlaybackPosition(0); });
    connect(ui->seekBackPushButton, &QPushButton::clicked, this,
            [this] { tester->changePlaybackPosition(tester->getPlaybackPosition() - 5000); });
    connect(ui->seekForwardPushButton, &QPushButton::clicked, this,
            [this] { tester->changePlaybackPosition(tester->getPlaybackPosition() + 5000); });

    connect(tester, &ABXTester::nextTrialStarted, this, &Widget::updateTrialCount);
}

Widget::~Widget()
{
    delete ui;
    delete tester;
}

void Widget::updateTrialCount()
{
    ui->trialCountLabel->setText(
            QString("Trial %1 of %2").arg(tester->getCurrentTrial()).arg(tester->getTrials()));
}
