#include "wndmenu.h"
#include "ui_wndmenu.h"

WndMenu::WndMenu(QWidget *parent) : QDialog(parent), _ui(new Ui::WndMenu) {
    _ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
}

WndMenu::~WndMenu() {
    delete _ui;
}

void WndMenu::changeEvent(QEvent *e) {
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        _ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void WndMenu::_clearItems() {
    while(!_items.isEmpty()) {
        delete _items.takeLast();
    }
}

void WndMenu::_setSelected(int num) {
    QLabel * item;

    if (_selectedItem >= 0) {
        item = _items.at(_selectedItem);
        item->setProperty("checked", false);
        item->style()->unpolish(item);
        item->style()->polish(item);
    }

    _selectedItem = num;
    item = _items.at(_selectedItem);
    item->setProperty("checked", true);
    item->style()->unpolish(item);
    item->style()->polish(item);

    _ui->scrollArea->ensureWidgetVisible(item);
}

void WndMenu::showMenu(int items) {
    QLabel * item;
    if (!isVisible()) {
        _selectedItem = -1;
        _clearItems();
        for(int i = 0; i < items; i++) {
            item = new QLabel();
            _ui->verticalLayout_2->addWidget(item);
            _items.append(item);
        }
        show();
        move(QPoint(480 / 2 - width() / 2, 272 / 2 - height() / 2));
    }
}

void WndMenu::hideMenu() {
    hide();
    _selectedItem = -1;
    _clearItems();
}

void WndMenu::setMenuItem(int itemno, QString text, bool isSelected) {
    QLabel * item;
    item = _items.at(itemno);
    if (item) {
        item->setText(QString(" %1").arg(text));
    }

    if (isSelected)
        _setSelected(itemno);
}
