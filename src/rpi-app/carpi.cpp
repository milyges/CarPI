#include "carpi.h"
#include <QDebug>
#include <QApplication>
#include "navit.h"

CarPI::CarPI(QObject * parent): QObject(parent) {
    qDebug() << "CarPI: Starting";

    _sourceCurrent = sourceUnknown;
    _sourcePaused = false;

    /* Kontroler płyty głównej urządzenia */
    _mainboard = MainBoard::getInstance();

    /* Emulator wyświetlacza radia */
    _displayEmulator = DisplayEmulator::getInstance();

    /* Emulator zmieniarki CD */
    _changerEmulator = ChangerEmulator::getInstance();

    /* Kontroler modułu bluetooth */
    _bluetooth = Bluetooth::getInstance();

    /* Odtwarzacz MP3 */
    _mp3Player = MP3Player::getInstance();

    /* Nawigacja */
    _navit = Navit::getInstance();

    /* Interfejs OBD II */
    _elm327 = Elm327::getInstance();

    _atmosphericPressure = 100; /* Zakładamy ciśnienie 1000hPa */

    connect(_mainboard, SIGNAL(radioPowerChanged(bool)), _displayEmulator, SLOT(radioPowerChanged(bool)));
    connect(_mainboard, SIGNAL(keyStateChanged(int)), this, SLOT(_pilotKeyStateChanged(int)));
    connect(_mainboard, SIGNAL(ignitionChanged(bool)), this, SLOT(_ignitionStateChanged(bool)));
    connect(_mainboard, SIGNAL(shutdown()), this, SLOT(shutdown()));

    connect(_displayEmulator, SIGNAL(displayTextChanged(QString)), this, SLOT(_displayTextChanged(QString)));
    connect(_displayEmulator, SIGNAL(displayIconsChanged(int)), this, SLOT(_displayIconsChanged(int)));
    connect(_displayEmulator, SIGNAL(displayMenuShow(int)), this, SLOT(_displayMenuShow(int)));
    connect(_displayEmulator, SIGNAL(displayMenuHide()), this, SLOT(_displayMenuHide()));
    connect(_displayEmulator, SIGNAL(displayMenuItemUpdate(int,QString,bool)), this, SLOT(_displayMenuSetItem(int,QString,bool)));
    connect(this, SIGNAL(radioNewKeyEvent(int)), _displayEmulator, SLOT(sendKeyEvent(int)));

    connect(_bluetooth, SIGNAL(connectionStateChanged(bool)), this, SLOT(_bluetoothConnectionStateChanged(bool)));
    connect(_bluetooth, SIGNAL(callStateChanged(BluetoothCallState,QString)), this, SLOT(_bluetoothCallStateChanged(BluetoothCallState,QString)));

    connect(_changerEmulator, SIGNAL(playbackStarted()), _mp3Player, SLOT(play()));
    connect(_changerEmulator, SIGNAL(playbackPaused()), _mp3Player, SLOT(pause()));
    connect(_changerEmulator, SIGNAL(playbackStopped()), _mp3Player, SLOT(stop()));
    connect(_changerEmulator, SIGNAL(nextTrack()), _mp3Player, SLOT(nextTrack()));
    connect(_changerEmulator, SIGNAL(prevTrack()), _mp3Player, SLOT(prevTrack()));
    connect(_changerEmulator, SIGNAL(loadCD(int)), this, SLOT(_changerEmulatorLoadCD(int)));

    connect(_mp3Player, SIGNAL(textChanged(QString)), this, SLOT(_mp3PlayerTextChanged(QString)));
    connect(this, SIGNAL(mp3PlayerNextAlbum()), _mp3Player, SLOT(nextAlbum()));
    connect(this, SIGNAL(mp3PlayerPrevAlbum()), _mp3Player, SLOT(prevAlbum()));
    connect(this, SIGNAL(mp3PlayerSwitchDisplayMode()), _mp3Player, SLOT(switchDisplayMode()));

    connect(_elm327, SIGNAL(pidValueChanged(int,QVector<int>)), this, SLOT(_elm327PidChanged(int,QVector<int>)));
    connect(_elm327, SIGNAL(voltageChanged(double)), this, SLOT(_elm327VoltageChanged(double)));
    connect(this, SIGNAL(elm327addWatchPid(int)), _elm327, SLOT(addWatchPid(int)));
    connect(this, SIGNAL(elm327start()), _elm327, SLOT(start()));
    connect(this, SIGNAL(elm327stop()), _elm327, SLOT(stop()));

    _mainboard->readState();

    emit elm327addWatchPid(0x05); /* Temperatura wody */
    emit elm327addWatchPid(0x04); /* Obciążenie silnika */
    emit elm327addWatchPid(0x0B); /* Ciśnienie absolutne w kolektorze dolotowym */
    emit elm327addWatchPid(0x0F); /* Temperatura powietrza w dolocie */
    emit elm327addWatchPid(0x23); /* Ciśnienie paliwa */
}

