/****************************************************************************
** Meta object code from reading C++ file 'wndcall.h'
**
** Created: Wed Jun 25 18:56:28 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "wndcall.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'wndcall.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_WndCall[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
       8,   25,   25,   25, 0x08,
      26,   63,   25,   25, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_WndCall[] = {
    "WndCall\0_callTimerTick()\0\0"
    "callStateChanged(BluetoothCallState)\0"
    "state\0"
};

void WndCall::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        WndCall *_t = static_cast<WndCall *>(_o);
        switch (_id) {
        case 0: _t->_callTimerTick(); break;
        case 1: _t->callStateChanged((*reinterpret_cast< BluetoothCallState(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData WndCall::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject WndCall::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_WndCall,
      qt_meta_data_WndCall, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &WndCall::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *WndCall::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *WndCall::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_WndCall))
        return static_cast<void*>(const_cast< WndCall*>(this));
    return QDialog::qt_metacast(_clname);
}

int WndCall::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
