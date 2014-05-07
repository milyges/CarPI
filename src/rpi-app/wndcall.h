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
    Ui::WndCall *ui;
    QTimer * _call_timer;
    QTime _call_duration;
    int _status;

private slots:
    void _call_timer_tick(void);

public:
    explicit WndCall(QWidget *parent = 0);
    ~WndCall();

public slots:
    void update_status(int newstatus);

protected:
    void changeEvent(QEvent *e);
};

#endif // WNDCALL_H
