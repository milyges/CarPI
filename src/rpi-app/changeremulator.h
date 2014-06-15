#ifndef CHANGEREMULATOR_H
#define CHANGEREMULATOR_H

#include <QObject>
#include <QThread>
#include <QList>
#include <QTimer>
#include <QTime>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <stdint.h>

enum ChangerEmulatorState {
    cdcStateBooting1 = 1,
    cdcStateBooting2,
    cdcStateStandby,
    cdcStatePlaying,
    cdcStatePause
};

enum ChangerEmulatorCDState {
    cdStateNoCD = 0x01,
    cdStatePaused = 0x03,
    cdStateLoadingTrack = 0x04,
    cdStatePlaying = 0x05,
    cdStateCueing = 0x07,
    cdStateRewinding = 0x09,
    cdStateSearchingTrack = 0x0A
};

enum ChangerEmulatorTrayState {
    trayStateNoTray = 0x02,
    trayStateCDReady = 0x03,
    trayStateLoadingCD = 0x04,
    trayStateUnloadingCD = 0x05
};

enum ChangerEmulatorTrackState {
    trackChangeEntering = 0x10,
    trackChangeEntered = 0x14,
    trackChangeLeavingCD = 0x40,
    trackChangeLeaving = 0x80
};

class ChangerEmulator : public QObject {
    Q_OBJECT
private:
    static ChangerEmulator * _instance;
    static QThread _workerThread;

    uint8_t _frameID;
    QSerialPort * _serial;

    QList<QByteArray> _sendQueue;
    QList<QByteArray> _recvQueue;

    QTimer * _ioTimer;
    QTimer * _watchdogTimer;
    int _watchdogTimeout;


    enum ChangerEmulatorState _changerState;
    enum ChangerEmulatorCDState _cdState;
    enum ChangerEmulatorTrayState _trayState;

    int _currentCD;
    int _currentTrack;
    bool _randomPlaying;
    int _cdBitmap;
    QTime _trackTime;
    QTime _cdTime;

    explicit ChangerEmulator(QObject *parent = 0);
    ~ChangerEmulator();

    void _reset(void);

    bool _packetSend(QByteArray &data);
    void _sendCurrentState(void);

    void _packetSendStatus(void);
    void _packetSendRandomStatus(void);
    void _packetSendTrayStatus(void);
    void _packetSendCDSummary(void);
    void _packetSendCDOperation(void);
    void _packetSendTrackChange(enum ChangerEmulatorTrackState type, int num);
    void _packetSendPlayingStatus(void);

    void _packetRecvCommand(QByteArray & data);

    static int _bcd(int num);

private slots:
    void _packetRecv(void);
    void _ioTimerTick(void);
    void _watchdogTick(void);

public:
    static ChangerEmulator * getInstance(void);

    
signals:
    void playbackStopped(void);
    void playbackStarted(void);
    void playbackPaused(void);

    void nextTrack(void);
    void prevTrack(void);
    void loadCD(int num);

public slots:

};

#endif // CHANGEREMULATOR_H
