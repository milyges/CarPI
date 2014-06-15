#include "elm327.h"
#include <QDebug>
#include <QStringList>
#include <QVector>

Elm327 * Elm327::_instance = NULL;
QThread Elm327::_workerThread;

Elm327::Elm327(QString portName, QObject *parent) : QObject(parent) {

    /* Port szeregowy */
    _serial = new QSerialPort(this);
    _serial->setPortName(portName);

    _watchTimer = new QTimer(this);
    _watchTimer->setInterval(200);
    _watchTimer->setSingleShot(true);
    connect(_watchTimer, SIGNAL(timeout()), this, SLOT(_refreshData()));

    _started = false;

    if (!_serial->open(QIODevice::ReadWrite)) {
        qDebug() << "ELM327: Unable to open serial port";
        return;
    }
    _serial->setBaudRate(QSerialPort::Baud38400);
    _serial->setDataBits(QSerialPort::Data8);
    _serial->setParity(QSerialPort::NoParity);
    _serial->setStopBits(QSerialPort::OneStop);
    _serial->setFlowControl(QSerialPort::NoFlowControl);
    _serial->clear();

}

Elm327::~Elm327() {
    delete _watchTimer;
}


QStringList Elm327::_sendCommand(QString cmd) {
    QString result;
    QString tmp;
    QStringList lines;
    bool done = false;

    if (!_started) {
        return QStringList();
    }

    //qDebug() << "Elm327: Send:" << cmd.trimmed();
    _serial->write(cmd.toAscii());

    do {
        if (!_serial->waitForReadyRead(1000)) {
            //qDebug() << "Elm327: timeout, result =" << result.trimmed();
            break;
        }

        tmp = QString(_serial->readAll());

        //qDebug() << "Elm327: tmp =" << tmp.trimmed();

        if (tmp.right(1) == ">") {
            done = true;
            tmp[tmp.size() - 1] = '\r';
        }

        result += tmp;

        //qDebug() << "Elm327: result =" << result.trimmed();

        if (result.right(1) == "\r") {
            if (result.trimmed() != cmd.trimmed() && (!result.trimmed().isEmpty())) {
                lines.append(result.trimmed());
            }
            result.clear();
        }
    } while(!done);

    //qDebug() << "ELM327: Got" << lines;
    return lines;
}

bool Elm327::_setProtocol(int num) {
    QStringList res;

    res = _sendCommand(QString("AT SP %1\r").arg(num, 0, 16).toUpper());

    return ((res.size() > 0) && (res.at(0) == "OK"));
}

QVector<int> Elm327::_readPID(int mode, int pid) {
    QStringList res;
    QStringList data;
    QVector<int> result;
    QString reply = QString("%1 %2").arg(0x40 | mode, 2, 16, QLatin1Char('0')).arg(pid, 2, 16, QLatin1Char('0')).toUpper();
    res = _sendCommand(QString("%1%2\r").arg(mode, 2, 16, QLatin1Char('0')).arg(pid, 2, 16, QLatin1Char('0')).toUpper());

    //res.append("41 04 80");
    //res.append("41 05 82");
    //res.append("41 23 13 CE");
    //res.append("41 0B 80");
    //res.append("41 0F 28");

    for(int i = 0; i < res.size(); i++) {
        /* Szukamy odpowiedzi */
        if (res.at(i).startsWith(reply)) { /* Odpowiedź */
            data = res.at(i).split(' ');
            for(int j = 2; j < data.size(); j++) {
                result.append(data.at(j).toInt(NULL, 16));
            }
        }
    }

    return result;
}

double Elm327::_readVoltage() {
    QStringList res;
    QString tmp;
    double voltage;

    res = _sendCommand("AT RV\r");
    if (res.size() < 1)
        return 0;

    tmp = res.at(0);
    if (tmp.isEmpty())
        return 0;

    voltage = tmp.left(tmp.size() - 1).toDouble();

    return voltage;
}

void Elm327::_refreshData() {
    QVector<int> data;
    static int voltageTimeout = 0;

    if (!_started) {
        return;
    }

    for(int i = 0; i < _watchList.size(); i++) {
        data = _readPID(1, _watchList.at(i));
        if (!data.isEmpty())
            emit pidValueChanged(_watchList.at(i), data);
    }

    if (voltageTimeout-- <= 0) {
        emit voltageChanged(_readVoltage());
        voltageTimeout = 5;
    }

    _watchTimer->start();
}

Elm327 *Elm327::getInstance() {
    if (!_instance) {
        _instance = new Elm327("/dev/ttyUSB0");
        _instance->moveToThread(&_workerThread);
        _workerThread.start(QThread::NormalPriority);
    }

    return _instance;
}

void Elm327::addWatchPid(int pid) {
    _watchList.append(pid);
}

void Elm327::start() {
    if (!_serial->isOpen()) {
        qDebug() << "ELM327: Serial not opened";
        return;
    }
    _sendCommand("AT @1");
    _setProtocol(0);
    _started = true;
    _watchTimer->start();
}

void Elm327::stop() {
    _started = false;
    _watchTimer->stop();
    _sendCommand("AT LP");
}
