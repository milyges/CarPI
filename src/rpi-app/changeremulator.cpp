#include "changeremulator.h"
#include <QDebug>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <QThread>

ChangerEmulator::ChangerEmulator(QObject *parent) : QObject(parent) {
    _frame_id = 0x00;    
    _cdc_state = cdcStateBooting1;
    _random_status = false;
    _current_cd = 1;

    _serial = new QSerialPort(this);
    _serial->setPortName("/dev/ttyAMA0");
    _serial->open(QIODevice::ReadWrite);
    _serial->setBaudRate(QSerialPort::Baud9600);
    _serial->setDataBits(QSerialPort::Data8);
    _serial->setParity(QSerialPort::EvenParity);
    _serial->setStopBits(QSerialPort::OneStop);
    _serial->setFlowControl(QSerialPort::NoFlowControl);
    connect(_serial, SIGNAL(readyRead()), this, SLOT(_recv_packet()));
    _serial->clear();

    _cdc_io_timer = new QTimer();
    _cdc_io_timer->setSingleShot(false);
    _cdc_io_timer->setInterval(100);
    connect(_cdc_io_timer, SIGNAL(timeout()), this, SLOT(_cdc_io_tick()));

    _cdc_watchdog_timer = new QTimer();
    _cdc_watchdog_timer->setSingleShot(false);
    _cdc_watchdog_timer->setInterval(1000);
    connect(_cdc_watchdog_timer, SIGNAL(timeout()), this, SLOT(_cdc_watchdog_tick()));

}

ChangerEmulator::~ChangerEmulator() {
    _serial->close();
    delete _serial;
}

void ChangerEmulator::_recv_packet(void) {
    QByteArray packet;
    packet = _serial->readAll();

//    qDebug() << "CDCEmu: New frame:";
//    for (int i = 0; i < packet.length(); i++) {
//        qDebug("Byte %d: 0x%02X", i, packet.at(i));
//    }

    if (packet.at(0) == 0xC5) { /* Potwierdzenie od HU */
        _cdc_reset_timeout = 5;
        //qDebug() << "got ack from HU, len:" << packet.length();
        switch (_cdc_state) {
            case cdcStateBooting1: {
                _cdc_state = cdcStateBooting2;
                break;
            }
            case cdcStateBooting2: {
                _send_status_packet();
                _send_random_status();
                _send_tray_status();
                _send_random_status();
                _cdc_state = cdcStateStandby;
                break;
            }
        }
    }
    else {
        _recv_queue.push_back(packet);

        _serial->write("\xC5"); /* Wysyłamy potwierdzenie */
    }
}

void ChangerEmulator::_cdc_io_tick() {
    QByteArray packet;

    while (!_recv_queue.isEmpty()) {
        packet = _recv_queue.takeFirst();

        /* Usuwamy: typ, id pakietu, długość */
        packet.remove(0, 3);
        _recv_command(packet);
    }

    while (!_send_queue.isEmpty()) {
        packet = _send_queue.takeFirst();

//        qDebug() << "CDCEmu: Sending frame:";
//        for(int i = 0; i < packet.length(); i++) {
//            qDebug("Byte %d: 0x%02X", i, packet.at(i));
//        }

        _serial->write(packet);
        _serial->flush();

        /* Czekamy na ACK */
        if (!_serial->waitForReadyRead(1000)) {
            qDebug() << "ACK Timeout";
            break;
        }
    }

    if ((_cdc_state == cdcStatePlaying) && ((_cd_state == cdStateLoadingTrack) || _cd_state == cdStateSearchingTrack)) {
        _cd_state = cdStatePaying;
        _send_track_change_frame(trackChangeEntering, _track_num);
        _send_cd_operation();
        _send_current_state();
    }
}

void ChangerEmulator::_cdc_watchdog_tick() {
    if (_cdc_reset_timeout <= 0) {
        qDebug() << "CDCEmu: Timeout, Reseting emulator";
        reset();
        return;
    }

    _cdc_reset_timeout--;

    _cd_time = _cd_time.addSecs(1);
    _track_time = _track_time.addSecs(1);

    _send_current_state();
}

bool ChangerEmulator::_send_packet(QByteArray &data) {
    QByteArray frame;
    uint8_t checksum = 0x00;
    int i;

    frame.append(0x3D); /* Nagłówek */
    frame.append(_frame_id); /* ID ramki */
    frame.append(data.length());
    frame.append(data);

    /* Liczymy sumę kontrolną */
    for(i = 0; i < frame.length(); i++) {
        checksum ^= frame.at(i);
    }
    frame.append(checksum);

    _frame_id++;

    _send_queue.append(frame);

    return true;
}

void ChangerEmulator::_send_current_state() {
    QByteArray packet;
    switch(_cdc_state) {
        case cdcStateBooting1: {
            packet.setRawData("\x11\x60\x06", 3);
            _send_packet(packet);
            _send_packet(packet);
            break;
        }
        case cdcStateBooting2: {
            packet.setRawData("\x15\x00\x25", 3);
            _send_packet(packet);
            break;
        }
        case cdcStatePause:
        case cdcStateStandby: {
            _send_status_packet();
            break;
        }
        case cdcStatePlaying: {
            _send_playing_state();
            _send_status_packet();
            break;
        }
    }
}

void ChangerEmulator::_send_status_packet(void) {
    QByteArray packet;

    packet.append(0x20); /* Status */
    packet.append(_cd_state);
    packet.append(_tray_state);
    packet.append(0x09);
    packet.append(0x05);
    packet.append(_current_cd);

    _send_packet(packet);
}

