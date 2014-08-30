#include "wndcallinfo.h"
#include "ui_wndcallinfo.h"

WndCallInfo::WndCallInfo(QWidget *parent) : QDialog(parent), _ui(new Ui::WndCallInfo) {
    _ui->setupUi(this);
}

WndCallInfo::~WndCallInfo() {
    delete _ui;
}
