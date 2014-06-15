#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QTimer>
#include "mainboard.h"

enum BluetoothCallState {
    callStateIdle = 0,
    callStateTalking,
    callStateIncoming,
    callStateOutgoing
};

class Bluetooth : public QObject {
    Q_OBJECT
private:
    static Bluetooth * _instance;

    enum BluetoothCallState _callState;
    int _connectionState;

    MainBoard * _mainboard;
    QSerialPort * _serial;
    QTimer * _reconnectTimer;

    explicit Bluetooth(QObject *parent = 0);
    ~Bluetooth();

    QString _sendCommand(QString command);

private slots:
    void _tryReconnectLast(void);
    void _bluetoothInterrupt(void);

public:
    static Bluetooth * getInstance(void);
    enum BluetoothCallState callState(void);
    bool isConnected(void);

signals:
    void connectionStateChanged(bool isConnected);
    void callStateChanged(enum BluetoothCallState callState);

public slots:
    void acceptCall(void);
    void rejectCall(void);
    void terminateCall(void);
    void dialTo(QString number);

};

#endif // BLUETOOTH_H
