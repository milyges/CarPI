#include "changeremulator.h"
#include <QDebug>

ChangerEmulator * ChangerEmulator::_instance = NULL;
QThread ChangerEmulator::_workerThread;

ChangerEmulator::ChangerEmulator(QObject *parent) : QObject(parent) {
    _reset();

    _serial = new QSerialPort(this);
    _serial->setPortName("/dev/ttyAMA0");
    _serial->open(QIODevice::ReadWrite);
    _serial->setBaudRate(QSerialPort::Baud9600);
    _serial->setDataBits(QSerialPort::Data8);
    _serial->setParity(QSerialPort::EvenParity);
    _serial->setStopBits(QSerialPort::OneStop);
    _serial->setFlowControl(QSerialPort::NoFlowControl);
    connect(_serial, SIGNAL(readyRead()), this, SLOT(_packetRecv()));
    _serial->clear();

    _ioTimer = new QTimer(this);
    _ioTimer->setInterval(100);
    _ioTimer->setSingleShot(false);
    connect(_ioTimer, SIGNAL(timeout()), this, SLOT(_ioTimerTick()));

    _watchdogTimer = new QTimer(this);
    _watchdogTimer->setInterval(1000);
    _watchdogTimer->setSingleShot(false);
    connect(_watchdogTimer, SIGNAL(timeout()), this, SLOT(_watchdogTick()));

    _watchdogTimer->start();
    _ioTimer->start();
}

ChangerEmulator::~ChangerEmulator() {
    delete _watchdogTimer;
    delete _ioTimer;
    delete _serial;
}

void ChangerEmulator::_reset() {
    qDebug() << "ChangerEmulator: Reseting emulator";
    _frameID = 0x00;
    _changerState = cdcStateBooting1;
    _currentCD = 1;
    _currentTrack = 1;
    _cdState = cdStateNoCD;
    _trayState = trayStateCDReady;
    _randomPlaying = false;
    _cdBitmap = 0xFC;
    _cdTime = QTime(0, 0);
    _trackTime = QTime(0, 0);
}

bool ChangerEmulator::_packetSend(QByteArray &data) {
    QByteArray frame;
    uint8_t checksum = 0x00;
    int i;

    frame.append(0x3D); /* Nagłówek ramki */
    frame.append(_frameID); /* ID Ramki */
    frame.append(data.length()); /* Rozmiar danych */
    frame.append(data); /* Dane */

    /* Obliczamy sumę kontrolną */
    for(i = 0; i < frame.length(); i++) {
        checksum ^= frame.at(i);
    }
    frame.append(checksum);

    _frameID++;

    _sendQueue.append(frame);

    return true;
}

void ChangerEmulator::_sendCurrentState() {
    QByteArray packet;
    switch(_changerState) {
        case cdcStateBooting1: {
            packet.setRawData("\x11\x60\x06", 3);
            _packetSend(packet);
            break;
        }
        case cdcStateBooting2: {
            packet.setRawData("\x15\x00\x25", 3);
            _packetSend(packet);
            break;
        }
        case cdcStatePause:
        case cdcStateStandby: {
            _packetSendStatus();
            break;
        }
        case cdcStatePlaying: {
            _packetSendPlayingStatus();
            _packetSendStatus();
            break;
        }
    }
}

void ChangerEmulator::_packetSendStatus() {
    QByteArray packet;

    packet.append(0x20); /* Status */
    packet.append(_cdState);
    packet.append(_trayState);
    packet.append(0x09);
    packet.append(0x05);
    packet.append(_currentCD);

    _packetSend(packet);
}

void ChangerEmulator::_packetSendRandomStatus() {
    QByteArray packet;

    packet.append(0x25); /* Packet ID = RANDOM_STATUS */

    if (!_randomPlaying)
        packet.append(0x03);
    else
        packet.append(0x07);

    _packetSend(packet);
}

void ChangerEmulator::_packetSendTrayStatus() {
    QByteArray packet;

    packet.append(0x26); /* Packet ID = TRAY_STATUS */
    packet.append(0x05);
    packet.append(_currentCD);
    packet.append(_cdBitmap);
    packet.append(_cdBitmap);

    _packetSend(packet);
}

void ChangerEmulator::_packetSendCDSummary() {
    QByteArray packet;

    qDebug() << "CDCEmu: Sending CD Summary";

    packet.append(0x46); /* Packet ID = CD_SUMMARY */
    /* 99 ścieżkowa płyta o dugości 60:00:00 */
    packet.append((char)0x99);
    packet.append((char)0x01);
    packet.append((char)0x00);
    packet.append((char)0x60);
    packet.append((char)0x00);
    packet.append((char)0x00);

    _packetSend(packet);
}

void ChangerEmulator::_packetSendCDOperation() {
    QByteArray packet;

    packet.append(0x21); /* Packet ID = CD_OPERATION */
    packet.append(_cdState);

    _packetSend(packet);
}

void ChangerEmulator::_packetSendTrackChange(enum ChangerEmulatorTrackState type, int num) {
    QByteArray packet;
    int tmp = type;
    packet.append(0x27); /* Packet ID = TRACK_CHANGE */

    if ((type == trackChangeEntering) && (num == 1)) {
        tmp |= 0x05;
    }

    packet.append(tmp);
    packet.append(_bcd(num));
    packet.append(0x22);

    _packetSend(packet);
}

