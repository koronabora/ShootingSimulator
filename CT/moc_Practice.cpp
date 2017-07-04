/****************************************************************************
** Meta object code from reading C++ file 'Practice.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "Practice.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Practice.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Practice_t {
    QByteArrayData data[12];
    char stringdata0[80];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Practice_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Practice_t qt_meta_stringdata_Practice = {
    {
QT_MOC_LITERAL(0, 0, 8), // "Practice"
QT_MOC_LITERAL(1, 9, 9), // "plotPoint"
QT_MOC_LITERAL(2, 19, 0), // ""
QT_MOC_LITERAL(3, 20, 2), // "id"
QT_MOC_LITERAL(4, 23, 1), // "x"
QT_MOC_LITERAL(5, 25, 1), // "y"
QT_MOC_LITERAL(6, 27, 4), // "type"
QT_MOC_LITERAL(7, 32, 15), // "plotMoveToPoint"
QT_MOC_LITERAL(8, 48, 9), // "newValues"
QT_MOC_LITERAL(9, 58, 8), // "targetId"
QT_MOC_LITERAL(10, 67, 7), // "newShot"
QT_MOC_LITERAL(11, 75, 4) // "time"

    },
    "Practice\0plotPoint\0\0id\0x\0y\0type\0"
    "plotMoveToPoint\0newValues\0targetId\0"
    "newShot\0time"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Practice[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    4,   34,    2, 0x06 /* Public */,
       7,    4,   43,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    3,   52,    2, 0x0a /* Public */,
      10,    4,   59,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::ULongLong, QMetaType::QReal, QMetaType::QReal, QMetaType::ULongLong,    3,    4,    5,    6,
    QMetaType::Void, QMetaType::ULongLong, QMetaType::QReal, QMetaType::QReal, QMetaType::ULongLong,    3,    4,    5,    6,

 // slots: parameters
    QMetaType::Void, QMetaType::ULongLong, QMetaType::QReal, QMetaType::QReal,    9,    4,    5,
    QMetaType::Void, QMetaType::ULongLong, QMetaType::QReal, QMetaType::QReal, QMetaType::ULongLong,    9,    4,    5,   11,

       0        // eod
};

void Practice::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Practice *_t = static_cast<Practice *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->plotPoint((*reinterpret_cast< quint64(*)>(_a[1])),(*reinterpret_cast< qreal(*)>(_a[2])),(*reinterpret_cast< qreal(*)>(_a[3])),(*reinterpret_cast< quint64(*)>(_a[4]))); break;
        case 1: _t->plotMoveToPoint((*reinterpret_cast< quint64(*)>(_a[1])),(*reinterpret_cast< qreal(*)>(_a[2])),(*reinterpret_cast< qreal(*)>(_a[3])),(*reinterpret_cast< quint64(*)>(_a[4]))); break;
        case 2: _t->newValues((*reinterpret_cast< quint64(*)>(_a[1])),(*reinterpret_cast< qreal(*)>(_a[2])),(*reinterpret_cast< qreal(*)>(_a[3]))); break;
        case 3: _t->newShot((*reinterpret_cast< quint64(*)>(_a[1])),(*reinterpret_cast< qreal(*)>(_a[2])),(*reinterpret_cast< qreal(*)>(_a[3])),(*reinterpret_cast< quint64(*)>(_a[4]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (Practice::*_t)(quint64 , qreal , qreal , quint64 );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Practice::plotPoint)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (Practice::*_t)(quint64 , qreal , qreal , quint64 );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Practice::plotMoveToPoint)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject Practice::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Practice.data,
      qt_meta_data_Practice,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Practice::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Practice::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Practice.stringdata0))
        return static_cast<void*>(const_cast< Practice*>(this));
    return QObject::qt_metacast(_clname);
}

int Practice::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void Practice::plotPoint(quint64 _t1, qreal _t2, qreal _t3, quint64 _t4)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Practice::plotMoveToPoint(quint64 _t1, qreal _t2, qreal _t3, quint64 _t4)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
