#include "wndcall.h"
#include "ui_wndcall.h"
#include <QPoint>

WndCall::WndCall(QWidget *parent) : QDialog(parent), ui(new Ui::WndCall) {
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    _status = callStatusIdle;

    _call_timer = new QTimer(this);
    _call_timer->setInterval(1000);
    _call_timer->setSingleShot(false);
    connect(_call_timer, SIGNAL(timeout()), this, SLOT(_call_timer_tick()));
}

WndCall::~WndCall() {
    delete _call_timer;
    delete ui;
}

void WndCall::update_status(int newstatus) {
    if (newstatus == _status) /* Nic do roboty */
        return;

    _status = newstatus;

    if (newstatus == callStatusIdle) {
        ui->lModeText->setText(QString::fromUtf8("Rozłączono"));
        if (isVisible())
            hide();

        if (_call_timer->isActive())
            _call_timer->stop();
    }
    else {
        if (!isVisible()) {
            show();
            move(QPoint(480 / 2 - width() / 2, 36));
        }
    }

    switch (newstatus) {
        case callStatusTalking: {
            ui->lModeText->setText(QString::fromUtf8("Rozmowa"));
            ui->lCallDuration->setVisible(true);
            ui->lStartIcon->setVisible(false);
            ui->lStartText->setVisible(false);
            ui->lStopText->setText(QString::fromUtf8("Mute"));

            if (!_call_timer->isActive()) {
                _call_duration = QTime(0, 0);
                ui->lCallDuration->setText(_call_duration.toString("mm:ss"));
                _call_timer->start();
            }
            break;
        }
        case callStatusIncomimg: {
            ui->lModeText->setText(QString::fromUtf8("Połączenie przychodzące"));
            ui->lCallDuration->setVisible(false);
            ui->lStartIcon->setVisible(true);
            ui->lStartText->setVisible(true);
            ui->lStopText->setText(QString::fromUtf8("Vol-"));
            ui->lStartText->setText(QString::fromUtf8("Vol+"));
            break;
        }
        case callStatusOutgoing: {
            ui->lModeText->setText(QString::fromUtf8("Wybieranie numeru"));
            ui->lCallDuration->setVisible(false);
            ui->lStartIcon->setVisible(false);
            ui->lStartText->setVisible(false);
            ui->lStopText->setText(QString::fromUtf8("Mute"));
            break;
        }
    }
}

void WndCall::changeEvent(QEvent *e) {
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}


void WndCall::_call_timer_tick() {
    _call_duration = _call_duration.addSecs(1);
    ui->lCallDuration->setText(_call_duration.toString("mm:ss"));
}
