/****************************************************************************
** Meta object code from reading C++ file 'germcellsimulator.h'
**
** Created: Sat Jul 7 15:44:14 2012
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "germcellsimulator.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'germcellsimulator.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_GermCellSimulator[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
      17,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      19,   18,   18,   18, 0x0a,
      42,   33,   28,   18, 0x0a,
      76,   64,   28,   18, 0x0a,
     102,   33,   28,   18, 0x0a,
     125,   33,   28,   18, 0x0a,
     150,   18,   18,   18, 0x0a,
     167,   18,   18,   18, 0x0a,
     177,   18,   18,   18, 0x08,
     195,   18,   18,   18, 0x08,
     212,   18,   18,   18, 0x08,
     229,   18,   18,   18, 0x08,
     251,  245,   18,   18, 0x08,
     267,  245,   18,   18, 0x08,
     284,   18,   18,   18, 0x08,
     294,   18,   18,   18, 0x08,
     305,   18,   18,   18, 0x08,
     319,   18,   18,   18, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_GermCellSimulator[] = {
    "GermCellSimulator\0\0update()\0bool\0"
    "filename\0saveCSV_inner(string)\0"
    "usrFileName\0savePosData_inner(string)\0"
    "saveData_inner(string)\0saveErrors_inner(string)\0"
    "loadSimulation()\0savePic()\0startSimulation()\0"
    "stopSimulation()\0startRecording()\0"
    "stopRecording()\0value\0changeZoom(int)\0"
    "changeSpeed(int)\0saveCSV()\0saveData()\0"
    "savePosData()\0saveErrors()\0"
};

const QMetaObject GermCellSimulator::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_GermCellSimulator,
      qt_meta_data_GermCellSimulator, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &GermCellSimulator::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *GermCellSimulator::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *GermCellSimulator::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GermCellSimulator))
        return static_cast<void*>(const_cast< GermCellSimulator*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int GermCellSimulator::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: update(); break;
        case 1: { bool _r = saveCSV_inner((*reinterpret_cast< string(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 2: { bool _r = savePosData_inner((*reinterpret_cast< string(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 3: { bool _r = saveData_inner((*reinterpret_cast< string(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 4: { bool _r = saveErrors_inner((*reinterpret_cast< string(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 5: loadSimulation(); break;
        case 6: savePic(); break;
        case 7: startSimulation(); break;
        case 8: stopSimulation(); break;
        case 9: startRecording(); break;
        case 10: stopRecording(); break;
        case 11: changeZoom((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: changeSpeed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 13: saveCSV(); break;
        case 14: saveData(); break;
        case 15: savePosData(); break;
        case 16: saveErrors(); break;
        default: ;
        }
        _id -= 17;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
