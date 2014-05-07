#include "mp3player.h"
#include <QProcess>
#include <QDir>
#include <QDebug>
#include <QApplication>
#include <QRegExp>

MP3Player::MP3Player(QObject *parent) : QObject(parent) {
    _state = mp3PlayerStateStopped;
    _worker = new MP3PlayerWorker();
    _worker_thread = new QThread(this);
    _worker->moveToThread(_worker_thread);

    connect(this, SIGNAL(worker_stop()), _worker, SLOT(stop()));
    connect(this, SIGNAL(worker_start()), _worker, SLOT(start()));
    connect(this, SIGNAL(worker_next_track()), _worker, SLOT(next_track()));
    connect(this, SIGNAL(worker_prev_track()), _worker, SLOT(prev_track()));
    connect(this, SIGNAL(worker_pause(bool)), _worker, SLOT(set_paused(bool)));
    connect(this, SIGNAL(worker_next_album()), _worker, SLOT(next_album()));
    connect(this, SIGNAL(worker_prev_album()), _worker, SLOT(prev_album()));
    connect(this, SIGNAL(worker_display_album()), _worker, SLOT(display_album()));

    connect(_worker, SIGNAL(text_changed(QString)), this, SLOT(worker_text_changed(QString)));

    _worker_thread->start();
}

MP3Player::~MP3Player() {
    _worker_thread->quit();
    _worker_thread->wait();
    delete _worker_thread;
    delete _worker;
}

void MP3Player::enable() {    
    if (_state != mp3PlayerStateStopped)
        return;

    _state = mp3PlayerStatePlaying;
    emit worker_start();
}

void MP3Player::disable() {
    if (_state != mp3PlayerStateStopped) {
        _state = mp3PlayerStateStopped;
        emit worker_stop();
    }
}

bool MP3Player::is_paused() {
    return (_state == mp3PlayerStatePaused);
}

void MP3Player::next_track() {
    emit worker_next_track();
}

void MP3Player::prev_track() {
    emit worker_prev_track();
}

void MP3Player::pause() {
    if (_state == mp3PlayerStatePlaying) {
        _state = mp3PlayerStatePaused;
        emit worker_pause(true);
    }
}

void MP3Player::play() {
    if (_state == mp3PlayerStateStopped) {
        enable();
    }
    else if (_state == mp3PlayerStatePaused) {
        _state = mp3PlayerStatePlaying;
        emit worker_pause(false);
    }
}

void MP3Player::stop() {
    if (_state != mp3PlayerStateStopped) {
        _state = mp3PlayerStateStopped;
        emit worker_stop();
    }
}

void MP3Player::numkey_pressed(int num) {
    switch(num) {
        case 0x01: emit worker_next_album(); break; /* Album + */
        case 0x02: emit worker_prev_album(); break; /* Album - */
        case 0x05: emit worker_display_album(); break; /* Display Album */
    }
}

void MP3Player::worker_text_changed(QString text) {
    if (_state != mp3PlayerStateStopped)
        emit text_changed(text);
}


void MP3PlayerWorker::start() {
    QString devname;
    QString lastuuid;
    QDir devdir("/dev", "sd*", QDir::Name | QDir::Reversed, QDir::System);
    QDir rootdir("/mnt/usb");
    QFileInfoList filelist;    
    int err, album, track;
    qint64 trackpos;

    qDebug() << "MP3PlayerWorker: start";
    emit text_changed(" USB");

    /* Wykrywanie urządzenia i montowanie */
    filelist = devdir.entryInfoList();

    if (filelist.size() == 0) {
        emit text_changed("USB: No device");
        return;
    }

    devname = filelist.at(0).absoluteFilePath();
    qDebug() << "MP3PlayerWorker: Detected device at" << devname;

    QProcess::execute("umount /mnt/usb");
    err = QProcess::execute(QString("mount %1 /mnt/usb").arg(devname));
    if (err != 0) {
        emit text_changed("USB: Device Error");
        return;
    }
    _uuid = _device_uuid(devname);

    emit text_changed("USB: Reading");

    _album_list.clear();
    _album_list.append(""); /* Główny katalog */
    _album_list.append(rootdir.entryList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name));

    //qDebug() << "MP3PlayerWorker: Album list:" << _album_list;
    _album_current = -1;

    lastuuid = _settings.value("mp3player/uuid").toString();
    album = _settings.value("mp3player/album").toInt();
    track = _settings.value("mp3player/track").toInt();
    trackpos = _settings.value("mp3player/trackpos").toULongLong();

    if ((_uuid.isEmpty()) || (lastuuid != _uuid) || (album >= _album_list.size())) { /* Nowa sesja */
        album = 0;
        track = 0;
        trackpos = 0;
    }

    _album_load(album);

    if (track >= _track_list.size()) { /* Nowa sesja */
        _album_load(0);
        track = 0;
        trackpos = 0;
    }

    _track_load(track);
    if (trackpos > 0) {
        _media_object->seek(trackpos);
    }

}

void MP3PlayerWorker::next_track() {
    int num;

    num = _track_current + 1;
    if (num >= _track_list.size()) { /* Następny album */
        next_album(false);
        num = 0;
    }

    _track_load(num);
}

