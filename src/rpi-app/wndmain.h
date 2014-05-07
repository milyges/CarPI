#ifndef WNDMAIN_H
#define WNDMAIN_H

#include <QMainWindow>
#include <QCloseEvent>
#include "displayemulator.h"
#include "navit.h"
#include "mainboard.h"
#include "wndmenu.h"
#include "changeremulator.h"
#include "mp3player.h"
#include "wndvolume.h"
#include "bluetooth.h"
#include "wndcall.h"

namespace Ui {
    class WndMain;
}

enum {
    sourceFM = 0x00,
    sourceCD = 0x01,
    sourceCDChanger = 0x02,
    sourceAUX = 0x03
};

#define RADIO_TEXT_MAX_LEN     18

class WndMain : public QMainWindow
{
    Q_OBJECT
private:
    Ui::WndMain * _ui;
    DisplayEmulator * _display;
    ChangerEmulator * _cd_changer;
    QTimer * _update_timer;
    QTimer * _scroll_timer;
    Navit * _navit;
    MainBoard * _main_board;
    MP3Player * _mp3_player;
    WndMenu * _wnd_menu;
    WndVolume * _wnd_volume;
    WndCall * _wnd_call;
    Bluetooth * _bluetooth;

    QString _radio_text;
    int _radio_text_pos;
    int _current_source;
    bool _paused;

    void _switch_source(int newsrc);

private slots:
    void _display_state_change(bool enabled);
    void _radio_text_changed(QString text, int chan);
    void _radio_icons_changed(uint16_t icon_mask);
    void _update_timer_tick(void);
    void _scroll_timer_tick(void);
    void _my_close(void);
    void _pilot_key_pressed(uint8_t keymask);
    void _set_radio_text(QString text);
    void _bluetooth_state_changed(bool connected);
    void _bluetooth_call_status_changed(int newstate);
    void _set_source(int source);

public:
    explicit WndMain(QWidget *parent = 0);
    ~WndMain();

protected:
    void changeEvent(QEvent *e);
    

};

#endif // WNDMAIN_H
