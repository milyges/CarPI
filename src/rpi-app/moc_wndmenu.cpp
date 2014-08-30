/****************************************************************************
** Meta object code from reading C++ file 'wndmenu.h'
**
** Created: Wed Jun 25 18:56:06 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "wndmenu.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'wndmenu.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_WndMenu[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
       8,   22,   28,   28, 0x0a,
      29,   28,   28,   28, 0x0a,
      40,   70,   28,   28, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_WndMenu[] = {
    "WndMenu\0showMenu(int)\0items\0\0hideMenu()\0"
    "setMenuItem(int,QString,bool)\0"
    "itemno,text,isSelected\0"
};

void WndMenu::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        WndMenu *_t = static_cast<WndMenu *>(_o);
        switch (_id) {
        case 0: _t->showMenu((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->hideMenu(); break;
        case 2: _t->setMenuItem((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData WndMenu::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject WndMenu::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_WndMenu,
      qt_meta_data_WndMenu, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &WndMenu::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *WndMenu::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *WndMenu::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_WndMenu))
        return static_cast<void*>(const_cast< WndMenu*>(this));
    return QDialog::qt_metacast(_clname);
}

int WndMenu::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
