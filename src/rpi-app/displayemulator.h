#ifndef DISPLAYEMULATOR_H
#define DISPLAYEMULATOR_H

#include <QObject>
#include <QTimer>
#include <QSocketNotifier>
#include <QStringList>
#include <stdint.h>

#include <net/if.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>

#include <linux/can.h>
#include <linux/can/raw.h>

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

class DisplayEmulator : public QObject {
    Q_OBJECT
private:
    int _socket_fd;
    QSocketNotifier * _socket_notifier;
    QTimer * _sync_timer;
    int _sync_timeout;

    QTimer * _blink_timer;
    QStringList _blink_texts;
    int _blink_idx;

    int _socket_send(struct can_frame * frame);
    void _send_reply(struct can_frame * frame, bool last = true);
    void _register_id(uint16_t id);

    void _packet_recv_sync(struct can_frame * frame);
    void _packet_recv_dispctrl(struct can_frame * frame);
    void _packet_recv_settext(struct can_frame * frame);

    void _can_reset(void);

    void _blink_add_text(QString text, int pos);

private slots:
    void _socket_recv(int fd);
    void _sync_timer_tick(void);
    void _blink_timer_tick(void);

public:
    explicit DisplayEmulator(const char * can_iface, QObject *parent = 0);
    ~DisplayEmulator();

    void set_state(bool enabled);
    void send_key(int keycode);

signals:
    void state_change_request(bool enabled);
    void icons_changed(uint16_t icon_mask);
    void radio_text_changed(QString text, int chan);
    void radio_menu_show(int items);
    void radio_menu_hide(void);
    void radio_menu_set_item(int idx, QString val, bool checked);
};

#endif // DISPLAYEMULATOR_H
