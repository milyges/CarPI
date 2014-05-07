#include "bluetooth.h"
#include "mainboard.h"
#include <QDebug>

Bluetooth::Bluetooth(QObject *parent) : QObject(parent) {
    /* Port szeregowy */
    _serial = new QSerialPort(this);
    _serial->setPortName("/dev/ttyUSB0");
    _serial->open(QIODevice::ReadWrite);
    _serial->setBaudRate(QSerialPort::Baud115200);
    _serial->setDataBits(QSerialPort::Data8);
    _serial->setParity(QSerialPort::NoParity);
    _serial->setStopBits(QSerialPort::OneStop);
    _serial->setFlowControl(QSerialPort::NoFlowControl);    
    _connection_status = 0xFF;
    _call_status = callStatusIdle;

    MainBoard::get_instance()->bluetooth_set_mode(true);
    _serial->clear();

    _reconnect_timer = new QTimer(this);
    _reconnect_timer->setSingleShot(false);
    _reconnect_timer->setInterval(5000);
    connect(_reconnect_timer, SIGNAL(timeout()), this, SLOT(_reconnect_last_device()));

    emit state_changed(false);
}

Bluetooth::~Bluetooth() {
    delete _reconnect_timer;
    delete _serial;
}

int Bluetooth::call_status() {
    return _call_status;
}

void Bluetooth::new_event() {
    QString status;
    uint8_t cstatus, state;
    int call_status;

    qDebug() << "Bluetooth: new event!";
    status = _send_command("Q\r\n");

    if (status.length() != 4) /* Błąd, zły format danych od modułu */
        return;

    cstatus = status.left(2).toUInt(NULL, 16);
    state = status.right(2).toUInt(NULL, 16);

    if (_connection_status != cstatus) {
        /* TODO: Sprawdź czy podłączony jest odpowieni profil */
        emit state_changed(cstatus > 0);
        _connection_status = cstatus;
        if (!_connection_status)
            _reconnect_timer->start();
        else
            _reconnect_timer->stop();
    }

    switch (state & 0x0F) {
        case 0x04: call_status = callStatusOutgoing; break;
        case 0x05: call_status = callStatusIncomimg; break;
        case 0x06:
        case 0x0C: call_status = callStatusTalking; break;
        default: call_status = callStatusIdle;
    }

    if (call_status != _call_status) {
        _call_status = call_status;

        emit call_status_changed(call_status);
    }
}

void Bluetooth::accept_call() {
    if (_call_status == callStatusIncomimg)
        _send_command("C\r\n");
}

void Bluetooth::reject_call() {
    if (_call_status == callStatusIncomimg)
        _send_command("E\r\n");
}

void Bluetooth::terminate_call() {
    if ((_call_status == callStatusOutgoing) || (_call_status == callStatusTalking))
        _send_command("E\r\n");
}

void Bluetooth::dial(QString number) {
    if (_call_status == callStatusIdle)
        _send_command(QString("A,%1").arg(number));
}

QString Bluetooth::_send_command(QString cmd) {
    QString result;

    /* Ten moduł bt jest lekko spierdolony, trzeba troche magii żeby się z nim dogadać */
    do {
        result = "";
        _serial->clear();

        _serial->write(cmd.toAscii());

        do {
            if (!_serial->waitForReadyRead(1000)) {
                qDebug() << "Bluetooth: _send_command(): timeout";
                result = "";
                break;
            }

            result += QString(_serial->readLine());
        } while(result.at(result.length() - 1) != '\n');
    } while ((result.trimmed() == "CMD") || (result.trimmed() == "END"));

    qDebug() << "Bleutooth: cmd =" << cmd.trimmed() << "reply =" << result.trimmed();

    return result.trimmed();
}

void Bluetooth::_reconnect_last_device() {
    _send_command("B\r\n");
}