void MP3PlayerWorker::prev_track() {
    int num;

    if (_media_object->currentTime() > 5000) { /* Jeżeli dalej niż 5 sekund ścieżki przesuń do początku */
        _media_object->seek(0);
        return;
    }

    num = _track_current - 1;
    if (num < 0) { /* Poprzedni album, ostatnia ścieżka */
        prev_album(false);
        num = _track_list.size() - 1;
    }

    _track_load(num);
}

void MP3PlayerWorker::set_paused(bool paused) {
    if (paused) {
        qDebug() << "MP3Player: pause()";
        _media_object->pause();
    }
    else {
        _media_object->play();
    }
}

void MP3PlayerWorker::next_album(bool trackload) {
    int num;

    num = _album_current + 1;
    if (num >= _album_list.size()) {
        num = 0;
    }

    _album_load(num);
    if (trackload)
        _track_load(0);
}

void MP3PlayerWorker::prev_album(bool trackload) {
    int num;

    num = _album_current - 1;
    if (num < 0) {
        num = _album_list.size() - 1;
    }

    _album_load(num);
    if (trackload)
        _track_load(0);
}

void MP3PlayerWorker::display_album() {
    _display_mode = displayModeAlbum;
    _display_update();
    _display_mode = displayModeTitle;
    if (_display_timer->isActive())
        _display_timer->stop();
    _display_timer->start();
}

void MP3PlayerWorker::_playback_finished() {
    next_track();
}

void MP3PlayerWorker::_display_update() {
    QString text;
    switch(_display_mode) {
        case displayModeTitle: {
            text = _track_list.at(_track_current);
            break;
        }
        case displayModeAlbum: {
            text = _album_list.at(_album_current);
            if (text.isEmpty())
                text = "Album: Root";
            else
                text.prepend("Album: ");
            break;
        }
    }

    if (!text.isEmpty())
        emit text_changed(text);
}

void MP3PlayerWorker::stop() {
    _settings.setValue("mp3player/uuid", _uuid);
    _settings.setValue("mp3player/album", _album_current);
    _settings.setValue("mp3player/track", _track_current);
    _settings.setValue("mp3player/trackpos", _media_object->currentTime());

    _media_object->stop();

    QProcess::execute("umount /mnt/usb");

    qDebug() << "MP3PlayerWorker: done";
}

MP3PlayerWorker::MP3PlayerWorker(QObject *parent) : QObject(parent) {
    _display_timer = new QTimer(this);
    _display_timer->setInterval(7000);
    _display_timer->setSingleShot(true);
    _display_mode = displayModeTitle;
    connect(_display_timer, SIGNAL(timeout()), this, SLOT(_display_update()));

    _media_files.clear();
    _media_files.append("*.mp3");
    _media_files.append("*.wav");
    _media_files.append("*.ogg");
    _media_files.append("*.wma");
    _media_files.append("*.flac");

    _media_object = new Phonon::MediaObject(this);
    _audio_output = new Phonon::AudioOutput(Phonon::MusicCategory, this);

    Phonon::createPath(_media_object, _audio_output);

    connect(_media_object, SIGNAL(stateChanged(Phonon::State, Phonon::State)), this, SLOT(_state_changed(Phonon::State, Phonon::State)));
    connect(_media_object, SIGNAL(finished()), this, SLOT(_playback_finished()));
}

MP3PlayerWorker::~MP3PlayerWorker() {
    delete _media_object;
    delete _audio_output;
}

void MP3PlayerWorker::_album_load(int num) {
    QDir albumdir(QString("/mnt/usb/%1").arg(_album_list.at(num)));

    _album_current = num;
    if (_album_list.at(num).isEmpty()) {
        emit text_changed("Album: Root");
    }
    else {
        emit text_changed(QString("Album: %1").arg(_album_list.at(num)));
    }

    albumdir.setNameFilters(_media_files);
    albumdir.setSorting(QDir::Name);
    albumdir.setFilter(QDir::Files);

    _track_list.clear();
    _track_list.append(albumdir.entryList());

    //qDebug() << "MP3PlayerWorker: Loaded track list:" << _track_list;
}

void MP3PlayerWorker::_track_load(int num) {    
    _track_current = num;

    _media_object->stop();

    Phonon::MediaSource trackfile(QString("/mnt/usb/%1/%2").arg(_album_list.at(_album_current)).arg(_track_list.at(num)));

    _media_object->setCurrentSource(trackfile);
    _media_object->play();
}

QString MP3PlayerWorker::_device_uuid(QString devpath) {
    QProcess proc;
    QString result;
    QRegExp uuid_regexp("UUID=\"([A-Fa-F0-9\\-]+)\"");

    proc.start("blkid", QStringList() << devpath);
    proc.waitForFinished();
    result = QString(proc.readAll()).trimmed();

    if (uuid_regexp.indexIn(result) != -1)
        return uuid_regexp.cap(1);
    else
        return "";
}

void MP3PlayerWorker::_state_changed(Phonon::State newstate, Phonon::State oldstate) {
    switch(newstate) {
    case Phonon::PlayingState: _display_update(); break;
        case Phonon::LoadingState: emit text_changed("USB: Loading"); break;
        case Phonon::ErrorState: emit text_changed("USB: Device error"); break;
        case Phonon::PausedState: emit text_changed("   PAUSE"); break;
    }
}
