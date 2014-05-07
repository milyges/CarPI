#include "wndmenu.h"
#include "ui_wndmenu.h"
#include <QDebug>

WndMenu::WndMenu(QWidget *parent) : QDialog(parent), ui(new Ui::WndMenu) {
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
}

WndMenu::~WndMenu() {
    delete ui;
}

void WndMenu::changeEvent(QEvent *e) {
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void WndMenu::_clear_items() {
    while(!_items.isEmpty()) {
        delete _items.takeLast();
    }
}

void WndMenu::_set_active(int num) {
    QLabel * item;

    if (_active_item >= 0) {
        item = _items.at(_active_item);
        item->setProperty("checked", false);
        item->style()->unpolish(item);
        item->style()->polish(item);
    }

    _active_item = num;
    item = _items.at(_active_item);
    item->setProperty("checked", true);
    item->style()->unpolish(item);
    item->style()->polish(item);

    ui->scrollArea->ensureWidgetVisible(item);
}

void WndMenu::show_menu(int items) {
    QLabel * item;
    if (!isVisible()) {
        _active_item = -1;
        _clear_items();
        for(int i = 0; i < items; i++) {
            item = new QLabel();
            ui->verticalLayout_2->addWidget(item);
            _items.append(item);
        }        
        show();
        move(QPoint(480 / 2 - width() / 2, 272 / 2 - height() / 2));
    }
}

void WndMenu::hide_menu() {
    hide();
    _active_item = -1;
    _clear_items();
}

void WndMenu::set_menu_item(int idx, QString text, bool is_checked) {
    QLabel * item;
    item = _items.at(idx);
    if (item) {
        item->setText(text);
    }

    if (is_checked)
        _set_active(idx);
}
