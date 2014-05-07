#ifndef MAINBOARD_H
#define MAINBOARD_H

#include <QObject>
#include <QFile>
#include <QTimer>
#include <stdint.h>

#define PILOT_KEY_SOURCE_L   (1 << 0)
#define PILOT_KEY_SOURCE_R   (1 << 1)
#define PILOT_KEY_LOAD       (1 << 2)
#define PILOT_KEY_VOLUP      (1 << 3)
#define PILOT_KEY_VOLDOWN    (1 << 4)
#define PILOT_KEY_PAUSE      (PILOT_KEY_VOLUP | PILOT_KEY_VOLDOWN)
#define PILOT_ROLL_NEXT      (1 << 5)
#define PILOT_ROLL_PREV      (1 << 6)


class MainBoard : public QObject {
    Q_OBJECT
private:
    QFile * _i2c_file;
    QTimer * _shutdown_timer;
    int _shutdown_timeout;
    double _last_temp;

    static MainBoard * _instance;

    explicit MainBoard(QObject *parent = 0);
    ~MainBoard();
    void _calculate_temp(uint16_t raw);
    static void _mainboard_int(void);
    static void _bluetooth_int(void);

private slots:
    void _shutdown_timer_tick(void);

public:
    static MainBoard * get_instance(void);
    float get_temp(void);
    void bluetooth_set_mode(bool cmd);

signals:
    void shutdown(void);
    void mainboard_new_event(void);
    void keypressed(uint8_t keymask);
    void bluetooth_event(void);

public slots:
    void mainboard_event(void);
};

#endif // MAINBOARD_H
