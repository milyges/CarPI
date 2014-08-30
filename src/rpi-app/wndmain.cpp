#include "wndmain.h"
#include "ui_wndmain.h"
#include <QTime>
#include <QDebug>

WndMain::WndMain(CarPI *parent) : QMainWindow(), _ui(new Ui::WndMain) {
    _carpi = parent;
    _currentModule = NULL;
    _radioModule = NULL;
    _ui->setupUi(this);

    _updateTimeTimer = new QTimer();
    _updateTimeTimer->setInterval(10000);
    _updateTimeTimer->setSingleShot(false);
    connect(_updateTimeTimer, SIGNAL(timeout()), this, SLOT(_updateTime()));
    _updateTime();
    _updateTimeTimer->start();


    _ui->lbRadioText->setMaxLength(20);

    connect(_carpi, SIGNAL(radioTextChanged(QString)), this, SLOT(_radioTextChanged(QString)));
    connect(_carpi, SIGNAL(radioIconsChanged(bool,bool,bool)), this, SLOT(_radioIconsChanged(bool,bool,bool)));
    connect(_carpi, SIGNAL(sourceChanged(CarPISource)), this, SLOT(_sourceChanged(CarPISource)));
    connect(_carpi, SIGNAL(bluetoothConnectionStateChanged(bool)), this, SLOT(_bluetoothStateChanged(bool)));
    connect(_carpi, SIGNAL(pilotKeyPressed(CarPIKey)), this, SLOT(_keyPressed(CarPIKey)));

    _wndVolume = new WndVolume(this);

    connect(_carpi, SIGNAL(volumeChanged(int)), _wndVolume, SLOT(showVolume(int)));

    _currentModule = NULL;

    _menuModule = new MenuModule();

    MenuItem * item;
    item = _menuModule->addItem(QString::fromUtf8("Nawigacja"), QPixmap());
    connect(item, SIGNAL(triggered()), this, SLOT(_navigationScreen()));
    item = _menuModule->addItem(QString::fromUtf8("Radio"), QPixmap());
    connect(item, SIGNAL(triggered()), this, SLOT(_radioScreen()));
    item = _menuModule->addItem(QString::fromUtf8("Wskaźniki"), QPixmap());
    connect(item, SIGNAL(triggered()), this, SLOT(_dashboardScreen()));
    item = _menuModule->addItem(QString::fromUtf8("Telefon"), QPixmap());
    item = _menuModule->addItem(QString::fromUtf8("Ustawienia"), QPixmap());

    _ui->mainWidget->addWidget(_menuModule);

    _navigationModule = new NavigationModule();
    _ui->mainWidget->addWidget(_navigationModule);

    _dashboardModule = new DashboardModule();
    connect(_carpi, SIGNAL(dialValueChanged(CarPIDialsID,double)), _dashboardModule, SLOT(dialValueChanged(CarPIDialsID,double)));
    _ui->mainWidget->addWidget(_dashboardModule);

    _radioModule = new RadioModule();
    _ui->mainWidget->addWidget(_radioModule);

    _setModule(_radioModule);
    //_setModule(_navigationModule);

    _radioTextChanged("");
    _bluetoothStateChanged(false);
    _sourceChanged(_carpi->currentSource());
}

WndMain::~WndMain() {
    delete _ui;
    delete _wndVolume;
    delete _menuModule;
    delete _navigationModule;
}

void WndMain::start() {

    showFullScreen();
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

void WndMain::_setModule(GuiModule *mod) {
    if (_currentModule) {
        _currentModule->deactivate();
        if (_currentModule->useRadioText()) {
            _ui->lbRadioText->setVisible(true);
            _ui->lbSourceIcon->setVisible(true);
        }
    }

    _currentModule = mod;
    if (_currentModule->useRadioText()) {
        _ui->lbRadioText->setVisible(false);
        _ui->lbSourceIcon->setVisible(false);
    }
    mod->activate();
    _ui->mainWidget->setCurrentWidget(mod);
}

void WndMain::_radioTextChanged(QString text) {
    if ((_currentModule) && (_currentModule->useRadioText())) {
        _currentModule->radioTextChanged(text);
    }

    if (_radioModule) {
        _radioModule->radioTextChanged(text);
    }

    if (text.isEmpty()) {
        _ui->lbSourceIcon->clear();
        _ui->lbRadioText->clear();
        _ui->lbSourceIcon->clear();
    }
    else {
        _ui->lbRadioText->setText(text);
    }
}

void WndMain::_radioIconsChanged(bool news, bool traffic, bool afrds) {
    if ((_currentModule) && (_currentModule->useRadioText())) {
        _currentModule->radioIconsChanged(news, traffic, afrds);
    }

    if (_radioModule) {
        _radioModule->radioIconsChanged(news, traffic, afrds);
    }
}

void WndMain::_bluetoothStateChanged(bool connected) {
    static QPixmap bluetooth_icons[] = {
        QPixmap(":/resources/icons/bluetooth_inactive.png"),
        QPixmap(":/resources/icons/bluetooth_active.png")
    };

    _ui->lbBluetoothIcon->setPixmap(bluetooth_icons[connected ? 1 : 0]);
}

void WndMain::_sourceChanged(CarPISource source) {
    static QPixmap sources_icons[] = {
        QPixmap(":/resources/icons/source_fm.png"),
        QPixmap(":/resources/icons/source_cd.png"),
        QPixmap(":/resources/icons/source_usb.png"),
        QPixmap(":/resources/icons/source_bluetooth.png"),
        QPixmap()
    };

    _ui->lbSourceIcon->setPixmap(sources_icons[source]);
    if (_radioModule) {
        _radioModule->radioSourceChanged(source);
    }
}

void WndMain::_keyPressed(CarPIKey key) {
    if ((_currentModule) && (_currentModule->useKey(key))) {
        _currentModule->keyPressed(key);
    }
    else {
        if (key == keyLoad) {
            _setModule(_menuModule);
        }
        else {
            _carpi->radioSendKey(key);
        }
    }
}

void WndMain::_updateTime() {
    _ui->lbTime->setText(QTime::currentTime().toString("HH:mm"));
}

void WndMain::_navigationScreen() {
    _setModule(_navigationModule);
}

void WndMain::_dashboardScreen() {
    _setModule(_dashboardModule);
}

void WndMain::_radioScreen() {
    _setModule(_radioModule);
}
