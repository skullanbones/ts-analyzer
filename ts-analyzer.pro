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
# ts-lib
LIBS += -L"$$_PRO_FILE_PWD_/3rdparty/ts-lib/lib" -lmpeg2ts
INCLUDEPATH += "$$_PRO_FILE_PWD_"/3rdparty/ts-lib
INCLUDEPATH += "$$_PRO_FILE_PWD_"/3rdparty/qhexedit2

# ts-lib
HEADERS += \
    3rdparty/ts-lib/include/mpeg2ts.h \
    3rdparty/ts-lib/include/Ts_IEC13818-1.h \
    3rdparty/ts-lib/include/TsUtilities.h \

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
    3rdparty/ts-lib/include/settings.json

DEFINES += QHEXEDIT_EXPORTS

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
