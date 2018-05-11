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
    qhexedit2/qhexedit.h \
    qhexedit2/chunks.h \
    qhexedit2/commands.h \
    searchdialog.h

# 3rd-party libs
LIBS += -L"$$_PRO_FILE_PWD_/ts-lib/libs" -lts
INCLUDEPATH += "$$_PRO_FILE_PWD_"/ts-lib/inc

SOURCES = \
    main.cpp \
    mainwindow.cpp \
    optionsdialog.cpp \
    qhexedit2/qhexedit.cpp \
    qhexedit2/chunks.cpp \
    qhexedit2/commands.cpp \
    searchdialog.cpp

RESOURCES = \
    application.qrc

FORMS += \
    optionsdialog.ui \
    searchdialog.ui

OTHER_FILES += \
    README.md

TRANSLATIONS += \
    translations/ts-analyzer_cs.ts \
    translations/ts-analyzer_de.ts \
    translations/ts-analyzer_ru.ts \

DEFINES += QHEXEDIT_EXPORTS

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
