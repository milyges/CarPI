#include "displayemulator.h"
#include <QDebug>
#include <QProcess>

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

DisplayEmulator * DisplayEmulator::_instance = NULL;
QThread DisplayEmulator::_workerThread;

DisplayEmulator::DisplayEmulator(QObject *parent) : QObject(parent) {
    struct sockaddr_can addr;
    struct ifreq ifr;

    if ((_socketFd = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
        qDebug("DisplayEmulator: socket(): fail");
        return;
    }

    strcpy(ifr.ifr_name, DISPLAY_CAN_INTERFACE);
    ioctl(_socketFd, SIOCGIFINDEX, &ifr);

    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    if (bind(_socketFd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        qDebug("DisplayEmulator: Socket bind() failed!");
        close(_socketFd);
        return;
    }

    _socketNotifier = new QSocketNotifier(_socketFd, QSocketNotifier::Read, this);
    connect(_socketNotifier, SIGNAL(activated(int)), this, SLOT(_socketRecv(int)));
    _socketNotifier->setEnabled(true);

    _syncState = displaySyncLost;
    _syncTimer = new QTimer(this);
    _syncTimer->setInterval(1000);
    _syncTimer->setSingleShot(true);
    connect(_syncTimer, SIGNAL(timeout()), this, SLOT(_syncTimeout()));

    connect(this, SIGNAL(displayStatusChanged(bool)), this, SLOT(_packetSendDisplayStatus(bool)));

    _menuVisible = false;
}

DisplayEmulator::~DisplayEmulator() {
    delete _syncTimer;
    delete _socketNotifier;
    close(_socketFd);
}

void DisplayEmulator::_socketSend(can_frame *frame) {
    //qDebug("DisplayEmulator: _socketSend: id=%x, data=%02X%02X%02X%02X%02X%02X%02X%02X", frame->can_id, frame->data[0], frame->data[1], frame->data[2], frame->data[3], frame->data[4], frame->data[5], frame->data[6], frame->data[7]);
    write(_socketFd, frame, sizeof(struct can_frame));
}

void DisplayEmulator::_packetSendReply(can_frame *frame, bool last) {
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

    _socketSend(frame);
}

void DisplayEmulator::_packetSendDisplayStatus(bool enabled) {
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

    _socketSend(&frame);
}

void DisplayEmulator::_packetRecvSync(can_frame * frame) {
    if (frame->data[0] == 0x79) { /* PING od radia, odsyłamy PONG */
        frame->can_id = CAN_ID_SYNC_DISPLAY;
        frame->data[0] = 0x69;
        frame->data[1] = 0x00;
        frame->data[2] = DISPLAY_FILL_BYTE;
        frame->data[3] = DISPLAY_FILL_BYTE;
        frame->data[4] = DISPLAY_FILL_BYTE;
        frame->data[5] = DISPLAY_FILL_BYTE;
        frame->data[6] = DISPLAY_FILL_BYTE;
        frame->data[7] = DISPLAY_FILL_BYTE;
        _socketSend(frame);
        _syncTimer->start(); /* Resetujemy timeout */

        if (_syncState == displaySyncPending) {
            _registerCanId(CAN_ID_DISPLAY_STATUS);
            _registerCanId(CAN_ID_KEY_PRESSED);
            _syncState = displaySyncOK;
        }
    }
    else if (frame->data[0] == 0x7A) { /* Początek synchronizacji */
        frame->can_id = CAN_ID_SYNC_DISPLAY;
        frame->data[0] = 0x61;
        frame->data[1] = 0x11;
        frame->data[2] = 0x00;
        frame->data[3] = DISPLAY_FILL_BYTE;
        frame->data[4] = DISPLAY_FILL_BYTE;
        frame->data[5] = DISPLAY_FILL_BYTE;
        frame->data[6] = DISPLAY_FILL_BYTE;
        frame->data[7] = DISPLAY_FILL_BYTE;
        _socketSend(frame);
        _syncState = displaySyncPending;
    }
}

void DisplayEmulator::_packetRecvDisplayControl(can_frame *frame) {
    bool enabled;

    if (frame->data[0] != 0x70) { /* 0x70 = Rejestracja funkcji */
        enabled = (frame->data[2] == 0x02) ? true : false;
        emit displayStatusChanged(enabled);
        //qDebug() << "DisplayEmulator: statusChanged to" << enabled;
    }

    _packetSendReply(frame);
}

void DisplayEmulator::_packetRecvSetText(can_frame *frame) {
    static char buf[32];
    static int bufpos = 0;
    static int iconsmask, mode, chan, location;
    static QString longText;
    int ptr, max, idx;
    bool selected, fullscreen;
    char c;
    QString text;

    if (frame->data[0] == 0x70) { /* Rejestracja funkcji */
        _packetSendReply(frame);
    }
    else if (frame->data[0] == 0x04) { /* Ustaw ikony */
        /* TODO!!! */
        qDebug() << "DisplayEmulator: TODO: Set icons";
        //qDebug() << frame->data[3];
        //iconsmask = frame->data[3];
        //mode = frame->data[5];
        //emit displayIconsChanged(iconsmask);
        _packetSendReply(frame);
    }
    else if (frame->data[0] == 0x10) { /* Ustaw tekst */
        bufpos = 0;
        buf[0] = '\0';

        if (frame->data[1] == 0x1C) { /* Teskt + Ikony */
            iconsmask = frame->data[3];
            mode = frame->data[5];
            ptr = 6;
            emit displayIconsChanged(iconsmask);
        }
        else if (frame->data[1] == 0x19) { /* Tylko tekst */
            ptr = 3;
        }

        chan = frame->data[ptr++] & 7;
        location = frame->data[ptr++];

        for( ; ptr < 8; ptr++) {
            buf[bufpos++] = frame->data[ptr];
        }

        _packetSendReply(frame, false);
    }
    else if(frame->data[0] > 0x20) { /* Ciąg dalszy poprzednich danych */
        ptr = 1;
        while(ptr < 8) {
            buf[bufpos] = frame->data[ptr++];
            if (!buf[bufpos])
                break;

            bufpos++;
        }

        if (ptr < 8) { /* Koniec danych */
            _packetSendReply(frame, true);

            /* Obrabiamy dane */
            for(ptr = 0; ptr < 12; ptr++) {
                c = char(buf[9 + ptr] & 0x7F);
                switch(c) {
                    case 0x07: text += QString::fromUtf8("⇧"); break;
                    case 0x08: text += QString::fromUtf8("⇩"); break;
                    case 0x09: text += QString::fromUtf8("⇨"); break;
                    case 0x0A: text += QString::fromUtf8("⇦"); break;
                    default: text += c;
                }
            }

            //qDebug() << " loc =" << location << " buf = " << buf << "text =" << text;

            max = (location >> 5) & 7;
            idx = (location >> 2) & 7;
            selected = ((location & 0x01) == 0x01);
            fullscreen = ((location & 0x02) == 0x02);

            if ((max > 0) && (fullscreen) && (!selected)) { /* Tekst migacjący (np. NEWS -> RMF FM -> NEWS...) */

            }
            else {

            }

            if ((max > 0) && (!fullscreen)) { /* Tryb menu */
                if (!_menuVisible) {
                    emit displayMenuShow(max + 1);
                    _menuVisible = true;
                }

                emit displayMenuItemUpdate(idx, text, selected);
                return;
            }
            else { /* Menu niewidoczne */
                if (_menuVisible) {
                    emit displayMenuHide();
                    _menuVisible = false;
                }
            }

            if ((max == 0) && (selected)) { /* Zwykły tekst */
                emit displayTextChanged(text);
            }
            else if ((fullscreen) && (selected)) { /* Tryb pełnoekranowy */
                if (idx == 0) {
                    longText.clear();
                }

                longText += text.left(8);

                if (idx == max) {
                    emit displayTextChanged(longText);
                }
            }
        }
        else {
            _packetSendReply(frame, false);
        }
    }
}

void DisplayEmulator::_registerCanId(int id) {
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

    _socketSend(&frame);
}

void DisplayEmulator::_socketRecv(int fd) {
    struct can_frame frame;
    int len;

    if (fd != _socketFd) { /* Cos jest nie tak */
        return;
    }

    len = read(fd, &frame, sizeof(struct can_frame));
    if (len <= 0) /* Błąd odczytu */
        return;

    //qDebug("DisplayEmulator: _socketRecv: id=%x, data=%02X%02X%02X%02X%02X%02X%02X%02X", frame.can_id, frame.data[0], frame.data[1], frame.data[2], frame.data[3], frame.data[4], frame.data[5], frame.data[6], frame.data[7]);

    switch(frame.can_id) {
        case CAN_ID_SYNC_HU: _packetRecvSync(&frame); break;
        case CAN_ID_DISPLAY_CONTROL: _packetRecvDisplayControl(&frame); break;
        case CAN_ID_SET_TEXT: _packetRecvSetText(&frame); break;
        default: _packetSendReply(&frame);
    }
}

void DisplayEmulator::_syncTimeout() {
    qDebug() << "DisplayEmulator: Timeout!";
    _syncState = displaySyncLost;
    /* Reset CANa */
    QProcess::execute("ifdown "DISPLAY_CAN_INTERFACE);
    QProcess::startDetached("ifup "DISPLAY_CAN_INTERFACE);
}

DisplayEmulator *DisplayEmulator::getInstance() {
    if (!_instance) {
        _instance = new DisplayEmulator();
        _instance->moveToThread(&_workerThread);
        _workerThread.start(QThread::NormalPriority);
    }

    return _instance;
}


void DisplayEmulator::radioPowerChanged(bool enabled) {
    //qDebug() << "DisplayEmulator: Radio power =" << enabled;

    _socketNotifier->setEnabled(enabled);

    if (!enabled) {
        _syncTimer->stop();
        _syncState = displaySyncLost;
        QProcess::execute("ifdown "DISPLAY_CAN_INTERFACE);
        emit displayTextChanged("");
        emit displayIconsChanged(0xFF);
    }
    else {
        //emit displayTextChanged("Radio ON");
        QProcess::startDetached("ifup "DISPLAY_CAN_INTERFACE);
    }

}

void DisplayEmulator::sendKeyEvent(int keycode) {
    struct can_frame frame;

    frame.can_id = CAN_ID_KEY_PRESSED;
    frame.can_dlc = 8;
    frame.data[0] = 0x03;
    frame.data[1] = 0x89;
    frame.data[2] = (keycode >> 8);
    frame.data[3] = (keycode & 0xFF);
    frame.data[4] = DISPLAY_FILL_BYTE;
    frame.data[5] = DISPLAY_FILL_BYTE;
    frame.data[6] = DISPLAY_FILL_BYTE;
    frame.data[7] = DISPLAY_FILL_BYTE;

    _socketSend(&frame);
}
