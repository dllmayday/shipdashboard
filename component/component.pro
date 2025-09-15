QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11 dll

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include(../base.pri)

TEMPLATE = lib

TARGET = shipdashboard

SOURCES += \
    adiwidget.cpp \
    compasswidget.cpp \
    controlpanel.cpp \
    depthgauge.cpp \
    gaugewidget.cpp \
    radarwidget.cpp

HEADERS += \
    adiwidget.h \
    compasswidget.h \
    controlpanel.h \
    depthgauge.h \
    gaugewidget.h \
    radarwidget.h

FORMS += \

DESTDIR   = $${BASE_APP_PATH}
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    component.qrc
