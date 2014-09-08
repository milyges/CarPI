#include <QDebug>
#include <QX11Info>
#include <QApplication>
#include <QDesktopWidget>
#include "navit.h"
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>

Navit * Navit::_instance = NULL;

Navit::Navit(QObject *parent) : QObject(parent) {
    _proc = new QProcess(this);
    _proc->setWorkingDirectory("/root/.navit");
    connect(_proc, SIGNAL(finished(int)), this, SLOT(_navitFinished(int)));

    _searchTimer = new QTimer(this);
    _searchTimer->setSingleShot(false);
    _searchTimer->setInterval(200);
    connect(_searchTimer, SIGNAL(timeout()), this, SLOT(_searchWindow()));

    _isRunning = false;
    _isVisible = false;
    _geometry = QRect(100, 100, 480, 320);
}

Navit::~Navit() {
    stop();
    delete _proc;
}

QRect Navit::geometry() {
    return _geometry;
}

void Navit::setGeometry(QRect geom) {
    _geometry = geom;
    move(geom.x(), geom.y());
    resize(geom.width(), geom.height());
}


void Navit::start() {
    if (!_isRunning) {
        _isRunning = true;
        _navitWindows.clear();
        _proc->start("nice -n 10 /usr/local/bin/navit");
        _searchTimer->start();
    }
}

void Navit::stop() {
    if (_isRunning) {
        _isRunning = false;
        _proc->terminate();
        _proc->waitForFinished(1000);
    }
}

void Navit::show() {
    if (XMapWindow(QX11Info::display(), windowID())) {
        _isVisible = true;
        move(_geometry.x(), _geometry.y());
        //resize(_geometry.width(), _geometry.height());
        raise();
    }

}

void Navit::hide() {
    if (XUnmapWindow(QX11Info::display(), windowID())) {
        _isVisible = false;
    }
}

void Navit::raise() {
    XRaiseWindow(QX11Info::display(), windowID());
}

void Navit::move(int x, int y) {
    if (XMoveWindow(QX11Info::display(), windowID(), x, y)) {
        _geometry.setX(x);
        _geometry.setY(y);
    }
}

void Navit::resize(int w, int h) {
    if (XResizeWindow(QX11Info::display(), windowID(), w, h)) {
        _geometry.setWidth(w);
        _geometry.setHeight(h);
    }
}

void Navit::sendKey(NavitKey key) {
    static int key2keycode[] = {
        0,
        XK_Return,
        XK_Escape,
        XK_Up,
        XK_Right,
        XK_Down,
        XK_Left
    };

    _sendKeyEvent(true, key2keycode[key], 0);
    _sendKeyEvent(false, key2keycode[key], 0);
}

void Navit::_searchWindow(Window startWinID) {
    Window rootWindow;
    Window parent;
    Window * ch;
    unsigned int chCount;
    XTextProperty data;
    int j;

    if (!startWinID) {
        startWinID = QX11Info::appRootWindow();
    }

    if (!XQueryTree(QX11Info::display(), startWinID, &rootWindow, &parent, &ch, &chCount)) {
        qDebug() << "XQueryTree: failed!";
        return;
    }

    for(unsigned int i = 0; i < chCount; i++) {
        if (XGetTextProperty(QX11Info::display(), ch[i], &data, XA_WM_CLASS)) {
            if (QString((char *)data.value) == "navit") {
                /* Dodaj okno do listy, jeśli go tam nie ma */
                for(j = 0; j < _navitWindows.count(); j++) {
                    if (_navitWindows.at(j) == ch[i])
                        break;
                }

                if (j >= _navitWindows.count()) {
                    _navitWindows.append(ch[i]);
                }

                if (_navitWindows.count() == _expectWindows) {
                    _searchTimer->stop();

                    /* Ustawiamy widocznosc okna */
                    if (_isVisible)
                        show();
                    else
                        hide();

                    return;
                }
            }
        }

        _searchWindow(ch[i]);
    }

    XFree(ch);
}

void Navit::_navitFinished(int code) {
    _navitWindows.clear();
    if (_isRunning) {
        _isRunning = false;
        start();
    }
}


Window Navit::windowID() {
    Window result = 0;
    for(int i = 0; i < _navitWindows.count(); i++) {
        if (_navitWindows.at(i) > result)
            result = _navitWindows.at(i);
    }

    return result;
}

void Navit::_sendKeyEvent(bool press, int keycode, int mod) {
    XKeyEvent ev;

    ev.display = QX11Info::display();
    ev.window = windowID();
    ev.root = QX11Info::appRootWindow();
    ev.subwindow = None;
    ev.x = 1;
    ev.y = 1;
    ev.x_root = 1;
    ev.y_root = 1;
    ev.same_screen = True;
    ev.keycode = XKeysymToKeycode(QX11Info::display(), keycode);
    ev.state = mod;

    if (press)
        ev.type = KeyPress;
    else
        ev.type = KeyRelease;

    XSendEvent(ev.display, ev.window, True, KeyPressMask, (XEvent *)&ev);
}

Navit *Navit::getInstance() {
    if (!_instance) {
        _instance = new Navit();
    }

    return _instance;
}
