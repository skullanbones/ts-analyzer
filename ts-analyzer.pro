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
INCLUDEPATH += "$$_PRO_FILE_PWD_"/3rdparty/qhexedit2
INCLUDEPATH += "$$_PRO_FILE_PWD_"/ts-lib/include
INCLUDEPATH += "$$_PRO_FILE_PWD_"/3rdparty/plog-1.1.4/include

# ts-lib
HEADERS += \
    ts-lib/include/TsStatistics.h \
    ts-lib/include/TsStandards.h \
    ts-lib/include/TsParser.h \
    ts-lib/include/TsPacketInfo.h \
    ts-lib/include/TsDemuxer.h \
    ts-lib/include/PsiTables.h \
    ts-lib/include/PesPacket.h \
    ts-lib/include/GetBits.h \
    ts-lib/include/CommonTypes.h

SOURCES += \
    ts-lib/src/GetBits.cc \
    ts-lib/src/TsDemuxer.cc \
    ts-lib/src/TsParser.cc \
    ts-lib/src/TsStatistics.cc \

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
    README.md

DEFINES += QHEXEDIT_EXPORTS

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
