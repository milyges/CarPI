#include "menumodule.h"
#include <QStyle>
#include <QScrollBar>
#include <QDebug>

MenuModule::MenuModule(QWidget *parent) : GuiModule(parent) {
   _layout = new QHBoxLayout();

   _scrollArea = new QScrollArea();
   _icon = new QLabel();
   _layout->addWidget(_scrollArea);
   _layout->addWidget(_icon);
   _icon->setText("aaaa");
   _icon->setObjectName("MenuIcon");

   _layout->setContentsMargins(0, 0, 0, 0);
   setLayout(_layout);

   _scrollArea->setWidget(new QWidget());
   _scrollArea->setWidgetResizable(true);
   _scrollArea->widget()->setLayout(new QVBoxLayout());
   _scrollArea->widget()->layout()->setContentsMargins(0, 0, 0, 0);
   _currentItemIndex = -1;
}

MenuModule::~MenuModule() {
    for(int i = 0; i < _menuItems.count(); i++) {
        delete _menuItems[i];
    }
    _menuItems.clear();

    delete _scrollArea->widget()->layout();
    delete _scrollArea->widget();
    delete _scrollArea;
    delete _layout;
}

bool MenuModule::useKey(CarPIKey key) {
    switch(key) {
        case keyVolUp:
        case keyVolDown:
        case keyPause: return true;
        default: return false;
    }
}

MenuItem *MenuModule::addItem(QString text, QPixmap icon) {
    MenuItem * item = new MenuItem(_scrollArea);
    item->setText(text);
    item->setIcon(icon);

    _scrollArea->widget()->layout()->addWidget(item);

    _menuItems.append(item);

    if (_currentItemIndex < 0) {
        _setCurrentItem(0);
    }

    return item;
}

void MenuModule::removeItem(MenuItem *item) {

}

void MenuModule::clearItems() {
    _currentItemIndex = -1;
    for(int i = 0; i < _menuItems.count(); i++) {
        _scrollArea->widget()->layout()->removeWidget(_menuItems[i]);
        delete _menuItems[i];
    }
    _menuItems.clear();
    _icon->clear();
}

MenuItem *MenuModule::currentItem() {
    if ((_currentItemIndex < 0) || (_currentItemIndex >= _menuItems.count())) {
        return NULL;
    }

    return _menuItems[_currentItemIndex];
}

void MenuModule::activate() {
}

void MenuModule::deactivate() {
}

void MenuModule::keyPressed(CarPIKey key) {
    switch(key) {
        case keyVolUp: _setCurrentItem(_currentItemIndex - 1); break;
        case keyVolDown: _setCurrentItem(_currentItemIndex + 1); break;
        case keyPause: _menuItems[_currentItemIndex]->trigger();
        default: { }
    }
}

void MenuModule::radioTextChanged(QString text) {

}

MenuItem::MenuItem(QWidget *parent) : QLabel(parent) {

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

void MenuModule::_setCurrentItem(int index) {
    if ((index < 0) || (index >= _menuItems.count())) {
        return;
    }

    /* Jeżeli istnieje, odznaczamy poprzedni element */
    if ((_currentItemIndex >= 0) && (_currentItemIndex < _menuItems.count())) {
        _menuItems[_currentItemIndex]->setProperty("selected", false);
        _menuItems[_currentItemIndex]->style()->unpolish(_menuItems[_currentItemIndex]);
        _menuItems[_currentItemIndex]->style()->polish(_menuItems[_currentItemIndex]);
        _menuItems[_currentItemIndex]->update();
    }

    _currentItemIndex = index;
    _menuItems[_currentItemIndex]->setProperty("selected", true);
    _menuItems[_currentItemIndex]->style()->unpolish(_menuItems[_currentItemIndex]);
    _menuItems[_currentItemIndex]->style()->polish(_menuItems[_currentItemIndex]);
    _menuItems[_currentItemIndex]->update();
    _icon->setPixmap(_menuItems[_currentItemIndex]->getIcon());
    _scrollArea->ensureWidgetVisible(_menuItems[_currentItemIndex]);
}

void MenuModule::resizeEvent(QResizeEvent *event) {
    _scrollArea->widget()->resize(_scrollArea->size());
}


bool MenuModule::useRadioText() {
    return false;
}


void MenuModule::radioIconsChanged(bool news, bool traffic, bool afrds) {

}
