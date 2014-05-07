#include "wndmain.h"
#include "ui_wndmain.h"
#include <QTime>
#include <QDebug>

WndMain::WndMain(QWidget *parent) : QMainWindow(parent), _ui(new Ui::WndMain) {
    _ui->setupUi(this);

    _main_board = MainBoard::get_instance();

    _update_timer = new QTimer();
    _update_timer->setSingleShot(false);
    _update_timer->setInterval(2000);
    connect(_update_timer, SIGNAL(timeout()), this, SLOT(_update_timer_tick()));
    _update_timer->start();
    _update_timer_tick();

    _wnd_menu = new WndMenu(this);
    _wnd_volume = new WndVolume(this);

    _current_source = -1;

    _display = new DisplayEmulator("can0", this);
    connect(_display, SIGNAL(state_change_request(bool)), this, SLOT(_display_state_change(bool)));
    connect(_display, SIGNAL(radio_text_changed(QString,int)), this, SLOT(_radio_text_changed(QString,int)));
    connect(_display, SIGNAL(icons_changed(uint16_t)), this, SLOT(_radio_icons_changed(uint16_t)));
    connect(_display, SIGNAL(radio_menu_hide()), _wnd_menu, SLOT(hide_menu()));
    connect(_display, SIGNAL(radio_menu_show(int)), _wnd_menu, SLOT(show_menu(int)));
    connect(_display, SIGNAL(radio_menu_set_item(int,QString,bool)), _wnd_menu, SLOT(set_menu_item(int,QString,bool)));

    connect(_main_board, SIGNAL(shutdown()), this, SLOT(_my_close()));
    connect(_main_board, SIGNAL(keypressed(uint8_t)), this, SLOT(_pilot_key_pressed(uint8_t)));

    _cd_changer = new ChangerEmulator(this);
    _cd_changer->start();
    _display_state_change(false);
    _radio_icons_changed(DISPLAY_ICON_MODE_NONE);

    _mp3_player = new MP3Player(this);
    connect(_mp3_player, SIGNAL(text_changed(QString)), this, SLOT(_set_radio_text(QString)));
    connect(_cd_changer, SIGNAL(playback_next_pressed()), _mp3_player, SLOT(next_track()));
    connect(_cd_changer, SIGNAL(playback_prev_pressed()), _mp3_player, SLOT(prev_track()));
    connect(_cd_changer, SIGNAL(playback_stopped()), _mp3_player, SLOT(stop()));
    connect(_cd_changer, SIGNAL(playback_started()), _mp3_player, SLOT(play()));
    connect(_cd_changer, SIGNAL(playback_paused()), _mp3_player, SLOT(pause()));
    connect(_cd_changer, SIGNAL(load_cd_pressed(int)), _mp3_player, SLOT(numkey_pressed(int)));

    _navit = new Navit(_ui->pageNavit);
    _navit->start();

    _scroll_timer = new QTimer(this);
    _scroll_timer->setSingleShot(false);
    connect(_scroll_timer, SIGNAL(timeout()), this, SLOT(_scroll_timer_tick()));

    _wnd_call = new WndCall(this);

    _bluetooth = new Bluetooth(this);
    connect(_main_board, SIGNAL(bluetooth_event()), _bluetooth, SLOT(new_event()));
    connect(_bluetooth, SIGNAL(state_changed(bool)), this, SLOT(_bluetooth_state_changed(bool)));
    connect(_bluetooth, SIGNAL(call_status_changed(int)), this, SLOT(_bluetooth_call_status_changed(int)));

    _bluetooth->new_event();
}

WndMain::~WndMain() {
    delete _bluetooth;
    delete _wnd_call;
    delete _navit;
    delete _update_timer;
    delete _cd_changer;
    delete _display;
    delete _wnd_menu;
    delete _ui;
}

