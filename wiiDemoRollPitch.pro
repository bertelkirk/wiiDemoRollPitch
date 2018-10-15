TEMPLATE = app

QT += qml quick
CONFIG += c++11

QMAKE_CXXFLAGS += -fpermissive -std=c++11 -O2 -W -m64 -pipe
#e

SOURCES += main.cpp \
    WiiRemoteSensor.cpp \
    SensorThread.cpp



INCLUDEPATH += /home/bertel/git/cwiid/libcwiid/ /usr/include/
LIBS +=   -L/usr/lib -L/usr/local/lib -lcwiid -ldl



RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    WiiRemoteSensor.h \
    SensorThread.h \
    ../../cwiid/libcwiid/cwiid.h

