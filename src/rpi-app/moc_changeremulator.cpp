/****************************************************************************
** Meta object code from reading C++ file 'changeremulator.h'
**
** Created: Wed Jun 25 19:01:49 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "changeremulator.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'changeremulator.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ChangerEmulator[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: signature, parameters, type, tag, flags
      16,   34,   34,   34, 0x05,
      35,   34,   34,   34, 0x05,
      53,   34,   34,   34, 0x05,
      70,   34,   34,   34, 0x05,
      82,   34,   34,   34, 0x05,
      94,  106,   34,   34, 0x05,

 // slots: signature, parameters, type, tag, flags
     110,   34,   34,   34, 0x08,
     124,   34,   34,   34, 0x08,
     139,   34,   34,   34, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_ChangerEmulator[] = {
    "ChangerEmulator\0playbackStopped()\0\0"
    "playbackStarted()\0playbackPaused()\0"
    "nextTrack()\0prevTrack()\0loadCD(int)\0"
    "num\0_packetRecv()\0_ioTimerTick()\0"
    "_watchdogTick()\0"
};

void ChangerEmulator::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        ChangerEmulator *_t = static_cast<ChangerEmulator *>(_o);
        switch (_id) {
        case 0: _t->playbackStopped(); break;
        case 1: _t->playbackStarted(); break;
        case 2: _t->playbackPaused(); break;
        case 3: _t->nextTrack(); break;
        case 4: _t->prevTrack(); break;
        case 5: _t->loadCD((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->_packetRecv(); break;
        case 7: _t->_ioTimerTick(); break;
        case 8: _t->_watchdogTick(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData ChangerEmulator::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject ChangerEmulator::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_ChangerEmulator,
      qt_meta_data_ChangerEmulator, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ChangerEmulator::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ChangerEmulator::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ChangerEmulator::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ChangerEmulator))
        return static_cast<void*>(const_cast< ChangerEmulator*>(this));
    return QObject::qt_metacast(_clname);
}

int ChangerEmulator::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void ChangerEmulator::playbackStopped()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void ChangerEmulator::playbackStarted()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void ChangerEmulator::playbackPaused()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void ChangerEmulator::nextTrack()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}

// SIGNAL 4
void ChangerEmulator::prevTrack()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}

// SIGNAL 5
void ChangerEmulator::loadCD(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}
QT_END_MOC_NAMESPACE
