#ifndef ABXTESTER_H
#define ABXTESTER_H

#include <qlist.h>
#include <qobject.h>
#include <qmediaplayer.h>
#include <qrandom.h>

class ABXTester : public QObject
{
    Q_OBJECT
public:
    explicit ABXTester(QString filePath1, QString filePath2, int trials, qint64 segmentStart,
                       qint64 segmentStop, QObject *parent = nullptr);
    ~ABXTester();
    int getCurrentTrial();
    int getTrials();
    qint64 getPlaybackPosition();

private:
    QString filePath1;
    QString filePath2;
    int currentTrial = 0;
    int trials;
    qint64 segmentStart;
    qint64 segmentStop;
    bool testIsStarted = false;
    bool testIsFinished = false;
    bool testIsRunning = false;
    QList<bool> results;
    QMediaPlayer *APlayer;
    QMediaPlayer *BPlayer;
    // XPlayer and currentPlayer are supposed to point at APlayer or BPlayer
    QMediaPlayer *XPlayer;
    QMediaPlayer *currentPlayer;
    QMediaPlayer *previousPlayer;
    qint64 currentPlaybackPosition;

    void stopCurrentPlayer();
    void startCurrentPlayer();

public slots:
    void startTest();
    void startNextTrial();
    void finishCurrentTrial();
    void chooseA();
    void chooseB();
    void listenToA();
    void listenToB();
    void listenToX();
    void changePlaybackPosition(qint64 position);

signals:
    void AChosen();
    void BChosen();
    void currentTrialFinished();
    void nextTrialStarted();
    void AIsListenedTo();
    void BIsListenedTo();
    void XIsListenedTo();
    void playbackPositionChanged(qint64 position);
    void testStarted();
    void testFinished(QList<bool> results);
};

#endif // ABXTESTER_H
