#-------------------------------------------------
#
# Project created by QtCreator 2013-12-08T01:33:11
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = map_gui
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    mapwidget.cpp

HEADERS  += mainwindow.h \
    mapwidget.h

FORMS    += mainwindow.ui

LIBS        += -L.\vlc-qt\lib\ -lvlc-qt -lvlc-qt-widgets
INCLUDEPATH += .\vlc-qt\include\

INCLUDEPATH += .\..\common\
INCLUDEPATH += .\..\common\exiv2\msvc64\include\

LIBS        += -Lc:\boost_1_55_0\lib64-msvc-12.0\ -llibboost_filesystem-vc120-mt-1_55
INCLUDEPATH += $(BOOST_ROOT)
