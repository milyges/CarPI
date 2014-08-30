#ifndef WNDVOLUME_H
#define WNDVOLUME_H

#include <QDialog>
#include <QTimer>

namespace Ui {
    class WndVolume;
}

class WndVolume : public QDialog
{
    Q_OBJECT
private:
    Ui::WndVolume *_ui;
    QTimer * _hideTimer;

public:
    explicit WndVolume(QWidget *parent = 0);
    ~WndVolume();
    
public slots:
    void showVolume(int volume);

protected:
    void changeEvent(QEvent *e);
    

};

#endif // WNDVOLUME_H
