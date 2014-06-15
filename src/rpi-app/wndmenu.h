#ifndef WNDMENU_H
#define WNDMENU_H

#include <QDialog>
#include <QLabel>
#include <QList>

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
    Ui::WndMenu * _ui;
    QList<QLabel*> _items;
    int _selectedItem;

    void _clearItems(void);
    void _setSelected(int num);

public slots:
    void showMenu(int items);
    void hideMenu(void);
    void setMenuItem(int itemno, QString text, bool isSelected);
};

#endif // WNDMENU_H
