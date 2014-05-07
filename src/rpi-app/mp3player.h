#ifndef MP3PLAYER_H
#define MP3PLAYER_H

#include <QObject>
#include <QFileInfoList>
#include <QStringList>
#include <QThread>
#include <QSettings>
#include <QTimer>
#include <phonon/MediaObject>
#include <phonon/AudioOutput>
#include <phonon/Path>
#include <phonon/MediaSource>


enum {
    mp3PlayerStateStopped = 1,
    mp3PlayerStateStarting,
    mp3PlayerStatePlaying,
    mp3PlayerStatePaused
};

enum {
    displayModeTitle = 1,
    displayModeAlbum
};

class MP3PlayerWorker : public QObject {
    Q_OBJECT
private:
    QStringList _media_files;
    QStringList _album_list;
    int _album_current;
    QStringList _track_list;
    int _track_current;
    QString _uuid;
    QSettings _settings;
    int _display_mode;
    QTimer * _display_timer;

    Phonon::MediaObject * _media_object;
    Phonon::AudioOutput * _audio_output;

    void _album_load(int num);
    void _track_load(int num);
    QString _device_uuid(QString devpath);

private slots:
    void _state_changed(Phonon::State newstate, Phonon::State oldstate);
    void _playback_finished(void);
    void _display_update(void);

public slots:
    void stop(void);
    void start(void);
    void next_track(void);
    void prev_track(void);
    void set_paused(bool paused);
    void next_album(bool trackload = true);
    void prev_album(bool trackload = true);
    void display_album(void);

public:
    MP3PlayerWorker(QObject *parent = 0);
    ~MP3PlayerWorker();


signals:
    void text_changed(QString s);
};

class MP3Player : public QObject {
    Q_OBJECT
private:
    MP3PlayerWorker * _worker;
    QThread * _worker_thread;
    int _state;

signals:
    void text_changed(QString title);
    void worker_stop(void);
    void worker_start(void);
    void worker_next_track(void);
    void worker_prev_track(void);
    void worker_pause(bool paused);
    void worker_next_album(void);
    void worker_prev_album(void);
    void worker_display_album(void);

public slots:
    void worker_text_changed(QString text);
    void next_track(void);
    void prev_track(void);

    void pause(void);
    void play(void);
    void stop(void);

    void numkey_pressed(int num);

public:
    explicit MP3Player(QObject *parent = 0);
    ~MP3Player();
    void enable(void);
    void disable(void);
    bool is_paused(void);
};

#endif // MP3PLAYER_H
