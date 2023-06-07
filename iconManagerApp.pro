QT += core gui
QT += svg
QT += xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = iconManagerApp
TEMPLATE = app

HEADERS += iconManager.h mainwindow.h
SOURCES += main.cpp iconManager.cpp mainwindow.cpp
RESOURCES += resources.qrc