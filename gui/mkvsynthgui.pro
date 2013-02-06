QT += core gui network

TARGET = mkvsynthGUI

TEMPLATE = app

MOC_DIR = buildfiles
OBJECTS_DIR = buildfiles

SOURCES += main.cpp \
           mainwindow.cpp \
           qmpwidget/qmpwidget.cpp \
           videowidget.cpp \
           videobar.cpp

HEADERS += mainwindow.h \
           qmpwidget/qmpwidget.h \
           qmpwidget/qmpyuvreader.h \
           videowidget.h \
           videobar.h 
