#ifndef DISPLAYEMULATOR_H
#define DISPLAYEMULATOR_H

#include <QObject>
#include <QThread>
#include <QTimer>
#include <QSocketNotifier>
#include <QStringList>

#include <net/if.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>

#include <linux/can.h>
#include <linux/can/raw.h>

#define DISPLAY_CAN_INTERFACE      "can0"

/* Ikonki na wyświetlaczu */
#define DISPLAY_ICON_NO_NEWS       (1 << 0)
#define DISPLAY_ICON_NEWS_ARROW    (1 << 1)
#define DISPLAY_ICON_NO_TRAFFIC    (1 << 2)
#define DISPLAY_ICON_TRAFFIC_ARROW (1 << 3)
#define DISPLAY_ICON_NO_AFRDS      (1 << 4)
#define DISPLAY_ICON_AFRDS_ARROW   (1 << 5)
#define DISPLAY_ICON_NO_MODE       (1 << 6)
#define DISPLAY_ICON_MODE_NONE     0xFF

#define DISPLAY_KEY_LOAD           0x0000 /* Ten na dole pilota ;) */
#define DISPLAY_KEY_SRC_RIGHT      0x0001
#define DISPLAY_KEY_SRC_LEFT       0x0002
#define DISPLAY_KEY_VOLUME_UP      0x0003
#define DISPLAY_KEY_VOLUME_DOWN    0x0004
#define DISPLAY_KEY_PAUSE          0x0005
#define DISPLAY_KEY_ROLL_UP        0x0101
#define DISPLAY_KEY_ROLL_DOWN      0x0141
#define DISPLAY_KEY_HOLD_MASK      (0x80 | 0x40)

enum DisplaySyncState {
    displaySyncOK = 0x01,   /* Synchronizacja OK */
    displaySyncPending,     /* Synchronizacja w trakcie negocjacji */
    displaySyncLost         /* Synchronizacja utracona */
};

class DisplayEmulator : public QObject {
    Q_OBJECT
private:
    static DisplayEmulator * _instance;
    static QThread _workerThread;

    int _socketFd;
    QSocketNotifier * _socketNotifier;
    QTimer * _syncTimer;
    enum DisplaySyncState _syncState;

    bool _menuVisible;

    explicit DisplayEmulator(QObject *parent = 0);
    ~DisplayEmulator();

    void _socketSend(struct can_frame * frame);
    void _packetSendReply(struct can_frame * frame, bool last = true);


    void _packetRecvSync(struct can_frame * frame);
    void _packetRecvDisplayControl(struct can_frame * frame);
    void _packetRecvSetText(struct can_frame * frame);

    void _registerCanId(int id);

private slots:
    void _socketRecv(int fd);
    void _syncTimeout(void);

    void _packetSendDisplayStatus(bool enabled);

public:
    static DisplayEmulator * getInstance(void);

signals:
    void displayStatusChanged(bool enabled);
    void displayIconsChanged(int mask);
    void displayTextChanged(QString text);
    void displayMenuShow(int items);
    void displayMenuItemUpdate(int item, QString text, bool isSelected);
    void displayMenuHide(void);

public slots:
    void radioPowerChanged(bool enabled);
    void sendKeyEvent(int keycode);
};

#endif // DISPLAYEMULATOR_H
