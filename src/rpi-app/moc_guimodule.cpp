/****************************************************************************
** Meta object code from reading C++ file 'guimodule.h'
**
** Created: Tue Sep 16 16:41:42 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "guimodule.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'guimodule.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_GuiModule[] = {

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
      10,   21,   21,   21, 0x0a,
      22,   21,   21,   21, 0x0a,
      35,   56,   21,   21, 0x0a,
      60,   86,   21,   21, 0x0a,
      91,  125,   21,   21, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_GuiModule[] = {
    "GuiModule\0activate()\0\0deactivate()\0"
    "keyPressed(CarPIKey)\0key\0"
    "radioTextChanged(QString)\0text\0"
    "radioIconsChanged(bool,bool,bool)\0"
    "news,traffic,afrds\0"
};

void GuiModule::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        GuiModule *_t = static_cast<GuiModule *>(_o);
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

const QMetaObjectExtraData GuiModule::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject GuiModule::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_GuiModule,
      qt_meta_data_GuiModule, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &GuiModule::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *GuiModule::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *GuiModule::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GuiModule))
        return static_cast<void*>(const_cast< GuiModule*>(this));
    return QWidget::qt_metacast(_clname);
}

int GuiModule::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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
