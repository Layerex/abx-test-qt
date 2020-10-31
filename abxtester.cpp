#include "abxtester.h"

ABXTester::ABXTester(QString filePath1, QString filePath2, int trials, qint64 segmentStart,
                     qint64 segmentStop, QObject *parent)
    : QObject(parent)
{
    this->filePath1 = filePath1;
    this->filePath2 = filePath2;
    this->trials = trials;
    this->segmentStart = segmentStart;
    this->segmentStop = segmentStop;
    APlayer = new QMediaPlayer(this);
    BPlayer = new QMediaPlayer(this);
    connect(APlayer, &QMediaPlayer::positionChanged, this, [&](qint64 position) {
        if (position >= this->segmentStop)
            APlayer->setPosition(this->segmentStart);
    });
    connect(BPlayer, &QMediaPlayer::positionChanged, this, [&](qint64 position) {
        if (position >= this->segmentStop)
            BPlayer->setPosition(this->segmentStart);
    });
}

ABXTester::~ABXTester()
{
    delete APlayer;
    delete BPlayer;
}

int ABXTester::getCurrentTrial()
{
    return currentTrial;
}

int ABXTester::getTrials()
{
    return trials;
}

qint64 ABXTester::getPlaybackPosition()
{
    if (testIsRunning)
        return currentPlayer->position();
    return 0;
}

void ABXTester::startTest()
{
    if (!testIsStarted) {
        startNextTrial();
    }
}

void ABXTester::startNextTrial()
{
    if (currentTrial >= trials) {
        emit testFinished(results);
        return;
    }
    if (!testIsStarted) {
        testIsStarted = true;
        emit testStarted();
    }
    ++currentTrial;
    emit nextTrialStarted();
    testIsRunning = true;
    if (QRandomGenerator::global()->bounded(2)) {
        BPlayer->setMedia(QUrl::fromLocalFile(filePath1));
        APlayer->setMedia(QUrl::fromLocalFile(filePath2));
    } else {
        APlayer->setMedia(QUrl::fromLocalFile(filePath1));
        BPlayer->setMedia(QUrl::fromLocalFile(filePath2));
    }

    if (QRandomGenerator::global()->bounded(2))
        XPlayer = BPlayer;
    else
        XPlayer = APlayer;
    currentPlayer = XPlayer;
    currentPlayer->setPosition(segmentStart);
    startCurrentPlayer();
}

void ABXTester::finishCurrentTrial()
{
    if (testIsRunning) {
        APlayer->stop();
        BPlayer->stop();
        currentPlaybackPosition = segmentStart;
        testIsRunning = false;
        emit currentTrialFinished();
    }
}

void ABXTester::stopCurrentPlayer()
{
    if (testIsRunning) {
        currentPlaybackPosition = currentPlayer->position();
        if (currentPlayer->state() == QMediaPlayer::PlayingState) {
            currentPlayer->stop();
        }
    }
}

void ABXTester::startCurrentPlayer()
{
    if (testIsRunning) {
        if (currentPlayer->state() == QMediaPlayer::StoppedState) {
            currentPlayer->setPosition(currentPlaybackPosition);
            currentPlayer->play();
        }
    }
}

void ABXTester::chooseA()
{
    if (testIsRunning) {
        emit AChosen();
        if (APlayer == XPlayer)
            results.append(true);
        else
            results.append(false);
        finishCurrentTrial();
    }
}

void ABXTester::chooseB()
{
    if (testIsRunning) {
        emit BChosen();
        if (BPlayer == XPlayer)
            results.append(true);
        else
            results.append(false);
        finishCurrentTrial();
    }
}

void ABXTester::listenToA()
{
    if (testIsRunning) {
        stopCurrentPlayer();
        currentPlayer = APlayer;
        startCurrentPlayer();
        emit AIsListenedTo();
    }
}

void ABXTester::listenToB()
{
    if (testIsRunning) {
        stopCurrentPlayer();
        currentPlayer = BPlayer;
        startCurrentPlayer();
        emit BIsListenedTo();
    }
}

void ABXTester::listenToX()
{
    if (testIsRunning) {
        stopCurrentPlayer();
        currentPlayer = XPlayer;
        startCurrentPlayer();
        emit XIsListenedTo();
    }
}

void ABXTester::changePlaybackPosition(qint64 position)
{
    if (testIsRunning) {
        if (position < segmentStart || position >= segmentStop)
            position = segmentStart;
        currentPlayer->setPosition(position);
        emit playbackPositionChanged(position);
    }
}
