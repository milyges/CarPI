#include "wndvolume.h"
#include "ui_wndvolume.h"

WndVolume::WndVolume(QWidget *parent) : QDialog(parent), _ui(new Ui::WndVolume) {
    _ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

    _hideTimer = new QTimer(this);
    _hideTimer->setSingleShot(false);
    _hideTimer->setInterval(3000);
    connect(_hideTimer, SIGNAL(timeout()), this, SLOT(hide()));
}

WndVolume::~WndVolume() {
    delete _hideTimer;
    delete _ui;
}

void WndVolume::showVolume(int volume) {
    _ui->pbVolume->setValue(volume);

    if (!isVisible()) {
        show();
    }

    _hideTimer->start();
}

void WndVolume::changeEvent(QEvent *e) {
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        _ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
