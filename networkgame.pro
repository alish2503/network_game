QT       += core gui widgets network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

INCLUDEPATH += include

DESTDIR = ./bin

win32:LIBS += -lws2_32

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += main.cpp \
    src/choiceswindow.cpp \
    src/gameclient.cpp \
    src/gameserver.cpp \
    src/mainwindow.cpp \
    src/resultwindow.cpp \
    src/waitwindow.cpp

HEADERS += \
    include/GameConstants.h \
    include/choiceswindow.h \
    include/gameclient.h \
    include/gameserver.h \
    include/mainwindow.h \
    include/resultwindow.h \
    include/waitwindow.h

FORMS += \
    ui/choiceswindow.ui \
    ui/mainwindow.ui \
    ui/resultwindow.ui \
    ui/waitwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
