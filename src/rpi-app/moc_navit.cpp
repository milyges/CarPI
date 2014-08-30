/****************************************************************************
** Meta object code from reading C++ file 'navit.h'
**
** Created: Tue Sep 16 16:10:53 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "navit.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'navit.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Navit[] = {

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
       6,   47,   62,   62, 0x08,
      63,   62,   62,   62, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Navit[] = {
    "Navit\0_navitFinished(int,QProcess::ExitStatus)\0"
    "err,exitStatus\0\0_readFromNavit()\0"
};

void Navit::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Navit *_t = static_cast<Navit *>(_o);
        switch (_id) {
        case 0: _t->_navitFinished((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QProcess::ExitStatus(*)>(_a[2]))); break;
        case 1: _t->_readFromNavit(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Navit::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Navit::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Navit,
      qt_meta_data_Navit, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Navit::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Navit::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Navit::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Navit))
        return static_cast<void*>(const_cast< Navit*>(this));
    return QObject::qt_metacast(_clname);
}

int Navit::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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
