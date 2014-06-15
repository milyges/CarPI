#ifndef CARPIDIAL_H
#define CARPIDIAL_H

#include <QWidget>
#include <QList>

struct DialRange {
    int start;
    int stop;
};

class CarpiDial : public QWidget {
    Q_OBJECT
public:
    explicit CarpiDial(QWidget *parent = 0);
    void setDialName(QString name);
    void setDialUnit(QString unit);
    void setMax(double max);
    void setMin(double min);
    void setValue(double value);
    void setValuePrecision(int prec);
    void setCriticalRange(double start, double stop);
    void setWarningRange(double start, double stop);
    void setDrawFromZero(bool isset);
    void setScaleTick(double tick);

protected:
    virtual void paintEvent(QPaintEvent * ev);

private:
    QString _name;
    QString _unitName;

    double _min;
    double _max;
    double _value;
    double _scaleTick;
    int _valuePrecision;

    bool _drawFromZero;

    QList<struct DialRange> _cricitalRanges;
    QList<struct DialRange> _warningRanges;

    void _drawBackground(QPainter * painter);
    void _drawLabels(QPainter * painter);
    void _drawScale(QPainter * painter);


};

#endif // CARPIDIAL_H
