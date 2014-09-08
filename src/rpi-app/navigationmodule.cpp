#include "navigationmodule.h"

NavigationModule::NavigationModule(QWidget *parent) : GuiModule(parent) {
    _navit = Navit::getInstance();

    _inMenuDetectTimer = new QTimer();
    _inMenuDetectTimer->setSingleShot(false);
    _inMenuDetectTimer->setInterval(500);
    connect(_inMenuDetectTimer, SIGNAL(timeout()), this, SLOT(_inMenuDetect()));
}

NavigationModule::~NavigationModule() {
    delete _inMenuDetectTimer;
}

bool NavigationModule::useKey(CarPIKey key) {
    if (_inMenu) {
        return true;
    }
    else {
        return false;
    }
}

void NavigationModule::activate() {
    _navit->setGeometry(QRect(0, 31, 480, 272 - 31));
    _navit->sendKey(navitKeyReturn);
    _navit->show();
    _inMenu = true;
    _inMenuDetectTimer->start(1000);
}

void NavigationModule::deactivate() {
    _navit->hide();
    _inMenu = false;
    _inMenuDetectTimer->stop();
}

void NavigationModule::keyPressed(CarPIKey key) {
    switch(key) {
        case keyVolUp: _navit->sendKey(navitKeyUp); break;
        case keySrcR: _navit->sendKey(navitKeyRight); break;
        case keyVolDown: _navit->sendKey(navitKeyDown); break;
        case keySrcL: _navit->sendKey(navitKeyLeft); break;
        case keyPause: _navit->sendKey(navitKeyReturn); break;
        case keyLoad: _navit->sendKey(navitKeyEscape); break;
        default: { }
    }
}

void NavigationModule::radioTextChanged(QString text) {

}

void NavigationModule::radioIconsChanged(bool news, bool traffic, bool afrds) {

}


void NavigationModule::_inMenuDetect() {
    QPixmap navitScreen;
    QImage img;
    QRgb pixel;

    _inMenuDetectTimer->setInterval(500);
    navitScreen = QPixmap::grabWindow(_navit->windowID(), 470, 2, -1, 1);
    img = navitScreen.toImage();
    pixel = img.pixel(img.width() - 1, 0);
    if (pixel != 0xFF424142) { /* Wyjście z menu */
        _inMenu = false;
        _inMenuDetectTimer->stop();
    }
}


bool NavigationModule::useRadioText() {
    return false;
}
