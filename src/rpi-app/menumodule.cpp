#include "menumodule.h"
#include <QStyle>
#include <QScrollBar>
#include <QDebug>

/*****************************************************/

MenuModule::MenuModule(QWidget *parent) : GuiModule(parent) {
    _rootMenu = NULL;
    _activeMenu = NULL;

    _icon = new QLabel(this);
    _icon->setGeometry(0, 0, 64, 64);
}

MenuModule::~MenuModule() {
    delete _icon;
}

bool MenuModule::useKey(CarPIKey key) {
    switch(key) {
        case keyVolUp:
        case keyVolDown:
        case keyPause: return true;
        default: return false;
    }
}

void MenuModule::activate() {
}

void MenuModule::deactivate() {
}

void MenuModule::keyPressed(CarPIKey key) {
    if (!_activeMenu)
        return;

    switch(key) {
        case keyVolUp: _activeMenu->setCurrentIndex(_activeMenu->currentIndex() - 1); _updateIcon(); break;
        case keyVolDown: _activeMenu->setCurrentIndex(_activeMenu->currentIndex() + 1); _updateIcon(); break;
        case keyPause: {
            MenuItem * item = _activeMenu->currentItem();
            if (item->subMenu()) {
                _activeMenu->hideMenu();
                _activeMenu = item->subMenu();
                _activeMenu->showMenu();
                resizeEvent(NULL);
            }
            else {
                emit item->trigger();
            }
            break;
        }
        case keyLoad: {
            if (_activeMenu->parentMenu()) {
                _activeMenu->hideMenu();
                _activeMenu = _activeMenu->parentMenu();
                _activeMenu->show();
                resizeEvent(NULL);
            }
            break;
        }
        default: { }
    }
}

void MenuModule::radioTextChanged(QString text) {

}

void MenuModule::setRootMenu(Menu *menu) {
    if (_activeMenu)
        _activeMenu->hideMenu();

    _rootMenu = menu;
    _activeMenu = menu;
    menu->showMenu();
    resizeEvent(NULL);
}

void MenuModule::radioIconsChanged(bool news, bool traffic, bool afrds) {

}

void MenuModule::resizeEvent(QResizeEvent *event) {
    _updateIcon();
    _icon->move(10, height() / 2 - _icon->height() / 2);

    if (_activeMenu) {
        _activeMenu->setGeometry(_icon->width() + _icon->x() + 10, 0, width() / 2, height());
    }
}

void MenuModule::_updateIcon() {
    MenuItem * item;
    if (!_activeMenu) {
        _icon->clear();
        return;
    }

    item = _activeMenu->currentItem();
    if (!item) {
        _icon->clear();
        return;
    }

    _icon->setPixmap(item->getIcon());
}

bool MenuModule::useRadioText() {
    return false;
}

/*****************************************************/

MenuItem * Menu::addItem(QString text, QPixmap icon) {
    MenuItem * item = new MenuItem(this);
    item->setText(text);
    item->setIcon(icon);
    item->adjustSize();

    _items.append(item);

    if (_currentItemIndex < 0) {
        setCurrentIndex(0);
    }

    return item;
}

void Menu::removeItem(MenuItem *item) {

}

void Menu::clear() {
    _currentItemIndex = -1;
    for(int i = 0; i < _items.count(); i++) {
        delete _items[i];
    }
    _items.clear();
}

void Menu::setCurrentIndex(int index) {
    int current_x, current_y;

    if ((index < 0) || (index >= _items.count())) {
        return;
    }

    /* Jeżeli istnieje, odznaczamy poprzedni element */
    if ((_currentItemIndex >= 0) && (_currentItemIndex < _items.count())) {
        _items[_currentItemIndex]->setProperty("selected", false);
        _items[_currentItemIndex]->style()->unpolish(_items[_currentItemIndex]);
        _items[_currentItemIndex]->style()->polish(_items[_currentItemIndex]);
        _items[_currentItemIndex]->update();
    }

    _currentItemIndex = index;
    _items[_currentItemIndex]->setProperty("selected", true);
    _items[_currentItemIndex]->style()->unpolish(_items[_currentItemIndex]);
    _items[_currentItemIndex]->style()->polish(_items[_currentItemIndex]);
    _items[_currentItemIndex]->update();
    //_icon->setPixmap(_menuItems[_currentItemIndex]->getIcon());

    current_x = 5;
    current_y = height() / 2 - _items[_currentItemIndex]->height() / 2;

    _items[_currentItemIndex]->moveTo(current_x, current_y);
    //_icon->move(width() / 2 - _icon->width() / 2, height() / 2 - _icon->height() / 2);

    for(int i = _currentItemIndex - 1; i >= 0; i--) {
        _items[i]->moveTo(current_x, current_y - _items[i]->height() * (_currentItemIndex - i));
    }

    for(int i = _currentItemIndex + 1; i < _items.count(); i++) {
        _items[i]->moveTo(current_x, current_y + _items[_currentItemIndex]->height() + _items[i]->height() * (i - _currentItemIndex - 1));
    }
}

void Menu::setMenuName(QString name) {
    _name = name;
}

QString Menu::menuName() {
    return _name;
}

void Menu::showMenu() {
    for(int i = 0; i < _items.count(); i++) {
        _items[i]->move(0, 0);
    }
    setCurrentIndex(0);
    show();
}

void Menu::hideMenu() {
    hide();
}

Menu *Menu::parentMenu() {
    return _parent;
}

Menu::Menu(QWidget *parent, Menu *parentMenu) : QWidget(parent) {
    _currentItemIndex = -1;
    _parent = parentMenu;
    hide();
}

Menu::~Menu() {
    for(int i = 0; i < _items.count(); i++) {
        delete _items[i];
    }

    _items.clear();
}

int Menu::currentIndex() {
    return _currentItemIndex;
}


void Menu::resizeEvent(QResizeEvent *event) {
    setCurrentIndex(_currentItemIndex);
}


/*****************************************************/

MenuItem::MenuItem(QWidget *parent) : QLabel(parent) {
    _animation = new QPropertyAnimation(this, "geometry");
    _animation->setDuration(500);
    _animation->setEasingCurve(QEasingCurve::OutCirc);
    _subMenu = NULL;
}

MenuItem::~MenuItem() {
    delete _animation;
}

void MenuItem::setIcon(QPixmap &icon) {
    _icon = icon;
}

QPixmap &MenuItem::getIcon() {
    return _icon;
}

void MenuItem::trigger() {
    emit triggered();
}

void MenuItem::moveTo(int x, int y) {
    _animation->stop();
    _animation->setStartValue(geometry());
    _animation->setEndValue(QRect(x, y, width(), height()));
    _animation->start();
}

void MenuItem::setSubMenu(Menu *menu) {
    _subMenu = menu;
}

Menu *MenuItem::subMenu() {
    return _subMenu;
}


MenuItem *Menu::currentItem() {
    if ((_currentItemIndex < 0) || (_currentItemIndex >= _items.count())) {
        return NULL;
    }

    return _items[_currentItemIndex];
}
