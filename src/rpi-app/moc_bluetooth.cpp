/****************************************************************************
** Meta object code from reading C++ file 'bluetooth.h'
**
** Created: Wed Jun 25 19:01:40 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "bluetooth.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'bluetooth.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Bluetooth[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      10,   39,   51,   51, 0x05,
      52,   89,   51,   51, 0x05,

 // slots: signature, parameters, type, tag, flags
      99,   51,   51,   51, 0x08,
     119,   51,   51,   51, 0x08,
     141,   51,   51,   51, 0x0a,
     154,   51,   51,   51, 0x0a,
     167,   51,   51,   51, 0x0a,
     183,  199,   51,   51, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Bluetooth[] = {
    "Bluetooth\0connectionStateChanged(bool)\0"
    "isConnected\0\0callStateChanged(BluetoothCallState)\0"
    "callState\0_tryReconnectLast()\0"
    "_bluetoothInterrupt()\0acceptCall()\0"
    "rejectCall()\0terminateCall()\0"
    "dialTo(QString)\0number\0"
};

void Bluetooth::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Bluetooth *_t = static_cast<Bluetooth *>(_o);
        switch (_id) {
        case 0: _t->connectionStateChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->callStateChanged((*reinterpret_cast< BluetoothCallState(*)>(_a[1]))); break;
        case 2: _t->_tryReconnectLast(); break;
        case 3: _t->_bluetoothInterrupt(); break;
        case 4: _t->acceptCall(); break;
        case 5: _t->rejectCall(); break;
        case 6: _t->terminateCall(); break;
        case 7: _t->dialTo((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Bluetooth::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Bluetooth::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Bluetooth,
      qt_meta_data_Bluetooth, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Bluetooth::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Bluetooth::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Bluetooth::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Bluetooth))
        return static_cast<void*>(const_cast< Bluetooth*>(this));
    return QObject::qt_metacast(_clname);
}

int Bluetooth::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void Bluetooth::connectionStateChanged(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Bluetooth::callStateChanged(BluetoothCallState _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
