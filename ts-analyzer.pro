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
LIBS += -L"$$_PRO_FILE_PWD_/3rdparty/ts-lib/libs" -lts
INCLUDEPATH += "$$_PRO_FILE_PWD_"/3rdparty/ts-lib/inc
INCLUDEPATH += "$$_PRO_FILE_PWD_"/3rdparty/qhexedit2

HEADERS += \
    3rdparty/ts-lib/inc/TsStatistics.h \
    3rdparty/ts-lib/inc/TsStandards.h \
    3rdparty/ts-lib/inc/TsParser.h \
    3rdparty/ts-lib/inc/TsPacketInfo.h \
    3rdparty/ts-lib/inc/TsDemuxer.h \
    3rdparty/ts-lib/inc/PsiTables.h \
    3rdparty/ts-lib/inc/PesPacket.h \
    3rdparty/ts-lib/inc/GetBits.hh \
    3rdparty/ts-lib/inc/GetBits.h \
    3rdparty/ts-lib/inc/CommonTypes.h

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
    parserdialog.ui

OTHER_FILES += \
    README.md

DEFINES += QHEXEDIT_EXPORTS

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
