#include "navit.h"
#include <QDebug>

Navit::Navit(QWidget *parent) : QObject(parent) {
    _container = new QX11EmbedContainer(parent);
    _container->setFixedSize(parent->width(), parent->height());
    _container->setContentsMargins(0, 0, 0, 0);
    QString wId = QString::number(_container->winId());
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();

    _process = new QProcess(_container);
    env.insert("NAVIT_XID", wId);
    _process->setEnvironment(env.toStringList());
    //qDebug() << "Navit windows id = " << wId;
    _process->setWorkingDirectory("/root/.navit");
    connect(_process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(_navit_finished(int, QProcess::ExitStatus)));
}

Navit::~Navit() {
    if (_started)
        stop();

    delete _container;
    delete _process;
}

void Navit::start() {
    //qDebug("starting navit");
    _process->start("navit");
    _started = true;
}

void Navit::stop() {
    _started = false;
    _process->terminate();
}

void Navit::_navit_finished(int err, QProcess::ExitStatus exitStatus) {
    if (_started) {
        qDebug("navit: restarting");
        _process->start("navit");
    }
}
