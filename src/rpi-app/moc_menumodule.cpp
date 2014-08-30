/****************************************************************************
** Meta object code from reading C++ file 'menumodule.h'
**
** Created: Tue Sep 16 16:42:01 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "menumodule.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'menumodule.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MenuItem[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
       9,   21,   21,   21, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_MenuItem[] = {
    "MenuItem\0triggered()\0\0"
};

void MenuItem::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MenuItem *_t = static_cast<MenuItem *>(_o);
        switch (_id) {
        case 0: _t->triggered(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData MenuItem::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MenuItem::staticMetaObject = {
    { &QLabel::staticMetaObject, qt_meta_stringdata_MenuItem,
      qt_meta_data_MenuItem, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MenuItem::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MenuItem::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MenuItem::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MenuItem))
        return static_cast<void*>(const_cast< MenuItem*>(this));
    return QLabel::qt_metacast(_clname);
}

int MenuItem::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QLabel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void MenuItem::triggered()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
static const uint qt_meta_data_MenuModule[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      11,   22,   22,   22, 0x0a,
      23,   22,   22,   22, 0x0a,
      36,   57,   22,   22, 0x0a,
      61,   87,   22,   22, 0x0a,
      92,  126,   22,   22, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_MenuModule[] = {
    "MenuModule\0activate()\0\0deactivate()\0"
    "keyPressed(CarPIKey)\0key\0"
    "radioTextChanged(QString)\0text\0"
    "radioIconsChanged(bool,bool,bool)\0"
    "news,traffic,afrds\0"
};

void MenuModule::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MenuModule *_t = static_cast<MenuModule *>(_o);
        switch (_id) {
        case 0: _t->activate(); break;
        case 1: _t->deactivate(); break;
        case 2: _t->keyPressed((*reinterpret_cast< CarPIKey(*)>(_a[1]))); break;
        case 3: _t->radioTextChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->radioIconsChanged((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData MenuModule::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MenuModule::staticMetaObject = {
    { &GuiModule::staticMetaObject, qt_meta_stringdata_MenuModule,
      qt_meta_data_MenuModule, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MenuModule::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MenuModule::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MenuModule::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MenuModule))
        return static_cast<void*>(const_cast< MenuModule*>(this));
    return GuiModule::qt_metacast(_clname);
}

int MenuModule::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = GuiModule::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
