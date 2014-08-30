#include "scrolledlabel.h"

ScrolledLabel::ScrolledLabel(QWidget *parent) : QLabel(parent) {
    _timer = new QTimer(this);
    connect(_timer, SIGNAL(timeout()), this, SLOT(_timerTick()));

    _timer->setSingleShot(false);
}

ScrolledLabel::~ScrolledLabel() {
    delete _timer;
}

void ScrolledLabel::setMaxLength(int len) {
    if (len > 0) {
        _maxLength = len;
    }
}


int ScrolledLabel::maxLength() {
    return _maxLength;
}

void ScrolledLabel::setText(const QString &text) {
    if (_text == text)
        return;

    _timer->stop();
    _text = text;
    _pos = 0;
    QLabel::setText(text.left(_maxLength));

    if (_text.length() > _maxLength) {
        _timer->setInterval(600);
        _timer->start();
    }
}


void ScrolledLabel::_timerTick() {
    _pos++;

    if (_pos >= _text.length() - (_maxLength / 3)) {
        _pos = 0;
        _timer->stop();
    }

    _timer->setInterval(200);
    QLabel::setText(_text.mid(_pos, _maxLength));
}
