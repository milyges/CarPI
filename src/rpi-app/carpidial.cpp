#include "carpidial.h"
#include <QDebug>
#include <QPainter>
#include <QLinearGradient>

CarpiDial::CarpiDial(QWidget *parent) : QWidget(parent) {
    _value = 0;
    _max = 0;
    _min = 0;
    _drawFromZero = false;
    _scaleTick = 1;
    _valuePrecision = 0;
}

void CarpiDial::setDialName(QString name) {
    _name = name;
    update();
}

void CarpiDial::setDialUnit(QString unit) {
    _unitName = unit;
    update();
}

void CarpiDial::setMax(double max) {
    _max = max;
    update();
}

void CarpiDial::setMin(double min) {
    _min = min;
    update();
}

void CarpiDial::setValue(double value) {
    if ((value >= _min) && (value <= _max)) {
        _value = value;
        update();
    }
}

void CarpiDial::setValuePrecision(int prec) {
    _valuePrecision = prec;
}

void CarpiDial::setCriticalRange(double start, double stop) {
    struct DialRange range;

    range.start = start;
    range.stop = stop;

    _cricitalRanges.append(range);
}

void CarpiDial::setWarningRange(double start, double stop) {
    struct DialRange range;

    range.start = start;
    range.stop = stop;

    _warningRanges.append(range);
}

void CarpiDial::setDrawFromZero(bool isset) {
    _drawFromZero = isset;
}

void CarpiDial::setScaleTick(double tick) {
    /* TODO: Sprawdź czy jest podzielna przez _max - _min */
    _scaleTick = tick;
}

void CarpiDial::paintEvent(QPaintEvent *ev) {
    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing);

    _drawBackground(&painter);
    _drawScale(&painter);
    _drawLabels(&painter);
}

void CarpiDial::_drawBackground(QPainter *painter) {
    QLinearGradient gradient(width() / 2, 0, width() / 2, height());
    QPen pen;

    gradient.setColorAt(0, QColor(0x20, 0x20, 0x20));
    gradient.setColorAt(0.3, QColor(0x33, 0x33, 0x33));
    gradient.setColorAt(1, Qt::black);

    pen.setWidth(2);
    pen.setColor(Qt::gray);
    painter->setPen(pen);
    painter->setBrush(QBrush(gradient));
    painter->drawRoundedRect(rect(), 10, 10);
}

void CarpiDial::_drawLabels(QPainter * painter) {
    QFont font;

    painter->setPen(Qt::white);

    font.setFamily("Sans");

    /* Nazwa wskaźnika */
    font.setBold(true);
    font.setPointSize(10);
    painter->setFont(font);
    painter->drawText(QRect(10, height() - 25, width() - 20, 20), Qt::AlignCenter, _name);

    /* Wartość */
    font.setPointSize(20);
    font.setBold(true);
    painter->setFont(font);
    painter->drawText(QRect(10, height() - 50, width() - 20, 22), Qt::AlignCenter, QString::number(_value, 'f', _valuePrecision));

    /* Jednostka */
    font.setPointSize(12);
    font.setBold(true);
    painter->setFont(font);
    painter->drawText(QRect(10, height() - 75, width() - 20, 20), Qt::AlignCenter, _unitName);

    /* Wartości graniczne */
    font.setPointSize(9);
    font.setBold(true);
    painter->setFont(font);
    painter->drawText(QRect(4, height() / 2 + 20, 30, 20), Qt::AlignCenter, QString("%1").arg(_min));
    painter->drawText(QRect(width() - 34, height() / 2 + 20, 30, 20), Qt::AlignCenter, QString("%1").arg(_max));
}

void CarpiDial::_drawScale(QPainter *painter) {
    QPen pen;
    QRect scaleRect(20, 20, width() - 40, width() - 40);
    QRect valueRect(32, 32, width() - 64, width() - 64);

    struct DialRange range;
    int i, ticks;
    double anglesPerValue = 180 / (_max - _min);

    /* Tło skali */
    pen.setColor(Qt::black);
    pen.setCapStyle(Qt::FlatCap);
    pen.setWidth(10);

    painter->setPen(pen);
    painter->drawArc(scaleRect, 0 * 16, 180 * 16);

    /* Obszary krytyczne */
    pen.setColor(Qt::red);
    pen.setWidth(6);
    painter->setPen(pen);

    for(i = 0; i < _cricitalRanges.size(); i++) {
        range = _cricitalRanges.at(i);
        painter->drawArc(scaleRect, (180 - (range.start - _min) * anglesPerValue) * 16, -(range.stop - range.start) * anglesPerValue * 16);
    }

    /* Obszary ostrzegawcze */
    pen.setColor(QColor(0xFF, 0xA5, 0x00));
    pen.setWidth(6);
    painter->setPen(pen);

    for(i = 0; i < _warningRanges.size(); i++) {
        range = _warningRanges.at(i);
        painter->drawArc(scaleRect, (180 - (range.start - _min) * anglesPerValue) * 16, -(range.stop - range.start) * anglesPerValue * 16);
    }

    /* Podziałka */
    pen.setColor(Qt::white);
    pen.setWidth(8);
    painter->setPen(pen);
    ticks = (_max - _min) / _scaleTick;
    for(i = 0; i <= ticks; i++) {
        painter->drawArc(scaleRect, (180 - (i * _scaleTick * anglesPerValue)) * 16 , 2 * 16);
    }

    /* Wartość */
    pen.setColor(QColor(0xE0, 0xE0, 0xE0));
    pen.setWidth(9);
    pen.setStyle(Qt::SolidLine);
    painter->setPen(pen);

    if (_drawFromZero) {
        painter->drawArc(valueRect, -(_min * anglesPerValue) * 16, -_value * anglesPerValue * 16);
    }
    else {
        painter->drawArc(valueRect, 180 * 16, -(_value - _min) * anglesPerValue * 16);
    }
}
