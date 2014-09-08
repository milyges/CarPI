#ifndef WNDMAIN_H
#define WNDMAIN_H

#include <QMainWindow>
#include <QTimer>
#include "wndvolume.h"
#include "wndcallinfo.h"
#include "guimodule.h"
#include "menumodule.h"
#include "dashboardmodule.h"
#include "radiomodule.h"
#include "carpi.h"
#include "navigationmodule.h"

namespace Ui {
    class WndMain;
}

class WndMain : public QMainWindow {
    Q_OBJECT
    

public:
    explicit WndMain(CarPI *parent);
    ~WndMain();
    void start(void);

protected:
    void changeEvent(QEvent *e);
    
private:
    Ui::WndMain *_ui;
    CarPI * _carpi;
    WndVolume * _wndVolume;
    WndCallInfo * _wndCallInfo;
    GuiModule * _currentModule;
    MenuModule * _menuModule;
    NavigationModule * _navigationModule;
    DashboardModule * _dashboardModule;
    RadioModule * _radioModule;

    Menu * _mainMenu;

    QTimer * _updateTimeTimer;

    void _setModule(GuiModule * mod);

private slots:
    void _radioTextChanged(QString text);
    void _radioIconsChanged(bool news, bool traffic, bool afrds);
    void _bluetoothStateChanged(bool connected);
    void _bluetoothCallStateChanged(BluetoothCallState state, QString number);
    void _sourceChanged(CarPISource source);
    void _keyPressed(CarPIKey key);
    void _updateTime(void);

    void _navigationScreen(void);
    void _dashboardScreen(void);
    void _radioScreen(void);
};

#endif // WNDMAIN_H
