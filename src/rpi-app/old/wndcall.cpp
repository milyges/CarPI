#include "wndcall.h"
#include "ui_wndcall.h"

WndCall::WndCall(QWidget *parent) : QDialog(parent), _ui(new Ui::WndCall) {
    _ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

    _callTimer = new QTimer(this);
    _callTimer->setSingleShot(false);
    _callTimer->setInterval(1000);
    connect(_callTimer, SIGNAL(timeout()), this, SLOT(_callTimerTick()));
}

WndCall::~WndCall() {
    delete _callTimer;
    delete _ui;
}

void WndCall::callStateChanged(BluetoothCallState state) {
    if (state == callStateIdle) {
        _ui->lModeText->setText(QString::fromUtf8("Rozłączono"));

        if (isVisible())
            hide();

        if (_callTimer->isActive())
            _callTimer->stop();
    }
    else {
        if (!isVisible()) {
            show();
            move(QPoint(480 / 2 - width() / 2, 36));
        }
    }

    switch(state) {
        case callStateTalking: {
            _ui->lModeText->setText(QString::fromUtf8("Rozmowa"));
            _ui->lCallDuration->setVisible(true);
            _ui->lStartIcon->setVisible(false);
            _ui->lStartText->setVisible(false);
            _ui->lStopText->setText("Mute");

            if (!_callTimer->isActive()) {
                _callDuration = QTime(0, 0);
                _ui->lCallDuration->setText(_callDuration.toString("mm:ss"));
                _callTimer->start();
            }
            break;
        }
        case callStateIncoming: {
            _ui->lModeText->setText(QString::fromUtf8("Połączenie przychodzące"));
            _ui->lCallDuration->setVisible(false);
            _ui->lStartIcon->setVisible(true);
            _ui->lStartText->setVisible(true);
            _ui->lStopText->setText(QString::fromUtf8("Vol-"));
            _ui->lStartText->setText(QString::fromUtf8("Vol+"));
            break;
        }
        case callStateOutgoing: {
            _ui->lModeText->setText(QString::fromUtf8("Wybieranie numeru"));
            _ui->lCallDuration->setVisible(false);
            _ui->lStartIcon->setVisible(false);
            _ui->lStartText->setVisible(false);
            _ui->lStopText->setText(QString::fromUtf8("Mute"));
            break;
        }
    }
}

void WndCall::changeEvent(QEvent *e) {
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        _ui->retranslateUi(this);
        break;
    default:
        break;
    }
}


void WndCall::_callTimerTick() {
    _callDuration = _callDuration.addSecs(1);
    _ui->lCallDuration->setText(_callDuration.toString("mm:ss"));
}
