#ifndef NAVIGATIONMODULE_H
#define NAVIGATIONMODULE_H

#include "guimodule.h"
#include "navit.h"
#include <QTimer>

class NavigationModule : public GuiModule {
    Q_OBJECT
private:
    Navit * _navit;
    bool _inMenu;
    QTimer * _inMenuDetectTimer;

private slots:
    void _inMenuDetect(void);

public:
    explicit NavigationModule(QWidget *parent = 0);
    ~NavigationModule();

    bool useKey(CarPIKey key);
    bool useRadioText(void);

signals:
    
public slots:
    void activate(void);
    void deactivate(void);
    void keyPressed(CarPIKey key);
    void radioTextChanged(QString text);
    void radioIconsChanged(bool news, bool traffic, bool afrds);
};

#endif // NAVIGATIONMODULE_H
