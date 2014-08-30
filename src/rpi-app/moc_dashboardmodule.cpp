/****************************************************************************
** Meta object code from reading C++ file 'dashboardmodule.h'
**
** Created: Tue Sep 16 16:42:40 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "dashboardmodule.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dashboardmodule.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DashboardModule[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      16,   27,   27,   27, 0x0a,
      28,   27,   27,   27, 0x0a,
      41,   62,   27,   27, 0x0a,
      66,   92,   27,   27, 0x0a,
      97,  131,   27,   27, 0x0a,
     150,  188,   27,   27, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_DashboardModule[] = {
    "DashboardModule\0activate()\0\0deactivate()\0"
    "keyPressed(CarPIKey)\0key\0"
    "radioTextChanged(QString)\0text\0"
    "radioIconsChanged(bool,bool,bool)\0"
    "news,traffic,afrds\0"
    "dialValueChanged(CarPIDialsID,double)\0"
    "dialID,value\0"
};

void DashboardModule::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        DashboardModule *_t = static_cast<DashboardModule *>(_o);
        switch (_id) {
        case 0: _t->activate(); break;
        case 1: _t->deactivate(); break;
        case 2: _t->keyPressed((*reinterpret_cast< CarPIKey(*)>(_a[1]))); break;
        case 3: _t->radioTextChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->radioIconsChanged((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 5: _t->dialValueChanged((*reinterpret_cast< CarPIDialsID(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData DashboardModule::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject DashboardModule::staticMetaObject = {
    { &GuiModule::staticMetaObject, qt_meta_stringdata_DashboardModule,
      qt_meta_data_DashboardModule, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DashboardModule::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DashboardModule::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DashboardModule::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DashboardModule))
        return static_cast<void*>(const_cast< DashboardModule*>(this));
    return GuiModule::qt_metacast(_clname);
}

int DashboardModule::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = GuiModule::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
