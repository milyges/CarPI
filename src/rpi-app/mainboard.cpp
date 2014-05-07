#include "mainboard.h"
#include <QDebug>
#include <QByteArray>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <cmath>
#include <wiringPi.h>

#define I2C_SLAVE_ADDR        0x30

#define REG_POWER_STATE       0x00
#define REG_TEMP_HIGH         0x01
#define REG_TEMP_LOW          0x02
#define REG_KEY_CNT           0x03
#define REG_KEY_0             0x04
#define REG_KEY_1             0x05
#define REG_KEY_2             0x06
#define REG_KEY_3             0x07
#define REG_KEY_4             0x08
#define REG_KEY_5             0x09
#define REG_KEY_6             0x0A
#define REG_KEY_7             0x0B

#define REG_MAX               0x0C

#define REG_POWER_STATE_MASK  0x01
#define REG_POWER_ENABLED     (1 << 0) /* Zasilanie RPi, R/W */
#define REG_POWER_RADIO       (1 << 1) /* Radio włączone, R */
#define REG_POWER_IGNITION    (1 << 2) /* Zapłon włączony, R */

#define MAINBOARD_INT_PIN     6        /* Przerwanie od AVR */
#define MAINBOARD_BT_MODE_PIN 0        /* Bluetooth: CMD/DATA */
#define MAINBOARD_BT_INT_PIN  2        /* Bluetooth: Przerwanie */
#define MAINBOARD_IN_SEL_PIN  7        /* Przełączanie wejścia SPDIF */

MainBoard * MainBoard::_instance = NULL;

MainBoard::MainBoard(QObject *parent) : QObject(parent) {
    int fd;

    wiringPiSetup();

    _i2c_file = new QFile("/dev/i2c-1");
    _i2c_file->open(QIODevice::ReadWrite | QIODevice::Unbuffered);
    fd = _i2c_file->handle();
    if (ioctl(fd, I2C_SLAVE, I2C_SLAVE_ADDR) < 0) {
        qDebug() << "MainBoard: ioctl() failed!";
        return;
    }

    //qDebug() << "MainBoard: I2C Ready.";

    _shutdown_timer = new QTimer(this);
    _shutdown_timer->setInterval(1000);
    _shutdown_timer->setSingleShot(false);
    connect(_shutdown_timer, SIGNAL(timeout()), this, SLOT(_shutdown_timer_tick()));

    connect(this, SIGNAL(mainboard_new_event()), this, SLOT(mainboard_event()));

    /* Pin przerwania od AVRa */
    pinMode(MAINBOARD_INT_PIN, INPUT);
    pullUpDnControl(MAINBOARD_INT_PIN, PUD_UP);
    wiringPiISR(MAINBOARD_INT_PIN, INT_EDGE_RISING, &MainBoard::_mainboard_int);

    /* Piny bluetooth: tryb */
    pinMode(MAINBOARD_BT_MODE_PIN, OUTPUT);
    bluetooth_set_mode(false);

    /* Piny bluetooth: przerwanie */
    pinMode(MAINBOARD_BT_INT_PIN, INPUT);
    pullUpDnControl(MAINBOARD_BT_INT_PIN, PUD_UP);
    wiringPiISR(MAINBOARD_BT_INT_PIN, INT_EDGE_RISING, &MainBoard::_bluetooth_int);
}

MainBoard::~MainBoard() {
    _i2c_file->close();
    if (_i2c_file)
        delete _i2c_file;
}

float MainBoard::get_temp() {
    return _last_temp;
}

void MainBoard::bluetooth_set_mode(bool cmd) {
    if (cmd)
        digitalWrite(MAINBOARD_BT_MODE_PIN, 0);
    else
        digitalWrite(MAINBOARD_BT_MODE_PIN, 1);
}

void MainBoard::mainboard_event() {
    int i;
    bool anykey = false;
//    qDebug() << "MainBoard: New event!";

    QByteArray data = _i2c_file->read(REG_MAX + 1);
    data = data.remove(0, 1);
#if 0
    qDebug() << "Dumping registers: ";
    for(i = 0; i < REG_MAX; i++) {
        qDebug("REG %d: 0x%X", i, data.at(i));
    }
#endif

    /* Obsługa zasilania */
    if ((data[REG_POWER_STATE] & (REG_POWER_IGNITION | REG_POWER_RADIO)) == 0) { /* Radio i zapłon wyłączone */
        if (!_shutdown_timer->isActive()) {
            //qDebug() << "MainBoard: starting shutdown timer";
            _shutdown_timeout = 600; /* 10 minut */
            _shutdown_timer->start();
        }
    }
    else {
        if (_shutdown_timer->isActive()) {
            //qDebug() << "MainBoard: shutdown canceled";
            _shutdown_timer->stop();
        }
    }

    /* Temperatura */
    _calculate_temp((data[REG_TEMP_HIGH] << 7) | (data[REG_TEMP_LOW] & 0x7F));

    /* Klawisze pilota */
    for(i = 0; i < 8; i++) {
        if (data.at(REG_KEY_0 + i) > 0) { /* Klawisz wciśnięty */
            //qDebug("MainBoard: key %x pressed", data.at(REG_KEY_0 + i));
            anykey = true;
            emit keypressed(data.at(REG_KEY_0 + i));
        }
    }

    if (!anykey) {
        emit keypressed(0);
    }
}

void MainBoard::_shutdown_timer_tick() {
    if (!--_shutdown_timeout) {
        _shutdown_timer->stop();
        qDebug() << "SHUTDOWN!!!";
        emit shutdown();
    }
}

MainBoard * MainBoard::get_instance() {
    if (!_instance)
        _instance = new MainBoard();

    return _instance;
}

void MainBoard::_calculate_temp(uint16_t raw) {
    double voltage, resistance;
    //qDebug("MainBoard: rawtemp: 0x%X", raw);
    if ((raw == 0x3FF) || (raw == 0x00)) { /* Zwarcie lub czujnik nie podłączony */
        return;
    }

    voltage = 5.0 * raw / 1023;
    //qDebug("MainBoard: ADC voltage = %fV", voltage);

    resistance = 10000 / (5.0 / voltage - 1);
    //qDebug("MainBoard: Termistor resistance: %fR", resistance);

    _last_temp = -27.2218 * log(resistance) + 238.3824;
    //qDebug("MainBoard: Temperature: %fC", _last_temp);
}

void MainBoard::_mainboard_int() {
    //qDebug() << "mainboard interrupt!";
    emit _instance->mainboard_new_event();
}

void MainBoard::_bluetooth_int() {
    //qDebug() << "bluetooth event!";
    emit _instance->bluetooth_event();
}
