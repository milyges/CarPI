#ifndef SCROLLEDLABEL_H
#define SCROLLEDLABEL_H

#include <QLabel>
#include <QTimer>

class ScrolledLabel : public QLabel {
    Q_OBJECT
private:
    int _maxLength;
    QTimer * _timer;
    QString _text;
    int _pos;

private slots:
    void _timerTick(void);

public:
    explicit ScrolledLabel(QWidget *parent = 0);    
    ~ScrolledLabel();

    void setMaxLength(int len);
    int maxLength(void);

    void setText(const QString & text);

signals:
    
public slots:
    
};

#endif // SCROLLEDLABEL_H
