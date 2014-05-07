#ifndef CHANGEREMULATOR_H
#define CHANGEREMULATOR_H

#include <QObject>
#include <QFile>
#include <QByteArray>
#include <stdint.h>
#include <QTimer>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QList>
#include <QTime>

enum {
    cdcStateBooting1 = 1,
    cdcStateBooting2,
    cdcStateStandby,
    cdcStatePlaying,
    cdcStatePause
};

enum {
    cdStateNoCD = 0x01,
    cdStatePaused = 0x03,
    cdStateLoadingTrack = 0x04,
    cdStatePaying = 0x05,
    cdStateCueing = 0x07,
    cdStateRewinding = 0x09,
    cdStateSearchingTrack = 0x0A
};

enum {
    trayStateNoTray = 0x02,
    trayStateCDReady = 0x03,
    trayStateLoadingCD = 0x04,
    trayStateUnloadingCD = 0x05
};

enum {
    trackChangeEntering = 0x10,
    trackChangeEntered = 0x14,
    trackChangeLeavingCD = 0x40,
    trackChangeLeaving = 0x80
};

class ChangerEmulator : public QObject {
    Q_OBJECT
private:
    uint8_t _frame_id;
    QSerialPort * _serial;
    int _cdc_state;
    int _current_cd;
    int _cdc_reset_timeout;
    int _cd_state;
    int _tray_state;
    bool _random_status;
    uint8_t _cd_bitmap;

    int _track_num;
    QTime _track_time;
    QTime _cd_time;

    QList<QByteArray> _send_queue;
    QList<QByteArray> _recv_queue;
    QTimer * _cdc_io_timer;
    QTimer * _cdc_watchdog_timer;

    bool _send_packet(QByteArray &data);

    void _send_status_packet(void);
    void _send_current_state(void);
    void _send_random_status(void);
    void _send_tray_status(void);
    void _send_cd_summary(void);
    void _send_cd_operation(void);
    void _send_cd_check(bool present);
    void _send_playing_state(void);
    void _send_track_change_frame(int type, int tracknum);

    void _recv_command(QByteArray & packet);

    static int _bcd(int num);

private slots:
    void _recv_packet(void);
    void _cdc_io_tick(void);
    void _cdc_watchdog_tick(void);


public:
    explicit ChangerEmulator(QObject *parent = 0);
    ~ChangerEmulator();
    void start(void);
    void stop(void);
    void reset(void);

    void next_track(void);
    void prev_track(void);
    void load_cd(int num);

signals:
    void playback_stopped(void);
    void playback_started(void);
    void playback_paused(void);
    void playback_next_pressed(void);
    void playback_prev_pressed(void);
    void load_cd_pressed(int num);

public slots:
    
};

#endif // CHANGEREMULATOR_H
