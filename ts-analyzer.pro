#-------------------------------------------------
#
# Project created by QtCreator 2018-04-25T20:00:31
#
#-------------------------------------------------


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ts-analyzer
CONFIG += c++11

HEADERS = \
    mainwindow.h \
    optionsdialog.h \
    searchdialog.h \
    parserdialog.h

SOURCES = \
    main.cpp \
    mainwindow.cpp \
    optionsdialog.cpp \
    searchdialog.cpp \
    parserdialog.cpp

### 3rd-party libs ###
# mpeg2ts
release {
    LIBS += -L"$$_PRO_FILE_PWD_/3rdparty/mpeg2ts/lib" -lmpeg2ts
}
debug {
    LIBS += -L"$$_PRO_FILE_PWD_/3rdparty/mpeg2ts/lib" -lmpeg2ts-d
}
INCLUDEPATH += "$$_PRO_FILE_PWD_"/3rdparty/mpeg2ts/include
INCLUDEPATH += "$$_PRO_FILE_PWD_"/3rdparty/qhexedit2

# mpeg2ts
HEADERS += \
    3rdparty/mpeg2ts/include/mpeg2ts/mpeg2ts.h \
    3rdparty/mpeg2ts/include/mpeg2ts/Ts_IEC13818-1.h \
    3rdparty/mpeg2ts/include/mpeg2ts/TsUtilities.h \

# hexedit2
HEADERS += \
    3rdparty/qhexedit2/qhexedit.h \
    3rdparty/qhexedit2/chunks.h \
    3rdparty/qhexedit2/commands.h \

SOURCES += \
    3rdparty/qhexedit2/qhexedit.cpp \
    3rdparty/qhexedit2/chunks.cpp \
    3rdparty/qhexedit2/commands.cpp \

RESOURCES = \
    application.qrc

FORMS += \
    optionsdialog.ui \
    searchdialog.ui \
    parserdialog.ui \
    mainwindow.ui

OTHER_FILES += \
    README.md \
    3rdparty/mpeg2ts/include/settings.json

DEFINES += QHEXEDIT_EXPORTS

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

#ifndef Q_OS_WIN32
DEFINES += MPEG2TS_STATIC_DEFINE
#endif
