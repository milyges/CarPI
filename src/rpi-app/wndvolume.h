#ifndef WNDVOLUME_H
#define WNDVOLUME_H

#include <QDialog>
#include <QTimer>

namespace Ui {
    class WndVolume;
}

class WndVolume : public QDialog {
    Q_OBJECT
    
public:
    explicit WndVolume(QWidget *parent = 0);
    ~WndVolume();
    void show_volume(int vol);

protected:
    void changeEvent(QEvent *e);
    
private:
    Ui::WndVolume * ui;
    QTimer * _hide_timer;
};

#endif // WNDVOLUME_H
