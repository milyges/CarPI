#include "ui_menumodule.h"
#include "menumodule.h"
#include <QStyle>
#include <QScrollBar>
#include <QDebug>

/*************************************************************************************/

MenuModule::MenuModule(QWidget *parent) : GuiModule(parent), _ui(new Ui::MenuModule) {
    _rootMenu = NULL;
    _activeMenu = NULL;

    _ui->setupUi(this);
    _ui->lbIcon->clear();
}

MenuModule::~MenuModule() {
    delete _ui;
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
        case keyVolUp: _setCurrentItem(_activeMenu->currentIndex() - 1); break;
        case keyVolDown: _setCurrentItem(_activeMenu->currentIndex() + 1); break;
        case keyPause: {
            MenuItem * item = _activeMenu->currentItem();
            if (item->subMenu()) {
                _setActiveMenu(item->subMenu());
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
                _setActiveMenu(_activeMenu->parentMenu());
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

bool MenuModule::useRadioText() {
    return false;
}

void MenuModule::setRootMenu(Menu *menu) {
    if (_activeMenu)
        _activeMenu->hideMenu();

    _rootMenu = menu;
    _setActiveMenu(menu);
    _activeMenu->showMenu();
    resizeEvent(NULL);
}

void MenuModule::radioIconsChanged(bool news, bool traffic, bool afrds) {

}

void MenuModule::resizeEvent(QResizeEvent *event) {
    _updateIcon();
}

void MenuModule::_updateIcon() {
    MenuItem * item;
    if (!_activeMenu) {
        _ui->lbIcon->clear();
        return;
    }

    item = _activeMenu->currentItem();
    if (!item) {
        _ui->lbIcon->clear();
        return;
    }

    _ui->lbIcon->setPixmap(item->getIcon());
}

void MenuModule::_setActiveMenu(Menu *menu) {
    if (_activeMenu) {
        _ui->hlMain->removeWidget(_activeMenu);
        _activeMenu->hideMenu();
    }

    _activeMenu = menu;
    _ui->hlMain->insertWidget(1, _activeMenu);
    _ui->lbMenuName->setText(_activeMenu->menuName());
}

void MenuModule::_setCurrentItem(int item) {
    if ((_activeMenu->currentItem()) && (_activeMenu->currentItem()->subWidget())) {
        _ui->vlRight->removeWidget(_activeMenu->currentItem()->subWidget());
        _activeMenu->currentItem()->subWidget()->hide();
    }

    _activeMenu->setCurrentIndex(item);

    if ((_activeMenu->currentItem()) && (_activeMenu->currentItem()->subWidget())) {
        _ui->vlRight->insertWidget(2, _activeMenu->currentItem()->subWidget());
        _activeMenu->currentItem()->subWidget()->show();
    }
    _updateIcon();
}


/*************************************************************************************/

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

MenuItem *Menu::currentItem() {
    if ((_currentItemIndex < 0) || (_currentItemIndex >= _items.count())) {
        return NULL;
    }

    return _items[_currentItemIndex];
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
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setMinimumSize(240, 100);
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


/*************************************************************************************/

MenuItem::MenuItem(QWidget *parent) : QLabel(parent) {
    _animation = new QPropertyAnimation(this, "geometry");
    _animation->setDuration(500);
    _animation->setEasingCurve(QEasingCurve::OutCirc);
    _subMenu = NULL;
    _subWidget = NULL;
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

void MenuItem::setSubWidget(QWidget *widget) {
    _subWidget = widget;
}

QWidget *MenuItem::subWidget() {
    return _subWidget;
}