CarPI::~CarPI() {

}

void CarPI::radioSendKey(CarPIKey key) {
    switch(key) {
        case keyLoad: emit radioNewKeyEvent(DISPLAY_KEY_LOAD); break;
        case keyVolDown: emit radioNewKeyEvent(DISPLAY_KEY_VOLUME_DOWN); break;
        case keyVolUp: emit radioNewKeyEvent(DISPLAY_KEY_VOLUME_UP); break;
        case keyPause: emit radioNewKeyEvent(DISPLAY_KEY_PAUSE); break;
        case keySrcL: emit radioNewKeyEvent(DISPLAY_KEY_SRC_LEFT); break;
        case keySrcR: emit radioNewKeyEvent(DISPLAY_KEY_SRC_RIGHT); break;
        case keyScrollDown: emit radioNewKeyEvent(DISPLAY_KEY_ROLL_DOWN); break;
        case keyScrollUp: emit radioNewKeyEvent(DISPLAY_KEY_ROLL_UP); break;
    }
}

void CarPI::updateStatus() {
    _mainboard->readState();
    emit bluetoothConnectionStateChanged(_bluetooth->isConnected());
}

CarPISource CarPI::currentSource() {
    return _sourceCurrent;
}

QString CarPI::lastRadioText() {
    return _lastRadioText;
}

void CarPI::_displayTextChanged(QString text) {
    enum CarPISource source = sourceUnknown;

    if (text.isEmpty()) { /* Radio wyłączone */
        _sourceCurrent = sourceUnknown;
    }

    //qDebug() << "CarPI: Display text changed to" << text;

    /* Detekcja pauzy */
    if (text == "   PAUSE    ") {
        if (!_sourcePaused) {
            _sourcePaused = true;
            emit pauseStateChanged(true);
            _lastRadioText = text;
            emit radioTextChanged(text);
        }
        return;
    }
    else {
        if (_sourcePaused) {
            _sourcePaused = false;
            emit pauseStateChanged(false);
        }
    }

    /* Detekcja regulacji głosności */
    if (text.startsWith("VOLUME ")) {
        emit volumeChanged(text.mid(7, 2).toInt());
        return;
    }

    /* Detekcja źródła */
    if (text == "AUX         ") {
        source = sourceAUX;
        text = "BLUETOOTH   "; /* Podmieniamy AUX na BLUETOOTH */
    }
    else if ((text == "CD          ") || (text == "  LOAD CD   ") || (text.startsWith("ALB ")) || (text.startsWith("LOAD ALB ")) || (text.length() > 12)) {
        source = sourceCD;
    }
    else if ((text == "CD CHANGER  ") || (text.startsWith("CD "))) {
        source = sourceCDChanger;
        if (_sourceCurrent != source) {
            text = "USB";
            emit radioTextChanged(text);
        }
    }
    else if ((text == "RADIO FM    ")) {
        source = sourceFM;
    }

    if ((_sourceCurrent != source) && (source != sourceUnknown)) {
        _sourceCurrent = source;
        emit sourceChanged(_sourceCurrent);
    }

    if (_sourceCurrent != sourceCDChanger) {
        _lastRadioText = text;
        emit radioTextChanged(text);
    }
}

