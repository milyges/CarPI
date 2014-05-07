#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QTimer>

#include <stdint.h>

enum {
    callStatusIdle = 0,
    callStatusTalking,
    callStatusIncomimg,
    callStatusOutgoing
};

class Bluetooth : public QObject {
    Q_OBJECT
private:
    QSerialPort * _serial;
    QTimer * _reconnect_timer;
    uint8_t _connection_status;
    int _call_status;

    QString _send_command(QString cmd);
private slots:
    void _reconnect_last_device(void);

public:
    explicit Bluetooth(QObject *parent = 0);
    ~Bluetooth();

    int call_status(void);

signals:
    void state_changed(bool connected);
    void call_status_changed(int newstatus);

public slots:
    void new_event(void);

    void accept_call(void);
    void reject_call(void);
    void terminate_call(void);
    void dial(QString number);
};

#endif // BLUETOOTH_H
