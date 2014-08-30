#include "dashboardmodule.h"

DashboardModule::DashboardModule(QWidget *parent) : GuiModule(parent) {
    for(int i = 0; i < 2; i++) {
        for(int j = 0; j < 3; j++) {
            _dials[i * 3 + j] = new CarpiDial(this);
            _dials[i * 3 + j]->setGeometry(2 + j * 159, i * 119, 155, 115);
        }
    }


    _dials[dialIntakeTemp]->setDialName("Intake Air Temp");
    _dials[dialIntakeTemp]->setDialUnit(QString::fromUtf8("°C"));
    _dials[dialIntakeTemp]->setMin(-30);
    _dials[dialIntakeTemp]->setMax(170);
    _dials[dialIntakeTemp]->setScaleTick(20);

    _dials[dialBoost]->setDialName("Boost");
    _dials[dialBoost]->setDialUnit("PSI");
    _dials[dialBoost]->setMin(-25);
    _dials[dialBoost]->setMax(25);
    _dials[dialBoost]->setScaleTick(5);
    _dials[dialBoost]->setDrawFromZero(true);
    _dials[dialBoost]->setValuePrecision(1);

    _dials[dialFuelPressure]->setDialName("Fuel Pressure");
    _dials[dialFuelPressure]->setDialUnit("MPa");
    _dials[dialFuelPressure]->setMin(0);
    _dials[dialFuelPressure]->setMax(150);
    _dials[dialFuelPressure]->setScaleTick(15);

    _dials[dialCoolantTemp]->setDialName("Coolant Temp");
    _dials[dialCoolantTemp]->setDialUnit(QString::fromUtf8("°C"));
    _dials[dialCoolantTemp]->setMin(-30);
    _dials[dialCoolantTemp]->setMax(170);
    _dials[dialCoolantTemp]->setScaleTick(20);
    _dials[dialCoolantTemp]->setCriticalRange(130, 170);
    _dials[dialCoolantTemp]->setWarningRange(110, 130);

    _dials[dialEngineLoad]->setDialName("Engine Load");
    _dials[dialEngineLoad]->setDialUnit("%");
    _dials[dialEngineLoad]->setMin(0);
    _dials[dialEngineLoad]->setMax(100);
    _dials[dialEngineLoad]->setScaleTick(10);
    _dials[dialEngineLoad]->setCriticalRange(90, 100);

    _dials[dialVoltage]->setDialName("Voltage");
    _dials[dialVoltage]->setDialUnit("V");
    _dials[dialVoltage]->setMin(8);
    _dials[dialVoltage]->setMax(18);
    _dials[dialVoltage]->setScaleTick(1);
    _dials[dialVoltage]->setCriticalRange(8, 10);
    _dials[dialVoltage]->setWarningRange(10, 11);
    _dials[dialVoltage]->setCriticalRange(16, 18);
    _dials[dialVoltage]->setValuePrecision(1);
}

DashboardModule::~DashboardModule() {
    for(int i = 0; i < 6; i++)
        delete _dials[i];
}

bool DashboardModule::useKey(CarPIKey key) {
    return false;
}

void DashboardModule::activate() {
}

void DashboardModule::deactivate() {
}

void DashboardModule::keyPressed(CarPIKey key) {
}

void DashboardModule::radioTextChanged(QString text) {

}

void DashboardModule::dialValueChanged(CarPIDialsID dialID, double value) {
    _dials[dialID]->setValue(value);
}


bool DashboardModule::useRadioText() {
    return false;
}


void DashboardModule::radioIconsChanged(bool news, bool traffic, bool afrds) {

}