void ChangerEmulator::_send_random_status() {
    QByteArray packet;

    packet.append(0x25); /* Packet ID = RANDOM_STATUS */
    if (!_random_status)
        packet.append(0x03);
    else
        packet.append(0x07);

    _send_packet(packet);
}

void ChangerEmulator::_send_tray_status() {
    QByteArray packet;

    packet.append(0x26); /* Packet ID = TRAY_STATUS */
    packet.append(0x05);
    packet.append(_current_cd);
    packet.append(_cd_bitmap);
    packet.append(_cd_bitmap);

    _send_packet(packet);
}

void ChangerEmulator::_send_cd_summary() {
    QByteArray packet;

    qDebug() << "CDCEmu: Sending CD Summary";
    packet.append(0x46); /* Packet ID = CD_SUMMARY */
    /* 10 ścieżkowa płyta o dugości 60:00:00 */
    packet.append((char)0x10);
    packet.append((char)0x01);
    packet.append((char)0x00);
    packet.append((char)0x60);
    packet.append((char)0x00);
    packet.append((char)0x00);

    _send_packet(packet);
}

void ChangerEmulator::_send_cd_operation(void) {
    QByteArray packet;

    packet.append(0x21); /* Packet ID = CD_OPERATION */
    packet.append(_cd_state);

    _send_packet(packet);
}

void ChangerEmulator::_send_cd_check(bool present) {
    QByteArray packet;

    packet.append(0x13); /* Packet ID = CD_CHECK */

    packet.append(0x02);

    if (present)
        packet.append(0x86);
    else
        packet.append(0x26);

    _send_packet(packet);
}

void ChangerEmulator::_send_playing_state() {
    QByteArray packet;

    packet.append(0x47); /* Packet ID = PLAYING */

    packet.append(_bcd(_track_num));
    packet.append(0x01);
    packet.append(_bcd(_cd_time.hour()));
    packet.append(_bcd(_cd_time.minute()));
    packet.append(_bcd(_cd_time.second()));
    packet.append((char)0x00);
    packet.append(_bcd(_track_time.hour()));
    packet.append(_bcd(_track_time.minute()));
    packet.append(_bcd(_track_time.second()));
    packet.append((char)0x00);

    _send_packet(packet);
}

void ChangerEmulator::_send_track_change_frame(int type, int tracknum) {
    QByteArray packet;

    packet.append(0x27); /* Packet ID = TRACK_CHANGE */

    if ((type == trackChangeEntering) && (tracknum == 1)) {
        type |= 0x05;
    }

    packet.append(type);
    packet.append(_bcd(tracknum));
    packet.append(0x22);

    _send_packet(packet);
}

void ChangerEmulator::_recv_command(QByteArray &packet) {
    qDebug("CDCEmu: Command: 0x%X", packet.at(0));
    int tmp;

    switch (packet.at(0)) {
        case 0x00: {
            _cdc_state = cdcStateBooting1; /* Reset maszyny */
            break;
        }
        case 0x13: { /* PLAY */
            _cd_state = cdStateLoadingTrack;
            _send_cd_operation();
            _send_current_state();

            _cdc_state = cdcStatePlaying;
            emit playback_started();
            break;
        }
        case 0x17: { /* NEXT_TRACK */
            emit playback_next_pressed();
            next_track();
            break;
        }
        case 0x19: { /* STOP_PLAY */
            _cd_state = cdStateLoadingTrack;
            _send_cd_operation();
            _send_status_packet();

            _cd_state = cdStateNoCD;
            _send_cd_operation();
            _send_status_packet();

            _cdc_state = cdcStatePause;
            emit playback_stopped();
            break;
        }
        case 0x1C: {
            _cd_state = cdcStatePause;
            _send_cd_operation();
            _send_status_packet();
            emit playback_paused();
            break;
        }
        case 0x22: { /* PREV_TRACK */
            emit playback_prev_pressed();
            prev_track();
            break;
        }
        case 0x26: { /* LOAD_CD */
            tmp = packet.at(1);
            qDebug() << "CDEmu: Load CD:" << tmp;
            emit load_cd_pressed(tmp);
            break;
        }
        case 0x86: { /* REQUEST_CD_INFO */
            _send_cd_summary();
            break;
        }
    }
}

int ChangerEmulator::_bcd(int num) {
    return ((num / 10) << 4) | (num % 10);
}

void ChangerEmulator::start() {
    reset();
    _cdc_io_timer->start();
    _cdc_watchdog_timer->start();
}

void ChangerEmulator::stop() {
    _cdc_io_timer->stop();
    _cdc_watchdog_timer->stop();    
}

void ChangerEmulator::reset() {
    _cdc_reset_timeout = 5;
    _cdc_state = cdcStateBooting1;
    _cd_bitmap = 0xFC;
    _cd_state = cdStateNoCD;
    _tray_state = trayStateCDReady;
    _cd_time = QTime(0, 0);
    _track_time = QTime(0, 0);
    _track_num = 1;
}

void ChangerEmulator::next_track() {
    _send_track_change_frame(trackChangeLeaving, _track_num);

    _track_num++;
    if (_track_num >= 100)
        _track_num = 1;

    _track_time = QTime(0, 0);
    _cd_state = cdStateSearchingTrack;
    _send_cd_operation();
}

void ChangerEmulator::prev_track() {
    _send_track_change_frame(trackChangeLeaving, _track_num);

    _track_num--;
    if (_track_num < 1)
        _track_num = 99;

    _track_time = QTime(0, 0);
    _cd_state = cdStateSearchingTrack;
    _send_cd_operation();
}

void ChangerEmulator::load_cd(int num) {
    _track_time = QTime(0, 0);
    _cd_time = QTime(0, 0);
}
