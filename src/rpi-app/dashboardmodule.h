#ifndef DASHBOARDMODULE_H
#define DASHBOARDMODULE_H

#include "guimodule.h"
#include "carpidial.h"

class DashboardModule : public GuiModule
{
    Q_OBJECT
private:
    CarpiDial * _dials[6];

public:
    explicit DashboardModule(QWidget *parent = 0);
    ~DashboardModule();

    bool useKey(CarPIKey key);
    bool useRadioText(void);
signals:
    
public slots:
    void activate(void);
    void deactivate(void);
    void keyPressed(CarPIKey key);
    void radioTextChanged(QString text);
    void radioIconsChanged(bool news, bool traffic, bool afrds);

    void dialValueChanged(enum CarPIDialsID dialID, double value);
};

#endif // DASHBOARDMODULE_H
