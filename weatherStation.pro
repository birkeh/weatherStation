QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    cairquality.cpp \
    cdisplayvalues.cpp \
    cmasterbrick.cpp \
    coutdoorweather.cpp \
    ctinkerforge.cpp \
    main.cpp \
    cmainwindow.cpp \
    tinkerforge/brick_master.c \
    tinkerforge/bricklet_air_quality.c \
    tinkerforge/bricklet_outdoor_weather.c \
    tinkerforge/ip_connection.c

HEADERS += \
    cairquality.h \
    cdisplayvalues.h \
    cmainwindow.h \
    cmasterbrick.h \
    coutdoorweather.h \
    ctinkerforge.h \
    tinkerforge/brick_master.h \
    tinkerforge/bricklet_air_quality.h \
    tinkerforge/bricklet_outdoor_weather.h \
    tinkerforge/ip_connection.h

FORMS += \
    cmainwindow.ui

win32:LIBS += -lws2_32 -ladvapi32
unix:QMAKE_CXXFLAGS += -pthread

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
