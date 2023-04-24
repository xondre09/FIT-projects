#-------------------------------------------------
#
# Project created by QtCreator 2017-04-29T18:42:04
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET   = hra2017
TEMPLATE = app

CONFIG   += c++11

RESOURCES = resources.qrc

SOURCES += solitairegui.cc\
           card.cc \
           cardgui.cc \
           solitairewindow.cc \
           undo.cc \
           game.cc \
           hint.cc \
           main.cc

HEADERS += solitairegui.h \
           cardgui.h \
           solitairewindow.h \
           undo.h \
           game.h \
           card.h \
           hint.h \
           main.h
