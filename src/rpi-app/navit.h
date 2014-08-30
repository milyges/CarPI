#ifndef NAVIT_H
#define NAVIT_H

#include <QObject>
#include <QX11EmbedContainer>
#include <QProcess>
#include <QWidget>
#include <QTimer>
#include <stdint.h>

enum NavitKey {
    navitUp = 1,
    navitRight,
    navitDown,
    navitLeft,
    navitReturn,
    navitEscape
};

class Navit : public QObject {
    Q_OBJECT
private:
    static Navit * _instance;

    QProcess * _process;
    bool _isStarted;
    bool _isVisible;

    uint32_t _windowID;

    explicit Navit(QObject *parent = 0);
    ~Navit(void);

    uint32_t  _searchWindow(void);

private slots:
    void _navitFinished(int err, QProcess::ExitStatus exitStatus);
    void _readFromNavit(void);

public:
    static Navit * getInstance(void);
    void start(void);
    void stop(void);
    void sendKey(enum NavitKey key);

    void show(void);
    void hide(void);

    uint32_t getWindowId(void);
};

#endif // NAVIT_H
