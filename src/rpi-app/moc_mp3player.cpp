/****************************************************************************
** Meta object code from reading C++ file 'mp3player.h'
**
** Created: Wed Jun 25 19:01:58 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "mp3player.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mp3player.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MP3Player[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      10,   31,   36,   36, 0x05,

 // slots: signature, parameters, type, tag, flags
      37,   91,   36,   36, 0x08,
     109,   36,   36,   36, 0x08,
     140,   36,   36,   36, 0x08,
     154,   36,   36,   36, 0x0a,
     161,   36,   36,   36, 0x0a,
     169,   36,   36,   36, 0x0a,
     176,   36,   36,   36, 0x0a,
     188,   36,   36,   36, 0x0a,
     200,  216,   36,   36, 0x0a,
     226,   36,   36,   36, 0x2a,
     238,  216,   36,   36, 0x0a,
     254,   36,   36,   36, 0x2a,
     266,   36,   36,   36, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_MP3Player[] = {
    "MP3Player\0textChanged(QString)\0text\0"
    "\0_mediaObjectStateChanged(Phonon::State,Phonon::State)\0"
    "newstate,oldstate\0_mediaObjectPlaybackFinished()\0"
    "_updateText()\0play()\0pause()\0stop()\0"
    "nextTrack()\0prevTrack()\0nextAlbum(bool)\0"
    "loadTrack\0nextAlbum()\0prevAlbum(bool)\0"
    "prevAlbum()\0switchDisplayMode()\0"
};

void MP3Player::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MP3Player *_t = static_cast<MP3Player *>(_o);
        switch (_id) {
        case 0: _t->textChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->_mediaObjectStateChanged((*reinterpret_cast< Phonon::State(*)>(_a[1])),(*reinterpret_cast< Phonon::State(*)>(_a[2]))); break;
        case 2: _t->_mediaObjectPlaybackFinished(); break;
        case 3: _t->_updateText(); break;
        case 4: _t->play(); break;
        case 5: _t->pause(); break;
        case 6: _t->stop(); break;
        case 7: _t->nextTrack(); break;
        case 8: _t->prevTrack(); break;
        case 9: _t->nextAlbum((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 10: _t->nextAlbum(); break;
        case 11: _t->prevAlbum((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 12: _t->prevAlbum(); break;
        case 13: _t->switchDisplayMode(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData MP3Player::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MP3Player::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_MP3Player,
      qt_meta_data_MP3Player, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MP3Player::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MP3Player::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MP3Player::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MP3Player))
        return static_cast<void*>(const_cast< MP3Player*>(this));
    return QObject::qt_metacast(_clname);
}

int MP3Player::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    }
    return _id;
}

// SIGNAL 0
void MP3Player::textChanged(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
