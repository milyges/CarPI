#include "navit.h"
#include <QDebug>
#include <QApplication>

Navit * Navit::_instance = NULL;

Navit::Navit(QObject *parent) : QObject(parent) {
    _isStarted = false;
    _isVisible = false;

    _process = new QProcess(this);
    _process->setWorkingDirectory("/root/.navit");
    connect(_process, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(_navitFinished(int,QProcess::ExitStatus)));
    connect(_process, SIGNAL(readyReadStandardOutput()), this, SLOT(_readFromNavit()));
}

Navit::~Navit() {
    stop();

    if (_process)
        delete _process;
}

uint32_t Navit::_searchWindow() {
    QProcess proc;
    QString wid;
    proc.start("xdotool search --onlyvisible --name navit");
    proc.waitForFinished();
    if (proc.exitCode() != 0) {
        qDebug() << "Navit: Search Window: exitCode = " << proc.exitCode();
        return 0;
    }

    wid = QString(proc.readLine());
    //qDebug() << "Navit: windowID =" << wid.toInt();
    return wid.toInt();
}

void Navit::_navitFinished(int err, QProcess::ExitStatus exitStatus) {
    if (_isStarted) {
        qDebug() << "Navit: restarting navit process";
        _isStarted = false;
        start();
    }
}

void Navit::_readFromNavit() {
    QString data = QString(_process->readAllStandardOutput());
    //qDebug() << "readFromNavit: data = " << data;
    if (!_windowID) {
        _windowID = data.toInt();
        qDebug() << "Navit: wid =" << _windowID;
        if (_isVisible) {
            show();
        }
        else {
            hide();
        }
    }
}

Navit *Navit::getInstance() {
    if (!_instance) {
        _instance = new Navit();
    }

    return _instance;
}

void Navit::start() {
    qDebug() << "Navit start, proc = " << _process;

    if ((!_isStarted) && (_process)) {
        _isStarted = true;
        _windowID = 0;
        _process->start("nice -n 10 /home/radio/startNavit.sh");
        if (_isVisible) {
            show();
        }
        else {
            hide();
        }
    }
}

void Navit::stop() {
    if ((_isStarted) && (_process)) {
        _isStarted = false;
        _process->terminate();
        _windowID = 0;
    }
}

void Navit::sendKey(enum NavitKey key) {
    QString keyName;
    QString cmdLine;
    if (!_windowID)
        return;

    qDebug() << "navit: sendKey" << key;
    switch (key) {
        case navitUp: keyName = "Up"; break;
        case navitRight: keyName = "Right"; break;
        case navitDown: keyName = "Down"; break;
        case navitLeft: keyName = "Left"; break;
        case navitReturn: keyName = "Return"; break;
        case navitEscape: keyName = "Escape"; break;
        default: return;
    }

    cmdLine = QString("xsendkey -window %1 %2").arg(_windowID).arg(keyName);
    QProcess::startDetached(cmdLine);
}

void Navit::show() {
    if (!_isStarted) {
        start();
    }
    if (_windowID > 0) {
        QProcess::execute(QString("xdotool windowmap %1").arg(_windowID));
        QProcess::execute(QString("xdotool windowactivate %1").arg(_windowID));
    }
    _isVisible = true;
}

void Navit::hide() {
    if (!_isStarted) {
        start();
    }
    if (_windowID > 0) {
        QProcess::execute(QString("xdotool windowunmap %1").arg(_windowID));
    }
    _isVisible = false;
}

uint32_t Navit::getWindowId() {
    return _windowID;
}
