/****************************************************************************
** Meta object code from reading C++ file 'wndmain.h'
**
** Created: Tue Sep 16 16:41:20 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "wndmain.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'wndmain.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_WndMain[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
       8,   35,   40,   40, 0x08,
      41,   76,   40,   40, 0x08,
      95,  124,   40,   40, 0x08,
     134,  162,   40,   40, 0x08,
     169,  191,   40,   40, 0x08,
     195,   40,   40,   40, 0x08,
     209,   40,   40,   40, 0x08,
     229,   40,   40,   40, 0x08,
     248,   40,   40,   40, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_WndMain[] = {
    "WndMain\0_radioTextChanged(QString)\0"
    "text\0\0_radioIconsChanged(bool,bool,bool)\0"
    "news,traffic,afrds\0_bluetoothStateChanged(bool)\0"
    "connected\0_sourceChanged(CarPISource)\0"
    "source\0_keyPressed(CarPIKey)\0key\0"
    "_updateTime()\0_navigationScreen()\0"
    "_dashboardScreen()\0_radioScreen()\0"
};

void WndMain::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        WndMain *_t = static_cast<WndMain *>(_o);
        switch (_id) {
        case 0: _t->_radioTextChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->_radioIconsChanged((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 2: _t->_bluetoothStateChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->_sourceChanged((*reinterpret_cast< CarPISource(*)>(_a[1]))); break;
        case 4: _t->_keyPressed((*reinterpret_cast< CarPIKey(*)>(_a[1]))); break;
        case 5: _t->_updateTime(); break;
        case 6: _t->_navigationScreen(); break;
        case 7: _t->_dashboardScreen(); break;
        case 8: _t->_radioScreen(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData WndMain::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject WndMain::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_WndMain,
      qt_meta_data_WndMain, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &WndMain::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *WndMain::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *WndMain::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_WndMain))
        return static_cast<void*>(const_cast< WndMain*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int WndMain::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
