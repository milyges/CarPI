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

class MenuItem : public QLabel {
    Q_OBJECT
private:
    QPixmap _icon;

public:
    MenuItem(QWidget * parent = 0);

    void setIcon(QPixmap & icon);
    QPixmap & getIcon(void);
    void trigger(void);

signals:
    void triggered(void);
};


class MenuModule : public GuiModule {
    Q_OBJECT
private:
    QHBoxLayout * _layout;
    QScrollArea * _scrollArea;
    int _currentItemIndex;
    QVector<MenuItem *> _menuItems;
    QLabel * _icon;

    void _setCurrentItem(int index);
    void resizeEvent(QResizeEvent * event);

public:
    explicit MenuModule(QWidget *parent = 0);
    ~MenuModule(void);

    bool useKey(CarPIKey key);
    bool useRadioText(void);

    MenuItem * addItem(QString text, QPixmap icon);
    void removeItem(MenuItem * item);
    void clearItems(void);
    MenuItem * currentItem(void);

public slots:
    void activate(void);
    void deactivate(void);
    void keyPressed(CarPIKey key);
    void radioTextChanged(QString text);
    void radioIconsChanged(bool news, bool traffic, bool afrds);
};

#endif // MENUMODULE_H
