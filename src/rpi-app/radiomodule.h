#ifndef RADIOMODULE_H
#define RADIOMODULE_H

#include "ui_radioModule.h"
#include "guimodule.h"
#include "scrolledlabel.h"
#include "carpi.h"

class RadioModule : public GuiModule {
    Q_OBJECT
private:
    Ui_RadioModule * _ui;

public:
    explicit RadioModule(QWidget *parent = 0);
    ~RadioModule();

    bool useKey(CarPIKey key);
    bool useRadioText(void);
signals:

public slots:
    void activate(void);
    void deactivate(void);
    void keyPressed(CarPIKey key);
    void radioTextChanged(QString text);
    void radioSourceChanged(CarPISource source);
    void radioIconsChanged(bool news, bool traffic, bool afrds);
};

#endif // RADIOMODULE_H
