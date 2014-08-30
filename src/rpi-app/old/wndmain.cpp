#include "wndmain.h"
#include "ui_wndmain.h"
#include <QTime>
#include <QKeyEvent>
#include <QPixmap>
#include "carpi.h"

WndMain::WndMain(QWidget *parent) : QMainWindow(parent), _ui(new Ui::WndMain) {
    QAction * action;

    _ui->setupUi(this);


    _ui->lRadioText->clear();
    _ui->lRadioIcons->clear();
    _ui->lBluetoothState->clear();
    _ui->lSourceIcon->clear();
    _ui->lTemp->clear();
    _ui->lTime->clear();
    setBluetoothIcon(false);

    _timerScroll = new QTimer(this);
    _timerScroll->setSingleShot(false);
    connect(_timerScroll, SIGNAL(timeout()), this, SLOT(_textScroll()));

    _timerTimeUpdate = new QTimer(this);
    _timerTimeUpdate->setInterval(5000);
    _timerTimeUpdate->setSingleShot(false);
    connect(_timerTimeUpdate, SIGNAL(timeout()), this, SLOT(_timeUpdate()));
    _timerTimeUpdate->start();

    _timeUpdate();

    for(int i = 0; i < 2; i++) {
        for(int j = 0; j < 3; j++) {
            _dials[i * 3 + j] = new CarpiDial(_ui->pageDials);
            _dials[i * 3 + j]->setGeometry(2 + j * 159, i * 119, 155, 115);
        }
    }


    _dials[dialIntakeTemp]->setDialName("Intake Air Temp");
    _dials[dialIntakeTemp]->setDialUnit(QString::fromUtf8("°C"));
    _dials[dialIntakeTemp]->setMin(-30);
    _dials[dialIntakeTemp]->setMax(170);
    _dials[dialIntakeTemp]->setScaleTick(20);

    _dials[dialBoost]->setDialName("Boost");
    _dials[dialBoost]->setDialUnit("PSI");
    _dials[dialBoost]->setMin(-25);
    _dials[dialBoost]->setMax(25);
    _dials[dialBoost]->setScaleTick(5);
    _dials[dialBoost]->setDrawFromZero(true);
    _dials[dialBoost]->setValuePrecision(1);

    _dials[dialFuelPressure]->setDialName("Fuel Pressure");
    _dials[dialFuelPressure]->setDialUnit("MPa");
    _dials[dialFuelPressure]->setMin(0);
    _dials[dialFuelPressure]->setMax(150);
    _dials[dialFuelPressure]->setScaleTick(15);

    _dials[dialCoolantTemp]->setDialName("Coolant Temp");
    _dials[dialCoolantTemp]->setDialUnit(QString::fromUtf8("°C"));
    _dials[dialCoolantTemp]->setMin(-30);
    _dials[dialCoolantTemp]->setMax(170);
    _dials[dialCoolantTemp]->setScaleTick(20);
    _dials[dialCoolantTemp]->setCriticalRange(130, 170);
    _dials[dialCoolantTemp]->setWarningRange(110, 130);

    _dials[dialEngineLoad]->setDialName("Engine Load");
    _dials[dialEngineLoad]->setDialUnit("%");
    _dials[dialEngineLoad]->setMin(0);
    _dials[dialEngineLoad]->setMax(100);
    _dials[dialEngineLoad]->setScaleTick(10);
    _dials[dialEngineLoad]->setCriticalRange(90, 100);

    _dials[dialVoltage]->setDialName("Voltage");
    _dials[dialVoltage]->setDialUnit("V");
    _dials[dialVoltage]->setMin(8);
    _dials[dialVoltage]->setMax(18);
    _dials[dialVoltage]->setScaleTick(1);
    _dials[dialVoltage]->setCriticalRange(8, 10);
    _dials[dialVoltage]->setWarningRange(10, 11);
    _dials[dialVoltage]->setCriticalRange(16, 18);
    _dials[dialVoltage]->setValuePrecision(1);

    _navitMenuEnabled = false;

    _mainMenu = new QMenu(this);
    //connect(_mainMenu, SIGNAL(aboutToHide()), this, SLOT(_menuHide()));

    action = _mainMenu->addAction(QString::fromUtf8("Menu nawigacji"));
    connect(action, SIGNAL(triggered(bool)), this, SLOT(_showNavitMenu()));

    _viewMenu = _mainMenu->addMenu(QString::fromUtf8("Widok"));
    _viewMenuActions = new QActionGroup(_viewMenu);

    _systemMenu = _mainMenu->addMenu("System");

    _mainMenu->addSeparator();

    action = _mainMenu->addAction(QString::fromUtf8("Powrót"));
    connect(action, SIGNAL(triggered(bool)), this, SLOT(_menuHide()));

    _showMapAction = _viewMenu->addAction("Mapa");
    _showMapAction->setCheckable(true);
    _showMapAction->setChecked(true);
    connect(_showMapAction, SIGNAL(triggered(bool)), this, SLOT(_setViewNavit()));
    _viewMenuActions->addAction(_showMapAction);

    _showDashboardAction = _viewMenu->addAction(QString::fromUtf8("Wskaźniki"));
    _showDashboardAction->setCheckable(true);
    connect(_showDashboardAction, SIGNAL(triggered(bool)), this, SLOT(_setViewDashboard()));
    _viewMenuActions->addAction(_showDashboardAction);

    action = _systemMenu->addAction(QString::fromUtf8("Wyłącz"));
    connect(action, SIGNAL(triggered(bool)), this, SLOT(_systemShutdown()));

    _timerNavitMenuDetect = new QTimer(this);
    _timerNavitMenuDetect->setSingleShot(false);
    _timerNavitMenuDetect->setInterval(800);
    connect(_timerNavitMenuDetect, SIGNAL(timeout()), this, SLOT(_navitMenuDetectTick()));
}

