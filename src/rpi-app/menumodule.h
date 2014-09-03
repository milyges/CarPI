#ifndef MENUMODULE_H
#define MENUMODULE_H

#include <QWidget>
#include <QObject>
#include "guimodule.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QVector>
#include <QLabel>
#include <QPixmap>
#include <QFrame>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>

class Menu;

namespace Ui {
    class MenuModule;
}

class MenuItem : public QLabel {
    Q_OBJECT
private:
    QPixmap _icon;
    QPropertyAnimation * _animation;
    Menu * _subMenu;
    QWidget * _subWidget;

public:
    MenuItem(QWidget * parent = 0);
    ~MenuItem();

    void setIcon(QPixmap & icon);
    QPixmap & getIcon(void);
    void trigger(void);

    void moveTo(int x, int y);

    void setSubMenu(Menu * menu);
    Menu * subMenu(void);

    void setSubWidget(QWidget * widget);
    QWidget * subWidget(void);

signals:
    void triggered(void);
};

class Menu : public QWidget {
    Q_OBJECT
private:
    QVector<MenuItem *> _items;
    int _currentItemIndex;
    QString _name;
    Menu * _parent;

    void resizeEvent(QResizeEvent * event);

public:
    explicit Menu(QWidget * parent, Menu * parentMenu = NULL);
    ~Menu();

    int currentIndex(void);

    MenuItem * addItem(QString text, QPixmap icon);
    void removeItem(MenuItem * item);
    void clear(void);
    MenuItem * currentItem(void);
    void setCurrentIndex(int index);

    void setMenuName(QString name);
    QString menuName(void);

    void showMenu(void);
    void hideMenu(void);

    Menu * parentMenu(void);
};

class MenuModule : public GuiModule {
    Q_OBJECT
private:
    Menu * _rootMenu;
    Menu * _activeMenu;

    Ui::MenuModule * _ui;

    void _setCurrentItem(int index);
    void resizeEvent(QResizeEvent * event);
    void _updateIcon(void);
    void _setActiveMenu(Menu * menu);

public:
    explicit MenuModule(QWidget *parent = 0);
    ~MenuModule(void);

    bool useKey(CarPIKey key);
    bool useRadioText(void);

    void setRootMenu(Menu * menu);

public slots:
    void activate(void);
    void deactivate(void);
    void keyPressed(CarPIKey key);
    void radioTextChanged(QString text);
    void radioIconsChanged(bool news, bool traffic, bool afrds);
};

#endif // MENUMODULE_H
