/****************************************************************************
** Meta object code from reading C++ file 'elm327.h'
**
** Created: Wed Jun 25 19:02:38 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "elm327.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'elm327.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Elm327[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
       7,   41,   50,   50, 0x05,
      51,   74,   50,   50, 0x05,

 // slots: signature, parameters, type, tag, flags
      82,   50,   50,   50, 0x08,
      97,  114,   50,   50, 0x0a,
     118,   50,   50,   50, 0x0a,
     126,   50,   50,   50, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Elm327[] = {
    "Elm327\0pidValueChanged(int,QVector<int>)\0"
    "pid,data\0\0voltageChanged(double)\0"
    "voltage\0_refreshData()\0addWatchPid(int)\0"
    "pid\0start()\0stop()\0"
};

void Elm327::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Elm327 *_t = static_cast<Elm327 *>(_o);
        switch (_id) {
        case 0: _t->pidValueChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QVector<int>(*)>(_a[2]))); break;
        case 1: _t->voltageChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 2: _t->_refreshData(); break;
        case 3: _t->addWatchPid((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->start(); break;
        case 5: _t->stop(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Elm327::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Elm327::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Elm327,
      qt_meta_data_Elm327, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Elm327::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Elm327::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Elm327::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Elm327))
        return static_cast<void*>(const_cast< Elm327*>(this));
    return QObject::qt_metacast(_clname);
}

int Elm327::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void Elm327::pidValueChanged(int _t1, QVector<int> _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Elm327::voltageChanged(double _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