void ChangerEmulator::_packetSendPlayingStatus() {
    QByteArray packet;

    packet.append(0x47); /* Packet ID = PLAYING */

    packet.append(_bcd(_currentTrack));
    packet.append(0x01);
    packet.append(_bcd(_cdTime.hour()));
    packet.append(_bcd(_cdTime.minute()));
    packet.append(_bcd(_cdTime.second()));
    packet.append((char)0x00);
    packet.append(_bcd(_trackTime.hour()));
    packet.append(_bcd(_trackTime.minute()));
    packet.append(_bcd(_trackTime.second()));
    packet.append((char)0x00);

    _packetSend(packet);
}

void ChangerEmulator::_packetRecvCommand(QByteArray &data) {
    qDebug("ChangerEmulator: Command: 0x%X", data.at(0));

    switch (data.at(0)) {
        case 0x00: { /* RESET */
            _reset();
            break;
        }
        case 0x13: { /* PLAY */
            _cdState = cdStateLoadingTrack;
            _packetSendCDOperation();
            _sendCurrentState();

            _changerState = cdcStatePlaying;

            emit playbackStarted();
            break;
        }
        case 0x17: { /* NEXT_TRACK */
            emit nextTrack();
            break;
        }
        case 0x19: { /* STOP */
            _cdState = cdStateLoadingTrack;
            _packetSendCDOperation();
            _packetSendStatus();

            _cdState = cdStateNoCD;
            _packetSendCDOperation();
            _packetSendStatus();

            _trayState = trayStateCDReady;
            _packetSendTrayStatus();
            _packetSendStatus();

            _changerState = cdcStateStandby;

            emit playbackStopped();
            break;
        }
        case 0x1C: { /* PAUSE */
            _changerState = cdcStatePause;
            _packetSendCDOperation();
            _packetSendStatus();

            emit playbackPaused();
            break;
        }
        case 0x22: { /* PREV_TRACK */
            emit prevTrack();
            break;
        }
        case 0x26: { /* LOAD_CD */
            emit loadCD(data.at(1));
            break;
        }
        case 0x86: { /* REQUEST_CD_INFO */
            _packetSendCDSummary();
            break;
        }
    }
}

int ChangerEmulator::_bcd(int num) {
    return ((num / 10) << 4) | (num % 10);
}

void ChangerEmulator::_packetRecv() {
    QByteArray packet;

    packet = _serial->readAll();

    /*qDebug() << "CDCEmu: New frame:";
    for (int i = 0; i < packet.length(); i++) {
        qDebug("Byte %d: 0x%02X", i, packet.at(i));
    }*/

    if (packet.at(0) == 0xC5) { /* Potwierdzenie od HU */
        _watchdogTimeout = 3;
        //qDebug() << "ChangerEmulator: got ack from HU, len:" << packet.length();
        switch(_changerState) {
            case cdcStateBooting1: {
                _changerState = cdcStateBooting2;
                break;
            }
            case cdcStateBooting2: {
                _packetSendStatus();
                _packetSendRandomStatus();
                _packetSendTrayStatus();
                _packetSendRandomStatus();
                _changerState = cdcStateStandby;
                break;
            }
        }
    }
    else { /* Nowe polecenie */
        _recvQueue.push_back(packet);
        _serial->write("\xC5"); /* Wysyłamy potwierdzenie do HU */
    }
}

void ChangerEmulator::_ioTimerTick() {
    QByteArray packet;

    while(!_recvQueue.isEmpty()) {
        packet = _recvQueue.takeFirst();

        /* Usuwamy: typ, id pakietu, długość */
        packet.remove(0, 3);
        _packetRecvCommand(packet);
    }

    while(!_sendQueue.isEmpty()) {
        packet = _sendQueue.takeFirst();

        /*qDebug() << "CDCEmu: Sending frame:";
        for(int i = 0; i < packet.length(); i++) {
            qDebug("Byte %d: 0x%02X", i, packet.at(i));
        }*/

        _serial->write(packet);
        _serial->flush();

        /* Czekamy na potwierdzenie od HU */
        if (!_serial->waitForReadyRead(200)) {
            //qDebug() << "ChangerEmulator: ACK timeout";
            break;
        }
    }

    if ((_changerState == cdcStatePlaying) && ((_cdState == cdStateLoadingTrack) || (_cdState == cdStateSearchingTrack))) {
        _cdState = cdStatePlaying;
        _packetSendTrackChange(trackChangeEntering, _currentTrack);
        _packetSendCDOperation();
        _sendCurrentState();
    }
}

void ChangerEmulator::_watchdogTick() {
    if (_changerState != cdcStateBooting1) {
        _watchdogTimeout--;
        if (_watchdogTimeout <= 0) {
            emit playbackStopped();
            _reset();
        }
    }
    _sendCurrentState();
}

ChangerEmulator * ChangerEmulator::getInstance() {
    if (!_instance) {
        _instance = new ChangerEmulator();
        _instance->moveToThread(&_workerThread);
        _workerThread.start(QThread::HighPriority);
    }

    return _instance;
}