void CarPI::_displayIconsChanged(int iconmask) {
    QString icons;
    bool news, rds, traffic;

    if ((iconmask & DISPLAY_ICON_NO_NEWS) == 0) {
        news = true;
    }
    else {
        news = false;
    }

    if ((iconmask & DISPLAY_ICON_NO_AFRDS) == 0) { /* Jeżeli jest AF-RDS to na pewno źródłem jest radio */
        icons += "AF-RDS";
        rds = true;

        if (_sourceCurrent != sourceFM) {
            _sourceCurrent = sourceFM;
            emit sourceChanged(_sourceCurrent);
        }
    }
    else {
        rds = false;
    }

    if ((iconmask & DISPLAY_ICON_NO_TRAFFIC) == 0) {
        traffic = true;
    }
    else {
        traffic = false;
    }

    emit radioIconsChanged(news, traffic, rds);
}

void CarPI::_displayMenuShow(int items) {
    emit menuShow(items);
}

void CarPI::_displayMenuHide() {
    emit menuHide();
}

void CarPI::_displayMenuSetItem(int itemno, QString text, bool isSelected) {
    emit menuSetItem(itemno, text, isSelected);
}

void CarPI::_bluetoothConnectionStateChanged(bool isConnected) {
    emit bluetoothConnectionStateChanged(isConnected);
}

void CarPI::_bluetoothCallStateChanged(BluetoothCallState state, QString number) {
    static enum CarPISource oldSource = sourceUnknown;
    static enum BluetoothCallState oldState = callStateIdle;

    if (state != oldState) {
        if ((state == callStateOutgoing) || (state == callStateIncoming)) {
            oldSource = _sourceCurrent;
            _switchToSoruce(sourceAUX);
        }
        else if ((state == callStateIdle) && (oldSource != sourceUnknown)) {
            _switchToSoruce(oldSource);
            oldSource = sourceUnknown;
        }

        oldState = state;
    }

    emit bluetoothCallStateChanged(state, number);
}

void CarPI::_pilotKeyStateChanged(int keymask) {
    static int oldKeymask = 0;
    static int timeout = 0;

    //qDebug() << "CarPI: pilot keymask changed to:" << keymask;

    if ((keymask & PILOT_KEY_PAUSE) == PILOT_KEY_VOLUP) {
        //qDebug() << "CarPI: VOL UP";

        if (--timeout <= 0) {
            emit pilotKeyPressed(keyVolUp);
            timeout = 3;
        }

        return;
    }

    if ((keymask & PILOT_KEY_PAUSE) == PILOT_KEY_VOLDOWN) {
        //qDebug() << "CarPI: VOL UP";

        if (--timeout <= 0) {
            emit pilotKeyPressed(keyVolDown);
            timeout = 3;
        }

        return;
    }

    if (!keymask) {
       timeout = 0;
       oldKeymask = 0;
       return;
    }

    /* Pozostałe klawisze */
    if (keymask == oldKeymask) {
        return;
    }


    timeout = 0;
    if ((keymask & PILOT_KEY_LOAD) == PILOT_KEY_LOAD) { /* Używamy tego klawisza do wejscia/wyjścia w tryb zarządzania CarPI */
        emit pilotKeyPressed(keyLoad);
    }

    if ((keymask & PILOT_KEY_PAUSE) == PILOT_KEY_PAUSE) {
        emit pilotKeyPressed(keyPause);
    }

    if ((keymask & PILOT_KEY_SOURCE_L) == PILOT_KEY_SOURCE_L) {
        emit pilotKeyPressed(keySrcL);
    }
    if ((keymask & PILOT_KEY_SOURCE_R) == PILOT_KEY_SOURCE_R) {
        emit pilotKeyPressed(keySrcR);
    }

    if ((keymask & PILOT_ROLL_NEXT) == PILOT_ROLL_NEXT) {
        emit pilotKeyPressed(keyScrollDown);
    }

    if ((keymask & PILOT_ROLL_PREV) == PILOT_ROLL_PREV) {
        emit pilotKeyPressed(keyScrollUp);
    }

    oldKeymask = keymask;
}

void CarPI::_mp3PlayerTextChanged(QString text) {
    if (_sourceCurrent != sourceCDChanger)
        return;

    emit radioTextChanged(text);
}

