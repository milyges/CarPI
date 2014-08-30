#include <QApplication>
#include <QObject>
#include <QFile>
//#include "wndmain.h"
//#include "wndvolume.h"
//#include "wndmenu.h"
//#include "wndcall.h"
#include "carpi.h"
#include "navit.h"
#include "wndmain.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("CarPI");
    app.setOrganizationName("milyges");
    qRegisterMetaType<QVector<int> >("QVector<int>");

    QFile styleFile("/home/radio/CarPI/themes/default.css");
    styleFile.open(QFile::ReadOnly);
    QString style = QLatin1String(styleFile.readAll());
    app.setStyleSheet(style);

    CarPI carpi;
    WndMain gui(&carpi);

    gui.start();

    //QObject::connect(&carpi, SIGNAL(radioIconsChanged(bool,bool,bool)), &wndMain, SLOT(setRadioIcons(bool,bool,bool)));
    //QObject::connect(&carpi, SIGNAL(radioTextChanged(QString)), &wndMain, SLOT(setRadioText(QString)));
    //QObject::connect(&carpi, SIGNAL(sourceChanged(CarPISource)), &wndMain, SLOT(setSource(CarPISource)));
    //QObject::connect(&carpi, SIGNAL(dialValueChanged(CarPIDialsID,double)), &wndMain, SLOT(dialValueChanged(CarPIDialsID,double)));
    //QObject::connect(&carpi, SIGNAL(changeDisplayMode()), &wndMain, SLOT(changeDisplayMode()));
    //QObject::connect(&carpi, SIGNAL(mainMenuEnter()), &wndMain, SLOT(menuShow()));
    //QObject::connect(&carpi, SIGNAL(mainMenuKeyPressed(CarPIKey)), &wndMain, SLOT(menuKeyPressed(CarPIKey)));
    //QObject::connect(&wndMain, SIGNAL(mainMenuHide()), &carpi, SLOT(mainMenuExit()));
    //QObject::connect(&wndMain, SIGNAL(shutdown()), &carpi, SLOT(shutdown()));

    //QObject::connect(&carpi, SIGNAL(volumeChanged(int)), &wndVolume, SLOT(showVolume(int)));

    //QObject::connect(&carpi, SIGNAL(menuShow(int)), &wndMenu, SLOT(showMenu(int)));
    //QObject::connect(&carpi, SIGNAL(menuHide()), &wndMenu, SLOT(hideMenu()));
    //QObject::connect(&carpi, SIGNAL(menuSetItem(int,QString,bool)), &wndMenu, SLOT(setMenuItem(int,QString,bool)));

    //QObject::connect(&carpi, SIGNAL(bluetoothConnectionStateChanged(bool)), &wndMain, SLOT(setBluetoothIcon(bool)));

    //QObject::connect(&carpi, SIGNAL(bluetoothCallStateChanged(BluetoothCallState)), &wndCall, SLOT(callStateChanged(BluetoothCallState)));

    //carpi.updateStatus();

    //wndMain.showFullScreen();

    return app.exec();
}
