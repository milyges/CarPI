#include "displayemulator.h"
#include <QDebug>
#include <QProcess>
#include <cerrno>
#include <cstring>
#include <unistd.h>

#define CAN_ID_SYNC_DISPLAY        0x3CF  /* Pakiet synchronizacyjny, wyświetlacz -> HU */
#define CAN_ID_SYNC_HU             0x3DF  /* Pakiet synchronizacyjny, HU -> wyświetlacz */
#define CAN_ID_DISPLAY_CONTROL     0x1B1  /* Konfiguracja wyświetlacza, HU -> wyświetlacz */
#define CAN_ID_DISPLAY_STATUS      0x1C1  /* Zmiana stanu wyświetlacza, wyświetlacz -> HU */
#define CAN_ID_SET_TEXT            0x121  /* Ustawienie tekstu na wyświetlaczu, HU -> wyświetlacz */
#define CAN_ID_KEY_PRESSED         0x0A9  /* Informacja o naciśnięciu klawisza, wyświetlacz -> HU */

#define CAN_ID_REPLY_FLAG          0x400  /* Flaga ustawiana dla odpowiedzi */

/* Bajty wypełniające */
#define DISPLAY_FILL_BYTE          0xA3
#define HU_FILL_BYTE               0x81

DisplayEmulator::DisplayEmulator(const char * can_iface, QObject *parent) : QObject(parent) {
    struct sockaddr_can addr;
    struct ifreq ifr;

    _can_reset();

    if ((_socket_fd = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
        qDebug("DisplayEmulator: socket(): fail");
        return;
    }

    strcpy(ifr.ifr_name, can_iface);
    ioctl(_socket_fd, SIOCGIFINDEX, &ifr);

    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    if (bind(_socket_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        qDebug("DisplayEmulator: Socket bind() failed!");
        close(_socket_fd);
        return;
    }

    _socket_notifier = new QSocketNotifier(_socket_fd, QSocketNotifier::Read, this);
    connect(_socket_notifier, SIGNAL(activated(int)), this, SLOT(_socket_recv(int)));
    _socket_notifier->setEnabled(true);

    _sync_timer = new QTimer(this);
    _sync_timer->setSingleShot(false);
    _sync_timer->setInterval(100);
    connect(_sync_timer, SIGNAL(timeout()), this, SLOT(_sync_timer_tick()));
    _sync_timeout = 0;

    _blink_timer = new QTimer(this);
    _blink_timer->setInterval(2000);
    _blink_timer->setSingleShot(false);
    connect(_blink_timer, SIGNAL(timeout()), this, SLOT(_blink_timer_tick()));

}

DisplayEmulator::~DisplayEmulator() {
    delete _socket_notifier;
    delete _sync_timer;
}

void DisplayEmulator::set_state(bool enabled) {
    struct can_frame frame;

    frame.can_id = CAN_ID_DISPLAY_STATUS;
    frame.can_dlc = 8;
    frame.data[0] = (enabled) ? 0x02 : 0x00;
    frame.data[1] = 0x64;
    frame.data[2] = 0x0F;
    frame.data[3] = DISPLAY_FILL_BYTE;
    frame.data[4] = DISPLAY_FILL_BYTE;
    frame.data[5] = DISPLAY_FILL_BYTE;
    frame.data[6] = DISPLAY_FILL_BYTE;
    frame.data[7] = DISPLAY_FILL_BYTE;

    _socket_send(&frame);
}

void DisplayEmulator::send_key(int keycode) {
    struct can_frame frame;

    frame.can_id = 0x0A9;
    frame.can_dlc = 8;
    frame.data[0] = 0x03;
    frame.data[1] = 0x89;
    frame.data[2] = (keycode >> 8);
    frame.data[3] = (keycode & 0xFF);
    frame.data[4] = DISPLAY_FILL_BYTE;
    frame.data[5] = DISPLAY_FILL_BYTE;
    frame.data[6] = DISPLAY_FILL_BYTE;
    frame.data[7] = DISPLAY_FILL_BYTE;

    _socket_send(&frame);
}


int DisplayEmulator::_socket_send(struct can_frame * frame) {
    //qDebug("DisplayEmulator: _socket_send: id=%x, data=%02X%02X%02X%02X%02X%02X%02X%02X", frame->can_id, frame->data[0], frame->data[1], frame->data[2], frame->data[3], frame->data[4], frame->data[5], frame->data[6], frame->data[7]);
    return write(_socket_fd, frame, sizeof(struct can_frame));
}

void DisplayEmulator::_socket_recv(int fd) {
    struct can_frame frame;
    int len;

    if (fd != _socket_fd)
        return;

    len = read(_socket_fd, &frame, sizeof(struct can_frame));
    if (len <= 0)
        return;

    //qDebug("DisplayEmulator: _socket_recv: id=%x, data=%02X%02X%02X%02X%02X%02X%02X%02X", frame.can_id, frame.data[0], frame.data[1], frame.data[2], frame.data[3], frame.data[4], frame.data[5], frame.data[6], frame.data[7]);
    switch(frame.can_id) {
        case CAN_ID_SYNC_HU: _packet_recv_sync(&frame); break;
        case CAN_ID_DISPLAY_CONTROL: _packet_recv_dispctrl(&frame); break;
        case CAN_ID_SET_TEXT: _packet_recv_settext(&frame); break;
    }
}

void DisplayEmulator::_sync_timer_tick(void) {
    if (_sync_timeout == 0) { /* Utrata synchronizacji */
        qDebug() << "DisplayEmulator: Lost sync";
        _sync_timer->stop();
        _can_reset();
    }
    else if (_sync_timeout < 0) {
        _sync_timeout = 50;
        _register_id(CAN_ID_DISPLAY_STATUS);
        _register_id(CAN_ID_KEY_PRESSED);
    }
    else {
        _sync_timeout--;
    }
}

void DisplayEmulator::_blink_timer_tick() {
    if (!_blink_texts.size()) { /* Koniec migania */
        _blink_timer->stop();
        _blink_idx = 0;
        return;
    }

    if (_blink_idx >= _blink_texts.size()) {
        _blink_idx = 0;
    }

    emit radio_text_changed(_blink_texts.at(_blink_idx), 0);

    _blink_idx++;
}

void DisplayEmulator::_send_reply(struct can_frame * frame, bool last) {
    int i;

    frame->can_id |= CAN_ID_REPLY_FLAG;
    frame->can_dlc = 8;

    i = 0;
    if (last) {
        frame->data[i++] = 0x74;
    }
    else {
        frame->data[i++] = 0x30;
        frame->data[i++] = 0x00;
        frame->data[i++] = 0x01;
    }

    for( ; i < 8; i++)
        frame->data[i] = DISPLAY_FILL_BYTE;

    _socket_send(frame);
}

void DisplayEmulator::_register_id(uint16_t id) {
    struct can_frame frame;

    frame.can_id = id;
    frame.can_dlc = 8;
    frame.data[0] = 0x70;
    frame.data[1] = DISPLAY_FILL_BYTE;
    frame.data[2] = DISPLAY_FILL_BYTE;
    frame.data[3] = DISPLAY_FILL_BYTE;
    frame.data[4] = DISPLAY_FILL_BYTE;
    frame.data[5] = DISPLAY_FILL_BYTE;
    frame.data[6] = DISPLAY_FILL_BYTE;
    frame.data[7] = DISPLAY_FILL_BYTE;

    _socket_send(&frame);
}

void DisplayEmulator::_packet_recv_sync(can_frame * frame) {
    if (frame->data[0] == 0x79) { /* Zwykły PING */
        frame->can_id = CAN_ID_SYNC_DISPLAY;
        frame->data[0] = 0x69;
        frame->data[1] = 0x00;
        frame->data[2] = DISPLAY_FILL_BYTE;
        frame->data[3] = DISPLAY_FILL_BYTE;
        frame->data[4] = DISPLAY_FILL_BYTE;
        frame->data[5] = DISPLAY_FILL_BYTE;
        frame->data[6] = DISPLAY_FILL_BYTE;
        frame->data[7] = DISPLAY_FILL_BYTE;
        _socket_send(frame);

        if (_sync_timeout > 0) /* Podbijamy licznik */
            _sync_timeout = 50;
    }
    else if (frame->data[0] == 0x7A) { /* Rozpoczęcie synchronizacji */
        //qDebug("Sync start");
        frame->can_id = CAN_ID_SYNC_DISPLAY;
        frame->data[0] = 0x61;
        frame->data[1] = 0x11;
        frame->data[2] = 0x00;
        frame->data[3] = DISPLAY_FILL_BYTE;
        frame->data[4] = DISPLAY_FILL_BYTE;
        frame->data[5] = DISPLAY_FILL_BYTE;
        frame->data[6] = DISPLAY_FILL_BYTE;
        frame->data[7] = DISPLAY_FILL_BYTE;
        _socket_send(frame);

        _sync_timeout = -1;
        _sync_timer->start();
    }
}

void DisplayEmulator::_packet_recv_dispctrl(can_frame * frame) {
    bool enabled;

    if (frame->data[0] == 0x70) {
        _send_reply(frame);
        return;
    }

    enabled = (frame->data[2] == 0x02) ? true : false;
    emit state_change_request(enabled);

    _send_reply(frame);
}

void DisplayEmulator::_packet_recv_settext(can_frame *frame) {
    static char buf[32];
    static int bufptr = 0;
    static uint8_t icons, mode, chan, loc;
    static bool menu_visible = false;
    static QString long_text;
    int ptr = 0, max, idx;
    bool fullscreen, checked;
    char c;

    if (frame->data[0] == 0x70) {
        _send_reply(frame);
    }
    else if (frame->data[0] == 0x04) { /* Ustaw tylko ikonki */
        _send_reply(frame);
    }
    else if (frame->data[0] == 0x10) { /* Ustaw tekst */
        bufptr = 0x00;
        buf[0] = '\0';
        if (frame->data[1] == 0x1C) { /* Tekst + ikony */
            icons = frame->data[3];
            mode = frame->data[5];            
            ptr = 6;
            emit icons_changed(icons);
        }
        else if (frame->data[1] == 0x19) { /* Sam tekst */
            ptr = 3;
        }

        chan = frame->data[ptr++] & 7;
        loc = frame->data[ptr++];

        for(; ptr < 8; ptr++) {
            buf[bufptr++] = frame->data[ptr];
        }
        _send_reply(frame, false);
    }
    else if (frame->data[0] > 0x20) {
        ptr = 1;
        while(ptr < 8) {
              buf[bufptr] = frame->data[ptr++];
              if (!buf[bufptr])
                  break;

              bufptr++;
        }

        if (ptr < 8) { /* Koniec danych */
            _send_reply(frame);
            QString text;
            /* Interesuje nas tylko wersja dla nowych wyświetlaczy */
            for(ptr = 0; ptr < 12; ptr++) {
                c = char(buf[9 + ptr] & 0x7F);
                if (c < ' ') {
                    switch (c) {
                        case 0x07: text += QString::fromUtf8("⇧"); break;
                        case 0x08: text += QString::fromUtf8("⇩"); break;
                        case 0x09: text += QString::fromUtf8("⇨"); break;
                        case 0x0A: text += QString::fromUtf8("⇦"); break;
                        default: {
                            qDebug() << "DisplayEmulator: unknown special char " << (int)c;
                            text += ' ';
                        }
                    }
                }
                else {
                    text += c;
                }
            }

            //qDebug() << " loc =" << loc << " buf = " << buf << "text =" << text;
            max = (loc >> 5) & 7;
            idx = (loc >> 2) & 7;
            checked = ((loc & 0x01) == 0x01);
            fullscreen = ((loc & 0x02) == 0x02);

            //qDebug() << "max =" << max << " idx =" << idx << " checked =" << checked << "fs =" << fullscreen;

            if ((max > 0) && (fullscreen) && (!checked)) { /* Tekst migający */
                //qDebug() << "blink" << text;
                _blink_add_text(text, idx);
            }
            else { /* Teskt nie migający */
                if (!_blink_texts.isEmpty())
                    _blink_texts.clear();
            }

            if ((max > 0) && (!fullscreen)) { /* Tryb Menu */
                if (!menu_visible) {
                    emit radio_menu_show(max + 1);
                    menu_visible = true;
                }
                emit radio_menu_set_item(idx, text, checked);
                return;
            }
            else { /* Bez menu */
                if (menu_visible) {
                    emit radio_menu_hide();
                    menu_visible = false;
                }
            }

            if ((max == 0) && (checked)) { /* Zwykły tekst */
                emit radio_text_changed(text, chan);
            }
            else if ((fullscreen) && (checked)) { /* Tryb "pełnoekranowy" */
                if (idx == 0) {
                    long_text = text.left(8);
                }
                else {
                    long_text += text.left(8);

                    if (idx == max) {
                        emit radio_text_changed(long_text, 0);
                    }
                }
                //qDebug() << "LongText" << long_text;
            }
        }
        else {
            _send_reply(frame, false);
        }
    }
}

void DisplayEmulator::_can_reset() {
    qDebug() << "DisplayEmulator: Reseting can0";
    QProcess::execute("ifdown can0");
    QProcess::execute("ifup can0");
}

void DisplayEmulator::_blink_add_text(QString text, int pos) {
    if (pos >= _blink_texts.size())
        _blink_texts.append(text);
    else
        _blink_texts.replace(pos, text);

    if (!_blink_timer->isActive()) {
        _blink_idx = 0;
        _blink_timer->start();
    }
}
