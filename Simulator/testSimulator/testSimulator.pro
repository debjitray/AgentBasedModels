TEMPLATE = app
CONFIG += console
CONFIG -= qt

SOURCES += main.cpp \
    testcell.cpp \
    testsim.cpp \
    ../simulator.cpp \
    ../cell.cpp

HEADERS += \
    testcell.h \
    testsim.h \
    ../simulator.h \
    ../cell.h \
    ../GlobalVariables.h \
    ../GlobalConstructs.h


