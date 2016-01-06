#-------------------------------------------------
#
# Project created by QtCreator 2015-12-31T15:14:46
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = dwFontExporter
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    wrapfreetype.cpp

HEADERS  += widget.h \
    wrapfreetype.h

INCLUDEPATH += $$_PRO_FILE_PWD_/freetype/include

LIBS += -lfreetype

FORMS    += widget.ui

RESOURCES += \
    resources.qrc