void CarPI::_changerEmulatorLoadCD(int num) {
    switch(num) {
        case 0x01: emit mp3PlayerPrevAlbum(); break;
        case 0x02: emit mp3PlayerNextAlbum(); break;
        case 0x05: emit mp3PlayerSwitchDisplayMode(); break;
    }
}

void CarPI::_ignitionStateChanged(bool enabled) {
    qDebug() << "CarPI: Ignition changed to" << enabled;

    if (enabled) {
        _navit->start();
        emit elm327start();
    }
    else {
        _navit->stop();
        emit elm327stop();
    }
}

void CarPI::shutdown() {
    QProcess::execute("poweroff");
    QApplication::exit(0);
}

void CarPI::_elm327PidChanged(int pid, QVector<int> data) {
    switch(pid) {
        case 0x04: { /* Obciążenie silnika */
            if (data.size() == 1) {
                emit dialValueChanged(dialEngineLoad, data.at(0) * 100 / 255);
            }
            else {
                emit dialValueChanged(dialEngineLoad, 0);
            }
            break;
        }
        case 0x05: { /* Temperatura płynu chłodzącego */
            if (data.size() == 1) {
                emit dialValueChanged(dialCoolantTemp, data.at(0) - 40);
            }
            break;
        }
        case 0x0B: { /* Ciśnienie absolutne w kolektorze dolotowym */
            if (data.size() == 1) {
                /* OBD II podaje ciśnienie absolutne, jeżeli odejmiemy ciśnienie atmosferyczne uzyskamy nad/pod ciśnienie w kolektorze ssącym w kPa.
                 * Kolejnym krokiem jest przeliczenie jednostki, używamy PSI */
                emit dialValueChanged(dialBoost, (data.at(0) - _atmosphericPressure) * 0.145037738);
            }
            else {
                emit dialValueChanged(dialBoost, 0);
            }
            break;
        }
        case 0x0F: {
            if (data.size() == 1) {
                emit dialValueChanged(dialIntakeTemp, data.at(0) - 40);
            }

            break;
        }
        case 0x23: { /* Ciśnienie w listwie common-rail */
            if (data.size() == 2) {
                emit dialValueChanged(dialFuelPressure, (((data.at(0) * 256) + data.at(1)) * 10) / 1000);
            }
            else {
                emit dialValueChanged(dialFuelPressure, 0);
            }
            break;
        }
    }
}

void CarPI::_elm327VoltageChanged(double voltage) {
    emit dialValueChanged(dialVoltage, voltage);
}

/* Funkcja udając naciskanie przycisków na pilocie wybiera zadane źródło
 * Jednak żeby nie było za prosto dodatkowe założenia:
 * 1. Unikamy przechodzenia przez CD oraz USB
 * 2. Staramy się wybrać trasę która ma jak najmniej przełączeń
 * Kolejka źródeł:
 * FM -> CD -> CD CHANGER -> AUX
 */
void CarPI::_switchToSoruce(CarPISource source) {
    static int sourceCost[4] = { 0x01, 0x03, 0x05, 0x01 }; /* "Koszty" wybrania poszczególnych źródeł */
    int lcost, rcost, i;

    if (_sourceCurrent == source)
        return;

    rcost = 0;
    lcost = 0;

    /* Liczymy koszt w prawo */
    i = _sourceCurrent;
    while(1) {
        i++;
        if (i > 3)
            i = 0;

        if (i == source)
            break;

        rcost += sourceCost[i];
    }

    /* Liczymy koszt w lewo */
    i = _sourceCurrent;
    while(1) {
        i--;
        if (i < 0)
            i = 3;

        if (i == source)
            break;

        lcost += sourceCost[i];
    }

    i = _sourceCurrent;
    while(1) {
        if (i < 0)
            i = 3;
        else if (i > 3)
            i = 0;

        if (i == source)
            break;

        if (lcost < rcost) {
            emit radioNewKeyEvent(DISPLAY_KEY_SRC_LEFT);
            i--;
        }
        else {
            emit radioNewKeyEvent(DISPLAY_KEY_SRC_RIGHT);
            i++;
        }
        QApplication::processEvents();
    }
}