WndMain::~WndMain() {
    for(int i = 0; i < 6; i++)
        delete _dials[i];

    delete _timerTimeUpdate;
    delete _timerScroll;
    delete _ui;
}

QWidget *WndMain::getNavitWidget() {
    return _ui->pageNavit;
}

void WndMain::setRadioText(QString text) {
    if (text == _radioText) /* Nic do roboty */
        return;

    _timerScroll->stop();
    _radioText = text;
    _radioTextPos = 0;

    if (text.isEmpty()) { /* Radio wyłączone */
        _ui->lRadioIcons->clear();
        _ui->lRadioText->clear();
        _ui->lSourceIcon->clear();
    }
    else {
        _ui->lRadioText->setText(_radioText.left(RADIO_TEXT_MAX));

        if (text.length() > RADIO_TEXT_MAX) { /* Musimy przewijać */
            _timerScroll->setInterval(600);
            _timerScroll->start();
        }
    }
}

void WndMain::setRadioIcons(bool news, bool traffic, bool afrds) {
    QString icons;

    if (news)
        icons += "NEWS\n";

    if (afrds)
        icons += "AF-RDS";

    if (traffic)
        icons += " TRAFFIC";

    _ui->lRadioIcons->setText(icons);
}

void WndMain::setSource(CarPISource source) {
    static QPixmap sources_icons[] = {
        QPixmap(":/resources/icons/source_fm.png"),
        QPixmap(":/resources/icons/source_cd.png"),
        QPixmap(":/resources/icons/source_usb.png"),
        QPixmap(":/resources/icons/source_bluetooth.png"),
    };

    _ui->lSourceIcon->setPixmap(sources_icons[source]);
}

void WndMain::setBluetoothIcon(bool isConnected) {
    static QPixmap bluetooth_icons[] = {
        QPixmap(":/resources/icons/bluetooth_inactive.png"),
        QPixmap(":/resources/icons/bluetooth_active.png")
    };

    _ui->lBluetoothState->setPixmap(bluetooth_icons[isConnected ? 1 : 0]);
}

void WndMain::dialValueChanged(CarPIDialsID dialID, double value) {
    //qDebug() << "dialValueChanged: value:" << value;
    _dials[dialID]->setValue(value);
}

