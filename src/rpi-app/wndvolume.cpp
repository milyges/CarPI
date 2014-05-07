#include "wndvolume.h"
#include "ui_wndvolume.h"

WndVolume::WndVolume(QWidget *parent) : QDialog(parent), ui(new Ui::WndVolume) {
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

    _hide_timer = new QTimer(this);
    _hide_timer->setInterval(3000);
    _hide_timer->setSingleShot(true);
    connect(_hide_timer, SIGNAL(timeout()), this, SLOT(hide()));
}

WndVolume::~WndVolume() {
    delete ui;
}

void WndVolume::show_volume(int vol) {
    ui->pbVolume->setValue(vol);
    _hide_timer->stop();
    _hide_timer->start();

    if (!isVisible()) {        
        show();
        move(QPoint(480 / 2 - width() / 2, 150 - height() / 2));
    }
}

void WndVolume::changeEvent(QEvent *e) {
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
