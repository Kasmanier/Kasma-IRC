#-------------------------------------------------
#
# Project created by QtCreator 2013-01-11T22:17:11
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Client
TEMPLATE = app


SOURCES += main.cpp\
        Client.cpp \
    Network.cpp \
    ChatWidget.cpp \
    ParserOutput.cpp \
    ParserInput.cpp \
    ConnectDialog.cpp

HEADERS  += Client.h \
    Globals.h \
    Network.h \
    ChatWidget.h \
    ParserOutput.h \
    ParserInput.h \
    ConnectDialog.h

FORMS    += Client.ui \
    ConnectDialog.ui
