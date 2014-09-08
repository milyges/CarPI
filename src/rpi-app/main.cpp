#include <QApplication>
#include <QObject>
#include <QFile>
#include "wndmain.h"
#include "carpi.h"

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

    return app.exec();
}
