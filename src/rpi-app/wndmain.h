#ifndef WNDMAIN_H
#define WNDMAIN_H

#include <QMainWindow>
#include <QTimer>
#include <QMenu>
#include <QVector>
#include <QActionGroup>
#include "carpi.h"
#include "carpidial.h"

#define RADIO_TEXT_MAX       18

namespace Ui {
    class WndMain;
}

class WndMain : public QMainWindow {
    Q_OBJECT

private:
    Ui::WndMain * _ui;
    QString _radioText;
    int _radioTextPos;
    QTimer * _timerScroll;
    QTimer * _timerTimeUpdate;
    QTimer * _timerNavitMenuDetect;
    CarpiDial * _dials[6];

    QMenu * _mainMenu;
    QMenu * _viewMenu;
    QMenu * _systemMenu;

    QAction * _showMapAction;
    QAction * _showDashboardAction;
    QActionGroup * _viewMenuActions;
    bool _navitMenuEnabled;

private slots:
    void _textScroll(void);
    void _timeUpdate(void);

    void _menuHide(void);

    void _setViewNavit(void);
    void _setViewDashboard(void);
    void _showNavitMenu(void);

    void _systemShutdown(void);

    void _navitMenuDetectTick(void);

public:
    explicit WndMain(QWidget *parent = 0);
    ~WndMain();

    QWidget * getNavitWidget(void);

public slots:
    void setRadioText(QString text);
    void setRadioIcons(bool news, bool traffic, bool afrds);
    void setSource(enum CarPISource source);
    void setBluetoothIcon(bool isConnected);

    void dialValueChanged(enum CarPIDialsID dialID, double value);

    void changeDisplayMode(void);

    void menuShow(void);
    void menuKeyPressed(enum CarPIKey key);

protected:
    void changeEvent(QEvent *e);
    
signals:
    void mainMenuHide(void);
    void shutdown(void);

};

#endif // WNDMAIN_H
