#ifndef NAVIT_H
#define NAVIT_H

#include <QObject>
#include <QProcess>
#include <QTimer>
#include <QVector>
#include <QRect>
#include <QPoint>
#include <X11/Xlib.h>

enum NavitKey {
    navitKeyNone = 0,
    navitKeyReturn,
    navitKeyEscape,
    navitKeyUp,
    navitKeyRight,
    navitKeyDown,
    navitKeyLeft
};

class Navit : public QObject {
    Q_OBJECT
    Q_PROPERTY(QRect geometry READ geometry WRITE setGeometry)

private:
    static const int _expectWindows = 2; /* Ilość okien navita jakie powinnismy znalezc */
    static Navit * _instance;

    QProcess * _proc;
    bool _isRunning;
    Window _wndID;
    QVector<Window> _navitWindows;
    QTimer * _searchTimer;

    /* Informacje o oknie */
    bool _isVisible;
    QRect _geometry;

    explicit Navit(QObject *parent = 0);
    ~Navit();

    void _sendKeyEvent(bool press, int keycode, int mod);

private slots:
    void _searchWindow(Window startWinID = 0);
    void _navitFinished(int code);

public:
    static Navit * getInstance(void);

    QRect geometry();
    void setGeometry(QRect geom);

    Window windowID(void);

signals:

public slots:
    void start(void);
    void stop(void);

    void show(void);
    void hide(void);
    void raise(void);

    void move(int x, int y);
    void resize(int w, int h);

    void sendKey(enum NavitKey key);
};

#endif // NAVIT_H
