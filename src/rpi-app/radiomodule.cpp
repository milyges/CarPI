#include "radiomodule.h"

RadioModule::RadioModule(QWidget *parent) : GuiModule(parent), _ui(new Ui_RadioModule()) {
    _ui->setupUi(this);
    _ui->lbTitle->setMaxLength(24);
    _ui->lbExtraInfo->clear();
}

RadioModule::~RadioModule() {
    delete _ui;
}

bool RadioModule::useKey(CarPIKey key) {
    return false;
}

bool RadioModule::useRadioText() {
    return true;
}

void RadioModule::activate() {

}

void RadioModule::deactivate() {

}

void RadioModule::keyPressed(CarPIKey key) {

}

void RadioModule::radioTextChanged(QString text) {
    _ui->lbTitle->setText(text);
}

void RadioModule::radioSourceChanged(CarPISource source) {
    static QPixmap sources_icons[] = {
        QPixmap(":/resources/icons/source_fm_big.png"),
        QPixmap(":/resources/icons/source_cd_big.png"),
        QPixmap(":/resources/icons/source_usb_big.png"),
        QPixmap(":/resources/icons/source_bluetooth_big.png"),
    };

    _ui->lbIcon->setPixmap(sources_icons[source]);
}

void RadioModule::radioIconsChanged(bool news, bool traffic, bool afrds) {
    QString icons;

    if (news)
        icons += "NEWS";

    if (traffic)
        icons += " TRAFFIC";

    if (afrds)
        icons += " AF-RDS";

    _ui->lbRadioIcons->setText(icons);
}