void WndMain::changeDisplayMode() {
    _ui->stackedWidget->setCurrentIndex((_ui->stackedWidget->currentIndex() + 1) % 2);
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

void WndMain::_textScroll() {
    _radioTextPos++;
    if (_radioTextPos >= _radioText.length() - 3) {
        _radioTextPos = 0;
        _timerScroll->stop();
    }
    _timerScroll->setInterval(200);
    _ui->lRadioText->setText(_radioText.mid(_radioTextPos, RADIO_TEXT_MAX));
}

void WndMain::_timeUpdate() {
    _ui->lTime->setText(QTime::currentTime().toString("HH:mm"));
}

void WndMain::_menuHide() {
    if (!_navitMenuEnabled)
        emit mainMenuHide();
}

void WndMain::_setViewNavit() {
    _menuHide();
    _ui->stackedWidget->setCurrentIndex(0);
    Navit::getInstance()->show();
}

void WndMain::_setViewDashboard() {
    _menuHide();
    _ui->stackedWidget->setCurrentIndex(1);
    activateWindow();
}

void WndMain::_showNavitMenu() {
    _navitMenuEnabled = true;
    _setViewNavit();
    _showMapAction->setChecked(true);
    Navit::getInstance()->sendKey(navitReturn);    
    _timerNavitMenuDetect->start();
}

void WndMain::_systemShutdown() {
    qDebug() << "WndMain: shutdown system";
    _menuHide();
    emit shutdown();
}

void WndMain::_navitMenuDetectTick() {
    QPixmap navitScreen;
    QImage img;
    QRgb pixel;
  //  qDebug() << "WndMain: navitMenuDetect tick";

    navitScreen = QPixmap::grabWindow(Navit::getInstance()->getWindowId(), 470, 2, -1, 1);
    img = navitScreen.toImage();

    pixel = img.pixel(img.width() - 1, 0);
//    qDebug("Pixel=%x", pixel);
    if (pixel != 0xFF424142) { /* Wyjście z menu */
        _navitMenuEnabled = false;
        _menuHide();
        _timerNavitMenuDetect->stop();
    }
}


void WndMain::menuShow() {
    if (!_mainMenu->isVisible()) {
        _mainMenu->popup(QPoint(20, 60));
    }
}

void WndMain::menuKeyPressed(CarPIKey key) {
    QKeyEvent * event = NULL;
    Navit * navit;
    QAction * action;

    if (_navitMenuEnabled) {
        navit = Navit::getInstance();
        switch(key) {
            case keyUp: navit->sendKey(navitUp); break;
            case keyRight: navit->sendKey(navitRight); break;
            case keyDown: navit->sendKey(navitDown); break;
            case keyLeft: navit->sendKey(navitLeft); break;
            case keyReturn: navit->sendKey(navitReturn); break;
            case keyEscape: navit->sendKey(navitEscape); break;
        }
    }
    else {
        switch (key) {
            case keyUp: event = new QKeyEvent(QEvent::KeyPress, Qt::Key_Up, Qt::NoModifier); break;
            case keyRight: event = new QKeyEvent(QEvent::KeyPress, Qt::Key_Right, Qt::NoModifier); break;
            case keyDown: event = new QKeyEvent(QEvent::KeyPress, Qt::Key_Down, Qt::NoModifier); break;
            case keyLeft: event = new QKeyEvent(QEvent::KeyPress, Qt::Key_Left, Qt::NoModifier); break;
            case keyReturn: event = new QKeyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier); break;
            //case keyEscape: event = new QKeyEvent(QEvent::KeyPress, Qt::Key_Escape, Qt::NoModifier); break;
        }

        if (event) {
            action = _mainMenu->activeAction();
            if ((action) && (action->menu()) && (action->menu()->isVisible())) {
                QCoreApplication::postEvent(action->menu(), event);
            }
            else {
                QCoreApplication::postEvent(_mainMenu, event);
            }
        }
    }
}
