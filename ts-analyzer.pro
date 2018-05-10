#-------------------------------------------------
#
# Project created by QtCreator 2018-04-25T20:00:31
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ts-analyzer
TEMPLATE = app

# 3rd-party libs
LIBS += -L"$$_PRO_FILE_PWD_/ts-lib/libs" -lts
INCLUDEPATH += "$$_PRO_FILE_PWD_"/ts-lib/inc

# Resources/assets
RESOURCES     = application.qrc

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp

HEADERS += \
        mainwindow.h \
    ts-lib/inc/TsStatistics.h \
    ts-lib/inc/TsStandards.h \
    ts-lib/inc/TsParser.h \
    ts-lib/inc/TsPacketInfo.h \
    ts-lib/inc/TsDemuxer.h \
    ts-lib/inc/PsiTables.h \
    ts-lib/inc/PesPacket.h \
    ts-lib/inc/GetBits.hh \
    ts-lib/inc/GetBits.h \
    ts-lib/inc/CommonTypes.h

FORMS += \
        mainwindow.ui

DISTFILES += \
    README.md
