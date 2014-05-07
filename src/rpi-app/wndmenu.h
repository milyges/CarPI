#ifndef WNDMENU_H
#define WNDMENU_H

#include <QDialog>
#include <QList>
#include <QLabel>

namespace Ui {
class WndMenu;
}

class WndMenu : public QDialog {
    Q_OBJECT
    
public:
    explicit WndMenu(QWidget *parent = 0);
    ~WndMenu();
    
protected:
    void changeEvent(QEvent *e);
    
private:
    Ui::WndMenu *ui;
    QList<QLabel*> _items;
    int _active_item;

    void _clear_items(void);
    void _set_active(int num);


public slots:
    void show_menu(int items);
    void hide_menu(void);
    void set_menu_item(int idx, QString text, bool is_checked);

};

#endif // WNDMENU_H
