#ifndef GUIMODULE_H
#define GUIMODULE_H

#include <QWidget>
#include "carpi.h"

class GuiModule : public QWidget {
    Q_OBJECT
public:
    GuiModule(QWidget * parent = 0) : QWidget(parent) {
    }
    virtual bool useKey(CarPIKey key) = 0;
    virtual bool useRadioText(void) = 0;

public slots:
    virtual void activate(void) = 0;
    virtual void deactivate(void) = 0;
    virtual void keyPressed(CarPIKey key) = 0;
    virtual void radioTextChanged(QString text) = 0;
    virtual void radioIconsChanged(bool news, bool traffic, bool afrds) = 0;
};

#endif // GUIMODULE_H
