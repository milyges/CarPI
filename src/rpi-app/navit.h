#ifndef NAVIT_H
#define NAVIT_H

#include <QObject>
#include <QX11EmbedContainer>
#include <QProcess>

class Navit : public QObject {
    Q_OBJECT
private:
    QX11EmbedContainer * _container;
    QProcess * _process;
    bool _started;

private slots:
    void _navit_finished(int err, QProcess::ExitStatus exitStatus);

public:
    explicit Navit(QWidget *parent = 0);
    ~Navit();
    void start(void);
    void stop(void);

signals:
    
public slots:
    
};

#endif // NAVIT_H
