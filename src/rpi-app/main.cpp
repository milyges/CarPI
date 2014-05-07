#include <QApplication>
#include <QDebug>
#include "wndmain.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    a.setOrganizationName("milyges");
    a.setOrganizationDomain("milyges.dug.net.pl");
    a.setApplicationName("CarPI");

    WndMain w;

    //qDebug() << "Starting application";
    w.showFullScreen();
    
    return a.exec();
}
