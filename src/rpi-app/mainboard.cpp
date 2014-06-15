#include "mainboard.h"
#include <wiringPi.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <QDebug>
#include <QApplication>
#include <QByteArray>

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

    _radioPowerEnabled = false;
    _ignitionEnabled = false;

    wiringPiSetup();

    _i2c_bus = new QFile("/dev/i2c-1");
    _i2c_bus->open(QIODevice::ReadWrite | QIODevice::Unbuffered);
    fd = _i2c_bus->handle();
    if (ioctl(fd, I2C_SLAVE, I2C_SLAVE_ADDR) < 0) {
        qDebug() << "MainBoard: ioctl() failed!";
        QApplication::exit(1);
        return;
    }

    /* Pin przerwania od AVRa */
    pinMode(MAINBOARD_INT_PIN, INPUT);
    pullUpDnControl(MAINBOARD_INT_PIN, PUD_UP);
    wiringPiISR(MAINBOARD_INT_PIN, INT_EDGE_RISING, &MainBoard::_mainboardIntHandler);

    /* Piny bluetooth: tryb */
    pinMode(MAINBOARD_BT_MODE_PIN, OUTPUT);


    /* Piny bluetooth: przerwanie */
    pinMode(MAINBOARD_BT_INT_PIN, INPUT);
    pullUpDnControl(MAINBOARD_BT_INT_PIN, PUD_UP);
    wiringPiISR(MAINBOARD_BT_INT_PIN, INT_EDGE_RISING, &MainBoard::_bluetoothIntHandler);

    connect(this, SIGNAL(mainboardInt()), this, SLOT(_mainboardInt()));

    /* Zegar wyłączający zasilanie */
    _shutdownTimer = new QTimer(this);
    _shutdownTimer->setInterval(1000);
    _shutdownTimer->setSingleShot(false);
    connect(_shutdownTimer, SIGNAL(timeout()), this, SLOT(_shutdownTimerTick()));
}

MainBoard::~MainBoard() {
    _i2c_bus->close();
}

void MainBoard::_mainboardInt() {
    QByteArray data;
    int i;
    bool anykey = false;
    bool ignitionEnabled, radioPowerEnabled;

    //qDebug() << "MainBoard: Interrput!";

    data = _i2c_bus->read(REG_MAX + 1).remove(0, 1);
#if 0
    qDebug() << "Dumping registers: ";
    for(i = 0; i < REG_MAX; i++) {
        qDebug("REG %d: 0x%X", i, data.at(i));
    }
#endif

    /* Stan zasilania */
    ignitionEnabled = ((data.at(REG_POWER_STATE) & REG_POWER_IGNITION) == REG_POWER_IGNITION);
    radioPowerEnabled = ((data.at(REG_POWER_STATE) & REG_POWER_RADIO) == REG_POWER_RADIO);

    //qDebug() << "ignition" << ignitionEnabled << ", radio" << radioPowerEnabled;

    if (_radioPowerEnabled != radioPowerEnabled) {
        _radioPowerEnabled = radioPowerEnabled;
        emit radioPowerChanged(radioPowerEnabled);
    }

    if (_ignitionEnabled != ignitionEnabled) {
        _ignitionEnabled = ignitionEnabled;
        emit ignitionChanged(ignitionEnabled);
    }

    if ((!ignitionEnabled) && (!radioPowerEnabled)) {
        if (!_shutdownTimer->isActive()) {
            _shutdownTimeout = 3600; /* Czas do zamknięcia systemu */
            _shutdownTimer->start();
        }
    }
    else {
        _shutdownTimer->stop();
    }

    /* Klawisze pilota */
    for(i = 0; i < 8; i++) {
        if (data.at(REG_KEY_0 + i) > 0) {
            anykey = true;
            emit keyStateChanged(data.at(REG_KEY_0 + i));
        }
    }

    /* Brak wciśniętego klawisza */
    if (!anykey) {
        emit keyStateChanged(0);
    }
}

void MainBoard::_shutdownTimerTick() {
    _shutdownTimeout--;

    if (_shutdownTimeout <= 0) {
        emit shutdown();
        _shutdownTimer->stop();
    }
}

MainBoard *MainBoard::getInstance() {
    if (!_instance)
        _instance = new MainBoard();

    return _instance;
}

void MainBoard::bluetoothSetMode(bool cmdMode) {
    if (cmdMode) {
        digitalWrite(MAINBOARD_BT_MODE_PIN, 0);
    }
    else {
        digitalWrite(MAINBOARD_BT_MODE_PIN, 1);
    }
}

void MainBoard::readState() {
    emit mainboardInt();
}

void MainBoard::_mainboardIntHandler() {
    emit _instance->mainboardInt();
}

void MainBoard::_bluetoothIntHandler() {
    emit _instance->bluetoothInt();
}
