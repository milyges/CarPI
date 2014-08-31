#include "wndcallinfo.h"
#include "ui_wndcallinfo.h"

WndCallInfo::WndCallInfo(QWidget *parent) : QDialog(parent), _ui(new Ui::WndCallInfo) {
    _ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

    _showPoint = QPoint(parent->width() / 2 - width() / 2, 32);

    _hideTimer = new QTimer(this);
    _hideTimer->setInterval(2000);
    _hideTimer->setSingleShot(true);
    connect(_hideTimer, SIGNAL(timeout()), this, SLOT(hide()));

    _durationTimer = new QTimer(this);
    _durationTimer->setInterval(1000);
    _durationTimer->setSingleShot(false);
    connect(_durationTimer, SIGNAL(timeout()), this, SLOT(_durationTimerTick()));
}

WndCallInfo::~WndCallInfo() {
    delete _ui;
}

void WndCallInfo::setPhoneNumber(QString number) {
    _ui->lbPhoneNumber->setText(number);
}

void WndCallInfo::incomingCall() {
    _showWindow();
    _ui->lbCallState->setText(QString::fromUtf8("Połączenie przychodzące"));
    _ui->lbPhoneNumber->clear();
    _ui->lbCallTime->clear();
    _ui->pbAccept->show();
    _ui->pbReject->show();
    _ui->pbReject->setText("Volume -");
    _durationTimer->stop();
}

void WndCallInfo::outgoingCall() {
    _showWindow();
    _ui->lbCallState->setText(QString::fromUtf8("Wybieranie numeru"));
    _ui->lbPhoneNumber->clear();
    _ui->lbCallTime->clear();
    _ui->pbAccept->hide();
    _ui->pbReject->show();
    _ui->pbReject->setText("Mute");
    _durationTimer->stop();
}

void WndCallInfo::talking() {
    _showWindow();
    _ui->lbCallState->setText(QString::fromUtf8("Rozmowa"));
    _ui->pbAccept->hide();
    _ui->pbReject->show();
    _ui->pbReject->setText("Mute");
    _callDuration = 0;
    _durationTimerTick();
    _durationTimer->start();
}

void WndCallInfo::idle() {
    _ui->lbCallState->setText(QString::fromUtf8("Połączenie zakończone"));
    _durationTimer->stop();
    _hideTimer->start();
}

void WndCallInfo::_durationTimerTick() {
    int m = _callDuration / 60;
    int s = _callDuration % 60;
    _ui->lbCallTime->setText(QString("%1:%2").arg(m, 2, 10, QChar('0')).arg(s, 2, 10, QChar('0')));
    _callDuration++;
}

void WndCallInfo::_showWindow() {
    if (!isVisible()) {
        show();
        move(_showPoint);
    }
}
