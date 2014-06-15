#ifndef ELM327_H
#define ELM327_H

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QVector>
#include <QList>
#include <QTimer>
#include <QThread>

class Elm327 : public QObject {
    Q_OBJECT
private:
    static Elm327 * _instance;
    static QThread _workerThread;

    QSerialPort * _serial;
    QList<int> _watchList;
    QTimer * _watchTimer;
    bool _started;

    explicit Elm327(QString portName, QObject *parent = 0);
    ~Elm327();
    QStringList _sendCommand(QString cmd);
    bool _setProtocol(int num);
    QVector<int> _readPID(int mode, int pid);
    double _readVoltage(void);

private slots:
    void _refreshData(void);

public:
    static Elm327 * getInstance(void);

signals:
    void pidValueChanged(int pid, QVector<int> data);
    void voltageChanged(double voltage);

public slots:
    void addWatchPid(int pid);
    void start(void);
    void stop(void);

};

#endif // ELM327_H
