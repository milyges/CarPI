#ifndef MAINBOARD_H
#define MAINBOARD_H

#include <QObject>
#include <QFile>
#include <QTimer>


/* Definicje klawiszy */
#define PILOT_KEY_SOURCE_L   (1 << 0)
#define PILOT_KEY_SOURCE_R   (1 << 1)
#define PILOT_KEY_LOAD       (1 << 2)
#define PILOT_KEY_VOLUP      (1 << 3)
#define PILOT_KEY_VOLDOWN    (1 << 4)
#define PILOT_KEY_PAUSE      (PILOT_KEY_VOLUP | PILOT_KEY_VOLDOWN)
#define PILOT_ROLL_PREV      (1 << 5)
#define PILOT_ROLL_NEXT      (1 << 6)

class MainBoard : public QObject {
    Q_OBJECT
private:
    static MainBoard * _instance;
    QFile * _i2c_bus;
    bool _radioPowerEnabled;
    bool _ignitionEnabled;

    QTimer * _shutdownTimer;
    int _shutdownTimeout;

    static void _mainboardIntHandler(void);
    static void _bluetoothIntHandler(void);
    explicit MainBoard(QObject *parent = 0);
    ~MainBoard();


private slots:
    void _mainboardInt(void);
    void _shutdownTimerTick(void);

public:
    static MainBoard * getInstance();

    void bluetoothSetMode(bool cmdMode);

    void readState(void);

signals:
    void mainboardInt(void);
    void bluetoothInt(void);

    void radioPowerChanged(bool enabled);
    void ignitionChanged(bool enabled);

    void keyStateChanged(int keyState);

    void shutdown(void);

public slots:
    
};

#endif // MAINBOARD_H
