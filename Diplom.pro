QT       += core gui sql testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Test/test_database.cpp \
    action.cpp \
    database.cpp \
    fileinout.cpp \
    main.cpp \
    mainwindow.cpp \
    mygraphicscene.cpp \
    operation.cpp

HEADERS += \
    Structures.h \
    Test/test_database.h \
    action.h \
    database.h \
    fileinout.h \
    mainwindow.h \
    mygraphicscene.h \
    operation.h

FORMS += \
    action.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RC_FILE = "file.rc"
