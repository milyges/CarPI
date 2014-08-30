/****************************************************************************
** Meta object code from reading C++ file 'carpi.h'
**
** Created: Tue Sep 16 16:10:32 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "carpi.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'carpi.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CarPI[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      33,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      19,       // signalCount

 // signals: signature, parameters, type, tag, flags
       6,   33,   40,   40, 0x05,
      41,   60,   40,   40, 0x05,
      67,   91,   40,   40, 0x05,
      98,  124,   40,   40, 0x05,
     129,  163,   40,   40, 0x05,
     182,  204,   40,   40, 0x05,
     212,  226,   40,   40, 0x05,
     232,   40,   40,   40, 0x05,
     243,  273,   40,   40, 0x05,
     296,  322,   40,   40, 0x05,
     326,  364,   40,   40, 0x05,
     376,  422,   40,   40, 0x05,
     428,   40,   40,   40, 0x05,
     449,   40,   40,   40, 0x05,
     470,   40,   40,   40, 0x05,
     499,  522,   40,   40, 0x05,
     526,   40,   40,   40, 0x05,
     539,   40,   40,   40, 0x05,
     553,  591,   40,   40, 0x05,

 // slots: signature, parameters, type, tag, flags
     604,  124,   40,   40, 0x08,
     633,  659,   40,   40, 0x08,
     668,  226,   40,   40, 0x08,
     690,   40,   40,   40, 0x08,
     709,  273,   40,   40, 0x08,
     747,  364,   40,   40, 0x08,
     786,  422,   40,   40, 0x08,
     833,  860,   40,   40, 0x08,
     868,  124,   40,   40, 0x08,
     899,  927,   40,   40, 0x08,
     931,  959,   40,   40, 0x08,
     967, 1003,   40,   40, 0x08,
    1012, 1042,   40,   40, 0x08,
    1050,   40,   40,   40, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CarPI[] = {
    "CarPI\0sourceChanged(CarPISource)\0"
    "source\0\0volumeChanged(int)\0volume\0"
    "pauseStateChanged(bool)\0paused\0"
    "radioTextChanged(QString)\0text\0"
    "radioIconsChanged(bool,bool,bool)\0"
    "news,traffic,afrds\0radioNewKeyEvent(int)\0"
    "keycode\0menuShow(int)\0items\0menuHide()\0"
    "menuSetItem(int,QString,bool)\0"
    "itemno,text,isSelected\0pilotKeyPressed(CarPIKey)\0"
    "key\0bluetoothConnectionStateChanged(bool)\0"
    "isConnected\0bluetoothCallStateChanged(BluetoothCallState)\0"
    "state\0mp3PlayerNextAlbum()\0"
    "mp3PlayerPrevAlbum()\0mp3PlayerSwitchDisplayMode()\0"
    "elm327addWatchPid(int)\0pid\0elm327stop()\0"
    "elm327start()\0dialValueChanged(CarPIDialsID,double)\0"
    "dialID,value\0_displayTextChanged(QString)\0"
    "_displayIconsChanged(int)\0iconmask\0"
    "_displayMenuShow(int)\0_displayMenuHide()\0"
    "_displayMenuSetItem(int,QString,bool)\0"
    "_bluetoothConnectionStateChanged(bool)\0"
    "_bluetoothCallStateChanged(BluetoothCallState)\0"
    "_pilotKeyStateChanged(int)\0keymask\0"
    "_mp3PlayerTextChanged(QString)\0"
    "_changerEmulatorLoadCD(int)\0num\0"
    "_ignitionStateChanged(bool)\0enabled\0"
    "_elm327PidChanged(int,QVector<int>)\0"
    "pid,data\0_elm327VoltageChanged(double)\0"
    "voltage\0shutdown()\0"
};

void CarPI::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CarPI *_t = static_cast<CarPI *>(_o);
        switch (_id) {
        case 0: _t->sourceChanged((*reinterpret_cast< CarPISource(*)>(_a[1]))); break;
        case 1: _t->volumeChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->pauseStateChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->radioTextChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->radioIconsChanged((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 5: _t->radioNewKeyEvent((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->menuShow((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->menuHide(); break;
        case 8: _t->menuSetItem((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 9: _t->pilotKeyPressed((*reinterpret_cast< CarPIKey(*)>(_a[1]))); break;
        case 10: _t->bluetoothConnectionStateChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 11: _t->bluetoothCallStateChanged((*reinterpret_cast< BluetoothCallState(*)>(_a[1]))); break;
        case 12: _t->mp3PlayerNextAlbum(); break;
        case 13: _t->mp3PlayerPrevAlbum(); break;
        case 14: _t->mp3PlayerSwitchDisplayMode(); break;
        case 15: _t->elm327addWatchPid((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 16: _t->elm327stop(); break;
        case 17: _t->elm327start(); break;
        case 18: _t->dialValueChanged((*reinterpret_cast< CarPIDialsID(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 19: _t->_displayTextChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 20: _t->_displayIconsChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 21: _t->_displayMenuShow((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 22: _t->_displayMenuHide(); break;
        case 23: _t->_displayMenuSetItem((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 24: _t->_bluetoothConnectionStateChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 25: _t->_bluetoothCallStateChanged((*reinterpret_cast< BluetoothCallState(*)>(_a[1]))); break;
        case 26: _t->_pilotKeyStateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 27: _t->_mp3PlayerTextChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 28: _t->_changerEmulatorLoadCD((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 29: _t->_ignitionStateChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 30: _t->_elm327PidChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QVector<int>(*)>(_a[2]))); break;
        case 31: _t->_elm327VoltageChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 32: _t->shutdown(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CarPI::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CarPI::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CarPI,
      qt_meta_data_CarPI, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CarPI::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CarPI::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CarPI::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CarPI))
        return static_cast<void*>(const_cast< CarPI*>(this));
    return QObject::qt_metacast(_clname);
}

int CarPI::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 33)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 33;
    }
    return _id;
}

// SIGNAL 0
void CarPI::sourceChanged(CarPISource _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CarPI::volumeChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CarPI::pauseStateChanged(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CarPI::radioTextChanged(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void CarPI::radioIconsChanged(bool _t1, bool _t2, bool _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void CarPI::radioNewKeyEvent(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void CarPI::menuShow(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void CarPI::menuHide()
{
    QMetaObject::activate(this, &staticMetaObject, 7, 0);
}

// SIGNAL 8
void CarPI::menuSetItem(int _t1, QString _t2, bool _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void CarPI::pilotKeyPressed(CarPIKey _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void CarPI::bluetoothConnectionStateChanged(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void CarPI::bluetoothCallStateChanged(BluetoothCallState _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}

// SIGNAL 12
void CarPI::mp3PlayerNextAlbum()
{
    QMetaObject::activate(this, &staticMetaObject, 12, 0);
}

// SIGNAL 13
void CarPI::mp3PlayerPrevAlbum()
{
    QMetaObject::activate(this, &staticMetaObject, 13, 0);
}

// SIGNAL 14
void CarPI::mp3PlayerSwitchDisplayMode()
{
    QMetaObject::activate(this, &staticMetaObject, 14, 0);
}

// SIGNAL 15
void CarPI::elm327addWatchPid(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 15, _a);
}

// SIGNAL 16
void CarPI::elm327stop()
{
    QMetaObject::activate(this, &staticMetaObject, 16, 0);
}

// SIGNAL 17
void CarPI::elm327start()
{
    QMetaObject::activate(this, &staticMetaObject, 17, 0);
}

// SIGNAL 18
void CarPI::dialValueChanged(CarPIDialsID _t1, double _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 18, _a);
}
QT_END_MOC_NAMESPACE
