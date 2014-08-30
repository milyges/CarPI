#ifndef WNDCALLINFO_H
#define WNDCALLINFO_H

#include <QDialog>

namespace Ui {
    class WndCallInfo;
}

class WndCallInfo : public QDialog {
    Q_OBJECT

public:
    explicit WndCallInfo(QWidget *parent = 0);
    ~WndCallInfo();

private:
    Ui::WndCallInfo * _ui;
};

#endif // WNDCALLINFO_H
