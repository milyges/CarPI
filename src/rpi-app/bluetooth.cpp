#include "bluetooth.h"
#include <QDebug>

Bluetooth * Bluetooth::_instance = NULL;

Bluetooth::Bluetooth(QObject *parent) : QObject(parent) {
    _callState = callStateIdle;
    _mainboard = MainBoard::getInstance();
    _connectionState = 0xFF;

    connect(_mainboard, SIGNAL(bluetoothInt()), this, SLOT(_bluetoothInterrupt()));

    _serial = new QSerialPort(this);
    _serial->setPortName("/dev/ttyUSB2");
    _serial->open(QIODevice::ReadWrite);
    _serial->setBaudRate(QSerialPort::Baud115200);
    _serial->setDataBits(QSerialPort::Data8);
    _serial->setParity(QSerialPort::NoParity);
    _serial->setStopBits(QSerialPort::OneStop);
    _serial->setFlowControl(QSerialPort::NoFlowControl);

    _mainboard->bluetoothSetMode(true);
    _serial->clear();

    _reconnectTimer = new QTimer(this);
    _reconnectTimer->setInterval(10000);
    _reconnectTimer->setSingleShot(false);
    connect(_reconnectTimer, SIGNAL(timeout()), this, SLOT(_tryReconnectLast()));

    _bluetoothInterrupt();
}

Bluetooth::~Bluetooth() {
    delete _serial;
}

QString Bluetooth::_sendCommand(QString command) {
    QString result;

    /* Ten moduł bt jest lekko spierdolony, trzeba troche magii żeby się z nim dogadać */
    do {
        result = "";
        _serial->clear();

        _serial->write(command.toAscii());

        do {
            if (!_serial->waitForReadyRead(1000)) {
                qDebug() << "Bluetooth: _send_command(): timeout";
                result = "";
                break;
            }

            result += QString(_serial->readLine());
        } while(result.at(result.length() - 1) != '\n');
    } while ((result.trimmed() == "CMD") || (result.trimmed() == "END"));

    //qDebug() << "Bleutooth: cmd =" << command.trimmed() << "reply =" << result.trimmed();

    return result.trimmed();
}

void Bluetooth::_tryReconnectLast() {
    _sendCommand("B\r\n");
}

void Bluetooth::_bluetoothInterrupt() {
    QString status;
    int state[2];
    enum BluetoothCallState callState;

    status = _sendCommand("Q\r\n");

    if (status.length() != 4) /* Coś nie tak z odpowiedzią */
        return;

    state[0] = status.left(2).toUInt(NULL, 16); /* Status połączenia z telefonem */
    state[1] = status.right(2).toUInt(NULL, 16); /* Status rozmowy */


    if (state[0] != _connectionState) {
        _connectionState = state[0];

        emit connectionStateChanged(state[0] > 0);

        if (state[0] > 0)
            _reconnectTimer->stop();
        else
            _reconnectTimer->start();
    }

    switch(state[1] & 0x0F) {
        case 0x04: callState = callStateOutgoing; break;
        case 0x05: callState = callStateIncoming; break;
        case 0x06:
        case 0x0C: callState = callStateTalking; break;
        default: callState = callStateIdle;
    }

    if (callState != _callState) {
        _callState = callState;
        emit callStateChanged(callState);
    }
}

Bluetooth *Bluetooth::getInstance() {
    if (!_instance) {
        _instance = new Bluetooth();
    }

    return _instance;
}

BluetoothCallState Bluetooth::callState() {
    return _callState;
}

bool Bluetooth::isConnected() {
    return (_connectionState > 0);
}


void Bluetooth::acceptCall() {
    if (_callState == callStateIncoming)
        _sendCommand("C\r\n");
}

void Bluetooth::rejectCall() {
    if (_callState == callStateIncoming)
        _sendCommand("E\r\n");
}

void Bluetooth::terminateCall() {
    if ((_callState == callStateOutgoing) || (_callState == callStateTalking))
        _sendCommand("E\r\n");
}

void Bluetooth::dialTo(QString number) {
    if (_callState == callStateIdle)
        _sendCommand(QString("A,%1").arg(number));
}
