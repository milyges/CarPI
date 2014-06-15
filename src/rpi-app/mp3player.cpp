#include "mp3player.h"
#include <QDir>
#include <QProcess>
#include <QDebug>
#include <QRegExp>
#include <QFile>

MP3Player * MP3Player::_instance = NULL;
QThread MP3Player::_workerThread;
QStringList MP3Player::_mediaFileTypes;

MP3Player::MP3Player(QObject *parent) : QObject(parent) {
    _state = mp3PlayerStandby;
    _displayMode = mp3PlayerDisplayTitle;

    _mediaObject = new Phonon::MediaObject(this);
    _audioOutput = new Phonon::AudioOutput(Phonon::MusicCategory, this);

    Phonon::createPath(_mediaObject, _audioOutput);

    connect(_mediaObject, SIGNAL(stateChanged(Phonon::State,Phonon::State)), this, SLOT(_mediaObjectStateChanged(Phonon::State,Phonon::State)));
    connect(_mediaObject, SIGNAL(finished()), this, SLOT(_mediaObjectPlaybackFinished()));
}

MP3Player::~MP3Player() {
    delete _audioOutput;
    delete _mediaObject;
}

void MP3Player::_mediaObjectStateChanged(Phonon::State newstate, Phonon::State oldstate) {
    switch(newstate) {
        case Phonon::PlayingState: _updateText(); break;
        case Phonon::LoadingState: emit textChanged("USB: Loading"); break;
    }
}

MP3Player *MP3Player::getInstance() {
    if (!_instance) {
        _mediaFileTypes.clear();
        _mediaFileTypes.append("*.mp3");
        _mediaFileTypes.append("*.ogg");
        _mediaFileTypes.append("*.wma");
        _mediaFileTypes.append("*.wav");
        _mediaFileTypes.append("*.flac");

        _instance = new MP3Player();
        _instance->moveToThread(&_workerThread);
        _workerThread.start(QThread::HighestPriority);
    }

    return _instance;
}

void MP3Player::play() {
    if (_state == mp3PlayerStandby) {
        _start();
    }

    if (_state == mp3PlayerPaused) {
        _mediaObject->play();
        _state = mp3PlayerPlaying;
    }
}

void MP3Player::pause() {
    if (_state == mp3PlayerPlaying) {
        _mediaObject->pause();
        _state = mp3PlayerPaused;
    }
}

void MP3Player::stop() {
    if (_state == mp3PlayerStandby) { /* Juz wyłączony */
        return;
    }
    
    if (_state != mp3PlayerError) {
        _settings.setValue("mp3player/uuid", _deviceUUID);
        _settings.setValue("mp3player/album", _albumCurrent);
        _settings.setValue("mp3player/track", _trackCurrent);
        _settings.setValue("mp3player/trackTime", _mediaObject->currentTime());
    }

    _mediaObject->stop();
    //QProcess::execute("umount /mnt/usb");
    _state = mp3PlayerStandby;
    qDebug() << "MP3Player: Stopped";
}

void MP3Player::nextTrack() {
    int num;

    num = _trackCurrent + 1;
    if (num >= _trackList.size()) {
        nextAlbum(false);
        num = 0;
    }

    if ((_trackList.isEmpty()) || (!_trackLoad(num))) {
        emit textChanged("USB: Disk error");
        _state = mp3PlayerError;
        stop();
        return;
    }

    _mediaObject->play();
}

void MP3Player::prevTrack() {
    int num;

    if (_mediaObject->currentTime() > 5000) {
        _mediaObject->seek(0);
        return;
    }

    num = _trackCurrent - 1;
    if (num < 0) {
        prevAlbum(false);
        num = _trackList.size() - 1;
    }

    _trackLoad(num);
    _mediaObject->play();
}

void MP3Player::nextAlbum(bool loadTrack) {
    int num;
    int start = _albumCurrent;
    bool loaded;

    do {
        num = _albumCurrent + 1;
        if (num >= _albumList.size()) {
            num = 0;
        }

        loaded = _albumLoad(num);
    } while ((start != _albumCurrent) && (!loaded));

    qDebug() << "MPPlayer: Loaded:" << loaded << ", num =" << _albumCurrent;
    if (!loaded) {
       //emit textChanged("USB: Empty disk");
       _state = mp3PlayerError;
       stop();
    }
    else if (loadTrack) {
        _trackLoad(0);
        _mediaObject->play();
    }
}

void MP3Player::prevAlbum(bool loadTrack) {
    int num;

    num = _albumCurrent - 1;
    if (num < 0) {
        num = _albumList.size() - 1;
    }

    _albumLoad(num);

    if (loadTrack) {
        _trackLoad(0);
        _mediaObject->play();
    }
}

void MP3Player::switchDisplayMode() {
    if (_displayMode == mp3PlayerDisplayAlbum)
        _displayMode = mp3PlayerDisplayTitle;
    else
        _displayMode = mp3PlayerDisplayAlbum;

    _updateText();
}

