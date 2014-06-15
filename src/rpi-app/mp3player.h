#ifndef MP3PLAYER_H
#define MP3PLAYER_H

#include <QObject>
#include <QThread>
#include <QTimer>
#include <QFileInfoList>
#include <QStringList>
#include <QSettings>
#include <phonon/MediaObject>
#include <phonon/AudioOutput>
#include <phonon/Path>
#include <phonon/MediaSource>

enum MP3PlayerState {
    mp3PlayerStandby = 1,
    mp3PlayerStarting,
    mp3PlayerPlaying,
    mp3PlayerPaused,
    mp3PlayerError
};

enum MP3PlayerDisplayMode {
    mp3PlayerDisplayTitle = 1,
    mp3PlayerDisplayAlbum
};


class MP3Player : public QObject {
    Q_OBJECT
private:
    static MP3Player * _instance;
    static QThread _workerThread;
    static QStringList _mediaFileTypes;
    QSettings _settings;

    enum MP3PlayerDisplayMode _displayMode;
    enum MP3PlayerState _state;

    Phonon::MediaObject * _mediaObject;
    Phonon::AudioOutput * _audioOutput;

    QString _deviceUUID;

    QStringList _albumList;
    int _albumCurrent;
    QStringList _trackList;
    int _trackCurrent;

    void _start(void);
    bool _albumLoad(int number);
    bool _trackLoad(int number);
    QString _deviceGetUUID(QString path);

    explicit MP3Player(QObject *parent = 0);
    ~MP3Player();

private slots:
    void _mediaObjectStateChanged(Phonon::State newstate, Phonon::State oldstate);
    void _mediaObjectPlaybackFinished(void);
    void _updateText(void);

public:
    static MP3Player * getInstance(void);
    
signals:
    void textChanged(QString text);

public slots:
    void play(void);
    void pause(void);
    void stop(void);

    void nextTrack(void);
    void prevTrack(void);

    void nextAlbum(bool loadTrack = true);
    void prevAlbum(bool loadTrack = true);

    void switchDisplayMode(void);
};

#endif // MP3PLAYER_H
