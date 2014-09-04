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
    _serial->write("\r\n\r\n"); /* Asekuracyjnie, zeby na pewno nic nie bylo wpisane */
    _reconnectTimer = new QTimer(this);
    _reconnectTimer->setInterval(10000);
    _reconnectTimer->setSingleShot(false);
    connect(_reconnectTimer, SIGNAL(timeout()), this, SLOT(_tryReconnectLast()));

    _bluetoothInterrupt();
}

Bluetooth::~Bluetooth() {
    delete _serial;
}

QString Bluetooth::_sendCommand(QString command, int expectLines) {
    QString result;
    int lines = 0;

    qDebug() << "BT: _sendCommand(" << command << "," << expectLines << ");";
    /* Ten moduł bt jest lekko spierdolony, trzeba troche magii żeby się z nim dogadać */
    do {
        result = "";
        _serial->clear();

        _serial->write(command.toAscii());

        lines = 0;

        do {
            if (!_serial->waitForReadyRead(1000)) {
                qDebug() << "Bluetooth: _send_command(): timeout, result =" << result;
                result = "";
                break;
            }

            result += QString(_serial->readLine());
            if (result.at(result.length() - 1) == '\n')
                lines++;

        } while(lines < expectLines);
    } while ((result.trimmed() == "CMD") || (result.trimmed() == "END"));

    qDebug() << "BT: cmd =" << command.trimmed() << "reply =" << result.trimmed();

    return result.trimmed();
}

QString Bluetooth::_callerID() {
    QStringList data = _sendCommand("T\r\n", 3).split("\r\n");
    QString number;

    for(int i = 0; i < data.count(); i++) {
        if (data.at(i).startsWith("Number=")) {
            number = data.at(i).mid(data.at(i).indexOf("=") + 1);
        }
    }

    return number;
}

void Bluetooth::_tryReconnectLast() {
    _sendCommand("B\r\n");
}

void Bluetooth::_bluetoothInterrupt() {
    QString status;
    int state[2];
    QString number;
    enum BluetoothCallState callState;

    status = _sendCommand("Q\r\n");

    if (status.length() != 4) /* Coś nie tak z odpowiedzią */
        return;

    state[0] = status.left(2).toUInt(NULL, 16); /* Status połączenia z telefonem */
    state[1] = status.right(2).toUInt(NULL, 16); /* Status rozmowy */

    if (state[0] & (1 << 4)) {
        number = _callerID();
    }

    switch(state[1] & 0x0F) {
        case 0x04: callState = callStateOutgoing; break;
        case 0x05: callState = callStateIncoming; break;
        case 0x06:
        case 0x0C: callState = callStateTalking; break;
        default: callState = callStateIdle;
    }

    if ((callState != _callState) || ((state[0] != _connectionState) && (state[0] & (1 << 4)))) {
        _callState = callState;
        //qDebug() << "bt: number =" << number;
        emit callStateChanged(callState, number);
    }

    if (state[0] != _connectionState) {
        _connectionState = state[0];

        emit connectionStateChanged((state[0] & 0x0F) > 0);

        if ((state[0] & 0x0F) > 0)
            _reconnectTimer->stop();
        else
            _reconnectTimer->start();
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

int Bluetooth::batteryLevel() {
    QString data = _sendCommand("GB\r\n");
    int start, end, val;
    bool dataOK;
    if (data.isEmpty()) {
        return -1;
    }

    if (!data.startsWith("AGBatteryLevel")) {
        return -1;
    }

    start = data.indexOf(QChar('='));
    end = data.lastIndexOf(QChar('%'));

    if ((start < 0) || (end < 0)) {
        return -1;
    }

    val = data.mid(start + 1, end - start - 1).trimmed().toInt(&dataOK);
    if (!dataOK) {
        return -1;
    }

    return val;
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
        _sendCommand(QString("A,%1\r\n").arg(number));
}

void Bluetooth::redialLast() {
    if (_callState == callStateIdle)
        _sendCommand("AR\r\n");
}

void Bluetooth::voiceCommand() {
    if (_callState == callStateIdle)
        _sendCommand("P\r\n");
}