void MP3Player::_start() {
    QString devName;
//    QDir devDir("/dev", "sd*", QDir::Name | QDir::Reversed, QDir::System);
    QDir rootDir("/mnt/usb");
    QString lastUUID, line;
    QFile mountsFile("/proc/mounts");
    QStringList fields;
    int album, track;
    qint64 trackTime;
    QFileInfoList filelist;

    _state = mp3PlayerStarting;
    qDebug() << "MP3Player: Starting";
    emit textChanged("USB");

    if (!mountsFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "MP3Player: open /proc/mounts failed";
    }

    QTextStream mountsStream(&mountsFile);

    while(1) {
        line = mountsStream.readLine();
	if (line.isEmpty())
            break;
        fields = line.split(QRegExp("\\s+"));
	
        if (fields.at(1) == "/mnt/usb") {
            devName = fields.at(0);
            break;
        }
    }

    mountsFile.close();
    /* Wykrywanie urządzenia i montowanie */
 //   filelist = devDir.entryInfoList();

 //   if (filelist.size() == 0) {
 //       emit textChanged("USB: No device");
 //       return;
 //   }

  //  devName = filelist.at(0).absoluteFilePath();
    qDebug() << "MP3PlayerWorker: Detected device at" << devName;

    if (devName.isEmpty()) {
         emit textChanged("USB: No device");
         return;
    }

    //QProcess::execute("umount /mnt/usb");
    //err = QProcess::execute(QString("mount -o codepage=852,iocharset=iso8859-2,utf8 %1 /mnt/usb").arg(devName));
/*    if (err != 0) {
        _state = mp3PlayerError;
        emit textChanged("USB: Mount error");
        return;
    }*/

    _deviceUUID = _deviceGetUUID(devName);

    emit textChanged("USB: Reading");

    _albumList.clear();
    _albumList.append("");
    _albumList.append(rootDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name | QDir::IgnoreCase));
    _albumCurrent = -1;

    lastUUID = _settings.value("mp3player/uuid").toString();
    album = _settings.value("mp3player/album").toInt();
    track = _settings.value("mp3player/track").toInt();
    trackTime = _settings.value("mp3player/trackTime").toULongLong();

    if ((_deviceUUID.isEmpty()) || (lastUUID != _deviceUUID) || (album >= _albumList.size())) { /* Nowa sesja */
        album = 0;
        track = 0;
        trackTime = 0;
    }

    _albumLoad(album);

    if (track >= _trackList.size()) { /* Nowa sesja */
        _albumLoad(0);
        track = 0;
        trackTime = 0;
    }

    _trackLoad(track);
    _mediaObject->play();

    if (trackTime > 0) {
        _mediaObject->seek(trackTime);
    }

    _state = mp3PlayerPlaying;
    qDebug() << "MP3Player: Started";
}

bool MP3Player::_albumLoad(int number) {
    QDir albumDir(QString("/mnt/usb/%1").arg(_albumList.at(number)));

    _albumCurrent = number;

    _displayMode = mp3PlayerDisplayAlbum;
    _updateText();

    albumDir.setNameFilters(_mediaFileTypes);
    albumDir.setSorting(QDir::Name | QDir::IgnoreCase);
    albumDir.setFilter(QDir::Files);

    _trackList.clear();
    _trackList.append(albumDir.entryList());

    //qDebug() << "MP3Player: Track list" << _trackList;

    return !_trackList.isEmpty(); /* Zwracamy false jezeli nie ma żadnej ściezki */
}

bool MP3Player::_trackLoad(int number) {
    QString fileName;

    if ((number < 0) || (number >= _trackList.size()))
        return false;

    _trackCurrent = number;

    _mediaObject->stop();
    _displayMode = mp3PlayerDisplayTitle;

    fileName = QString("/mnt/usb/%1/%2").arg(_albumList.at(_albumCurrent)).arg(_trackList.at(number));
    if (!QFile(fileName).exists()) {
        return false;
    }

    Phonon::MediaSource trackfile(fileName);

    _mediaObject->setCurrentSource(trackfile);

    return true;
}

QString MP3Player::_deviceGetUUID(QString path) {
    QProcess proc;
    QString result;
    QRegExp uuidRegexp("UUID=\"([A-Fa-F0-9\\-]+)\"");

    proc.start("blkid", QStringList() << path);
    proc.waitForFinished();
    result = QString(proc.readAll()).trimmed();

    if (uuidRegexp.indexIn(result) != -1)
        return uuidRegexp.cap(1);
    else
        return "";
}

void MP3Player::_mediaObjectPlaybackFinished() {
    nextTrack();
}

void MP3Player::_updateText() {
    QString text;
    switch(_displayMode) {
        case mp3PlayerDisplayTitle: {
            text = _trackList.at(_trackCurrent);
            break;
        }
        case mp3PlayerDisplayAlbum: {
            text = _albumList.at(_albumCurrent);
            if (text.isEmpty())
                text = "ROOT";
            text.prepend("Album: ");
            break;
        }
    }

    if (!text.isEmpty())
        emit textChanged(text);
}
