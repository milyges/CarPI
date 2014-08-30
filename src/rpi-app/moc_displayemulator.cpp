/****************************************************************************
** Meta object code from reading C++ file 'displayemulator.h'
**
** Created: Wed Jun 25 19:01:31 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "displayemulator.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'displayemulator.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DisplayEmulator[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: signature, parameters, type, tag, flags
      16,   43,   51,   51, 0x05,
      52,   77,   51,   51, 0x05,
      82,  110,   51,   51, 0x05,
     115,  136,   51,   51, 0x05,
     142,  182,   51,   51, 0x05,
     203,   51,   51,   51, 0x05,

 // slots: signature, parameters, type, tag, flags
     221,  238,   51,   51, 0x08,
     241,   51,   51,   51, 0x08,
     256,   43,   51,   51, 0x08,
     287,   43,   51,   51, 0x0a,
     311,  329,   51,   51, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_DisplayEmulator[] = {
    "DisplayEmulator\0displayStatusChanged(bool)\0"
    "enabled\0\0displayIconsChanged(int)\0"
    "mask\0displayTextChanged(QString)\0text\0"
    "displayMenuShow(int)\0items\0"
    "displayMenuItemUpdate(int,QString,bool)\0"
    "item,text,isSelected\0displayMenuHide()\0"
    "_socketRecv(int)\0fd\0_syncTimeout()\0"
    "_packetSendDisplayStatus(bool)\0"
    "radioPowerChanged(bool)\0sendKeyEvent(int)\0"
    "keycode\0"
};

void DisplayEmulator::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        DisplayEmulator *_t = static_cast<DisplayEmulator *>(_o);
        switch (_id) {
        case 0: _t->displayStatusChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->displayIconsChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->displayTextChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->displayMenuShow((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->displayMenuItemUpdate((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 5: _t->displayMenuHide(); break;
        case 6: _t->_socketRecv((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->_syncTimeout(); break;
        case 8: _t->_packetSendDisplayStatus((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 9: _t->radioPowerChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 10: _t->sendKeyEvent((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData DisplayEmulator::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject DisplayEmulator::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_DisplayEmulator,
      qt_meta_data_DisplayEmulator, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DisplayEmulator::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DisplayEmulator::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DisplayEmulator::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DisplayEmulator))
        return static_cast<void*>(const_cast< DisplayEmulator*>(this));
    return QObject::qt_metacast(_clname);
}

int DisplayEmulator::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void DisplayEmulator::displayStatusChanged(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void DisplayEmulator::displayIconsChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void DisplayEmulator::displayTextChanged(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void DisplayEmulator::displayMenuShow(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void DisplayEmulator::displayMenuItemUpdate(int _t1, QString _t2, bool _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void DisplayEmulator::displayMenuHide()
{
    QMetaObject::activate(this, &staticMetaObject, 5, 0);
}
QT_END_MOC_NAMESPACE