void WndMain::changeEvent(QEvent *e) {
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        _ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void WndMain::_display_state_change(bool enabled) {
    if (!enabled)
        _ui->lRadioText->setText("Radio OFF");        
    else
        _ui->lRadioText->setText("");

    _ui->lSourceIcon->setText("");
    _display->set_state(enabled);
}

void WndMain::_radio_text_changed(QString text, int chan) {
    /* Detekcja źródła */
    int source = -1;

    //qDebug() << "text =" << text;

    if (text == "   PAUSE    ") { /* Detekcja wyciszenia */
        _paused = true;
    }
    else {
        _paused = false;
    }

    if (text.startsWith("VOLUME ")) { /* Regulacja głosności */
        _wnd_volume->show_volume(text.mid(7, 2).toInt());        
        return;
    }
    else if (text == "AUX         ") {
        source = sourceAUX;
        text = "BLUETOOTH   ";
    }
    else if ((text == "CD          ") || (text == "  LOAD CD   ") || (text.startsWith("ALB ")) || (text.startsWith("LOAD ALB ")) || (text.length() > 12)) {
        source = sourceCD;
    }
    else if ((text == "CD CHANGER  ") || (text.startsWith("CD "))) {
        source = sourceCDChanger;
    }
    else if ((text == "RADIO FM    ")) {
        source = sourceFM;
    }

    if ((_current_source != source) && (source != -1))
        _switch_source(source);

    if ((_current_source != sourceCDChanger) || (_mp3_player->is_paused())) { /* Kiedy aktywny jest emulator zmieniarki i nie jest zapauzowane teksty ustawia mp3 player */
        _set_radio_text(text);
    }
    else {
        qDebug() << "WndMain: Radio text changed to" << text;
    }
}

void WndMain::_radio_icons_changed(uint16_t icon_mask) {
    QString icons;

    if ((icon_mask & DISPLAY_ICON_NO_NEWS) == 0) {
        icons += "NEWS\n";
    }

    if ((icon_mask & DISPLAY_ICON_NO_AFRDS) == 0) { /* Jeżeli jest AF-RDS to na pewno źródłem jest radio */
        icons += "AF-RDS";
        if (_current_source != sourceFM)
            _switch_source(sourceFM);
    }

    if ((icon_mask & DISPLAY_ICON_NO_TRAFFIC) == 0) {
        icons += "  TRAFFIC";
    }

    _ui->lRadioIcons->setText(icons);
}

void WndMain::_update_timer_tick() {
    //_ui->lTemp->setText(QString::fromUtf8("%1°C").arg(_main_board->get_temp(), 3, 'f', 0));
    _ui->lTemp->setText(QString::fromUtf8("--°C"));
    _ui->lTime->setText(QTime::currentTime().toString("HH:mm"));
}

void WndMain::_scroll_timer_tick() {
    _ui->lRadioText->setText(_radio_text.mid(_radio_text_pos, RADIO_TEXT_MAX_LEN));
    _radio_text_pos++;
    _scroll_timer->setInterval(200);

    if (_radio_text_pos >= _radio_text.length() - 3) {
        _radio_text_pos = 0;
        _scroll_timer->stop();
        _ui->lRadioText->setText(_radio_text.mid(_radio_text_pos, RADIO_TEXT_MAX_LEN));
    }
}

void WndMain::_my_close() {
    _navit->stop();
    QProcess::execute("poweroff");
    close();
}

void WndMain::_pilot_key_pressed(uint8_t keymask) {
    static uint8_t old_keymask = 0;
    int call_status = _bluetooth->call_status();

    //qDebug() << "keymask =" << keymask;
    /* Klawisze które można przytrzymać */
    if ((keymask & PILOT_KEY_PAUSE) == PILOT_KEY_VOLUP) {
        if (call_status == callStatusIncomimg)
            _bluetooth->accept_call();
        else
            _display->send_key(DISPLAY_KEY_VOLUME_UP);
    }

    if ((keymask & PILOT_KEY_PAUSE) == PILOT_KEY_VOLDOWN) {
        if (call_status == callStatusIncomimg)
            _bluetooth->reject_call();
        else
            _display->send_key(DISPLAY_KEY_VOLUME_DOWN);
    }
    /* Pozostałe klawisze */
    if (keymask == old_keymask)
        return;

    if ((keymask & PILOT_KEY_LOAD) == PILOT_KEY_LOAD)
        _display->send_key(DISPLAY_KEY_LOAD);

    if ((keymask & PILOT_KEY_PAUSE) == PILOT_KEY_PAUSE) {
        if ((call_status == callStatusOutgoing) || (call_status == callStatusTalking))
            _bluetooth->terminate_call();
        else
            _display->send_key(DISPLAY_KEY_PAUSE);
    }

    if ((keymask & PILOT_KEY_SOURCE_L) == PILOT_KEY_SOURCE_L)
        _display->send_key(DISPLAY_KEY_SRC_LEFT);
    if ((keymask & PILOT_KEY_SOURCE_R) == PILOT_KEY_SOURCE_R)
        _display->send_key(DISPLAY_KEY_SRC_RIGHT);

    if ((keymask & PILOT_ROLL_NEXT) == PILOT_ROLL_NEXT)
        _display->send_key(DISPLAY_KEY_ROLL_DOWN);
    if ((keymask & PILOT_ROLL_PREV) == PILOT_ROLL_PREV)
        _display->send_key(DISPLAY_KEY_ROLL_UP);


    old_keymask = keymask;
}

void WndMain::_set_radio_text(QString text) {
    _radio_text = text;
    _radio_text_pos = 0;
    if (text.length() <= RADIO_TEXT_MAX_LEN) {
        _scroll_timer->stop();
        _ui->lRadioText->setText(text);
    }
    else {
        _scroll_timer_tick();
        _scroll_timer->setInterval(600);
        _scroll_timer->start();
    }
}

void WndMain::_bluetooth_state_changed(bool connected) {
    if (connected) {
        _ui->lBluetoothState->setPixmap(QPixmap(":/icons/bluetooth_active.png"));
    }
    else {
        _ui->lBluetoothState->setPixmap(QPixmap(":/icons/bluetooth_inactive.png"));
    }
}

void WndMain::_bluetooth_call_status_changed(int newstate) {
    static int _old_source = -1;

    if ((newstate == callStatusOutgoing) || (newstate == callStatusIncomimg)) { /* Coś dzwoni */

        _old_source = _current_source;

        _set_source(sourceAUX);
    }
    else if (newstate == callStatusIdle) {
        if (_old_source != -1)
            _set_source(_old_source);
    }

    _wnd_call->update_status(newstate);
}

void WndMain::_set_source(int source) {
    /* Funkcja udając naciskanie przycisków na pilocie wybiera zadane źródło
     * Jednak żeby nie było za prosto dodatkowe założenia:
     * 1. Unikamy przechodzenia przez CD oraz USB
     * 2. Staramy się wybrać trasę która ma jak najmniej przełączeń
     * Kolejka źródeł:
     * FM -> CD -> CD CHANGER -> AUX
     */
    static int source_cost[4] = { 0x01, 0x03, 0x05, 0x01 }; /* "Koszty" wybrania poszczególnych źródeł */
    int lcost, rcost, i;

    if (_current_source == source) /* Nic do roboty */
        return;

    qDebug() << "WndMain: switch from" << _current_source << "to" << source;

    rcost = 0;
    lcost = 0;

    /* Liczymy koszt w prawo */
    i = _current_source;
    while(1) {
        i++;
        if (i > 3)
            i = 0;

        if (i == source)
            break;

        rcost += source_cost[i];
    }

    /* Liczymy koszt w lewo */
    i = _current_source;
    while(1) {
        i--;
        if (i < 0)
            i = 3;

        if (i == source)
            break;

        lcost += source_cost[i];
    }


    qDebug() << "WndMain: lcost =" << lcost << "rcost =" << rcost;

    i = _current_source;
    while(1) {
        if (i < 0)
            i = 3;
        else if (i > 3)
            i = 0;

        if (i == source)
            break;

        if (lcost < rcost) {
            _display->send_key(DISPLAY_KEY_SRC_LEFT);
            i--;
        }
        else {
            _display->send_key(DISPLAY_KEY_SRC_RIGHT);
            i++;
        }
        QApplication::processEvents();
    }

}

void WndMain::_switch_source(int newsrc) {
    if (_current_source == sourceCDChanger) {
        //_mp3_player->disable();
    }

    _current_source = newsrc;

    switch (_current_source) {
        case sourceFM: {
            _ui->lSourceIcon->setPixmap(QPixmap(":/icons/source_fm.png"));
            break;
        }
        case sourceCD: {
            _ui->lSourceIcon->setPixmap(QPixmap(":/icons/source_cd.png"));
            break;
        }
        case sourceCDChanger: {
            _ui->lSourceIcon->setPixmap(QPixmap(":/icons/source_usb.png"));
            break;
        }
        case sourceAUX: {
            _ui->lSourceIcon->setPixmap(QPixmap(":/icons/phone.png"));
            break;
        }
    }

    qDebug() << "CarPI: Change source to" << _current_source;
}
