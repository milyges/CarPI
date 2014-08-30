#ifndef WNDCALL_H
#define WNDCALL_H

#include <QDialog>
#include <QTimer>
#include <QTime>

#include "bluetooth.h"

namespace Ui {
    class WndCall;
}

class WndCall : public QDialog {
    Q_OBJECT    

private:
    Ui::WndCall * _ui;
    QTimer * _callTimer;
    QTime _callDuration;

private slots:
    void _callTimerTick(void);

public:
    explicit WndCall(QWidget *parent = 0);
    ~WndCall();
    
public slots:
    void callStateChanged(enum BluetoothCallState state);

protected:
    void changeEvent(QEvent *e);
    

};

#endif // WNDCALL_H
